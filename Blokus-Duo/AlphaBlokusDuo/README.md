# AlphaBlokusDuo

既存のC++版Blokus Duoをゲームエンジンとして再利用する、小規模なAlphaZero MVPです。
既存の`main.cpp`やMCTS実装から独立しており、ゲームルールはPython側へ複製しません。

## MVPの構成

- C++/pybind11: 盤面、合法手、着手、終局、既存AIとの接続
- PyTorch: 49チャンネル入力の方策・価値ネットワーク
- PUCT: ニューラルネットワークの事前確率と価値を使う探索
- Self-play: 自己対戦から`(state, policy, value)`を生成
- Trainer: 方策損失と価値損失による学習
- Arena: RANDOM、通常MCTS、評価付きMCTSとの比較

方策空間は`21 pieces × 8 orientations × 14 × 14 + PASS = 32,929`です。
合法手以外は探索時にマスクされます。現行エンジンと同様、対称なピースの重複回転はMVPでは残しています。

## セットアップ（PowerShell）

```powershell
cd D:\研究\Master-Research(github)\Blokus-Duo_AI\Blokus-Duo\AlphaBlokusDuo
python -m venv .venv
.\.venv\Scripts\Activate.ps1
python -m pip install --upgrade pip
python -m pip install -r requirements.txt
python build_extension.py
```

上記でCPU版が入った場合、NVIDIA GPU用のCUDA 12.6版へ切り替えます。

```powershell
python -m pip install --force-reinstall torch==2.14.0 --index-url https://download.pytorch.org/whl/cu126
```

GPUを確認します。

```powershell
python -m scripts.inspect_gpu
```

## 動作確認

```powershell
python -m unittest discover -v
python -m scripts.smoke_test
```

`smoke_test`は、C++接続、テンソル化、ネットワーク、PUCT、自己対戦1局、学習1回を順に確認します。

## 小規模学習

```powershell
python -m scripts.train
```

既定値は自己対戦2局、各着手32シミュレーション、32チャンネル・残差ブロック2個です。
これは強いAIを作る設定ではなく、学習ループの成立を確認するための設定です。

## 反復学習

```powershell
python -m scripts.train_loop
```

反復学習では、各イテレーションで次の処理を行います。

1. 現在の最良モデルで自己対局を生成
2. 過去の局面を含むリプレイバッファから候補モデルを学習
3. 候補モデルと最良モデルを先後同数で対戦
4. 候補の獲得ポイント率が基準以上なら最良モデルへ昇格

既定値は10イテレーション、各イテレーション自己対局10局、各着手64シミュレーション、
学習3エポック、新旧モデル対戦20局、昇格基準55%です。引き分けは0.5ポイントとして扱います。

長時間実行を小さく区切る場合は、到達させたい総イテレーション数を指定します。

```powershell
python -m scripts.train_loop --iterations 2
python -m scripts.train_loop --iterations 5
python -m scripts.train_loop --iterations 10
```

2回目以降は保存状態から自動的に再開します。`--iterations 5`は5回を追加する指定ではなく、
「合計5イテレーションまで進める」という指定です。未完了イテレーションを再開するときは、
自己対局数、シミュレーション数、学習回数、対戦数、昇格基準を変更しないでください。

主な保存先は次のとおりです。

- `checkpoints/best.pt`: 現在の最良モデル
- `checkpoints/iterations/iteration_XXXX.pt`: 各候補モデル
- `self_play_data/replay_buffer.npz`: 疎形式で保存した学習局面
- `logs/training_state.json`: 再開用の進行状態
- `logs/training_iterations.jsonl`: イテレーション別の結果

最良モデルを既存AIと比較する場合はチェックポイントを指定します。

```powershell
python -m scripts.evaluate --checkpoint checkpoints/best.pt --baseline random --games 20
python -m scripts.evaluate --checkpoint checkpoints/best.pt --baseline standard --games 20 --baseline-iterations 100
python -m scripts.evaluate --checkpoint checkpoints/best.pt --baseline eval --games 20 --baseline-iterations 100
```

## 既存AIとの比較

```powershell
python -m scripts.evaluate --baseline random --games 4
python -m scripts.evaluate --baseline standard --games 4 --baseline-iterations 100
python -m scripts.evaluate --baseline eval --games 4 --baseline-iterations 100
```

偶数局ではAlphaZero側の先後を入れ替えます。チェックポイントが存在しない場合は、未学習モデルとして評価されます。

## 観測49チャンネル

1. 現手番プレイヤーの配置
2. 相手の配置
3. 現手番プレイヤーの着手可能点
4. 相手の着手可能点
5. 自分の残りピース21面
6. 相手の残りピース21面
7. 自分の正規化スコア
8. 相手の正規化スコア
9. 連続パス数

盤面は常に「現在手番のプレイヤー」を自分として正規化されます。
