from __future__ import annotations

import shutil
import subprocess
import sys
import sysconfig
from pathlib import Path

import pybind11


ROOT = Path(__file__).resolve().parent
PROJECT = ROOT.parent
OUTPUT = ROOT / "alphablokus" / f"_alpha_game{sysconfig.get_config_var('EXT_SUFFIX')}"


def main() -> None:
    compiler = shutil.which("g++")
    if compiler is None:
        raise RuntimeError("g++ was not found on PATH")

    python_root = Path(sys.base_prefix)
    command = [
        compiler,
        "-std=c++20",
        "-O3",
        "-shared",
        "-static-libgcc",
        "-static-libstdc++",
        f"-I{pybind11.get_include()}",
        f"-I{sysconfig.get_paths()['include']}",
        f"-I{PROJECT / 'include'}",
        f"-I{ROOT / 'cpp'}",
        str(ROOT / "cpp" / "alpha_game.cpp"),
        str(ROOT / "cpp" / "bindings.cpp"),
        str(PROJECT / "src" / "board.cpp"),
        str(PROJECT / "src" / "piece.cpp"),
        str(PROJECT / "src" / "game.cpp"),
        f"-L{python_root / 'libs'}",
        f"-lpython{sys.version_info.major}{sys.version_info.minor}",
        "-o",
        str(OUTPUT),
    ]
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    print("Building", OUTPUT.name)
    subprocess.run(command, check=True)
    print("Built", OUTPUT)


if __name__ == "__main__":
    main()
