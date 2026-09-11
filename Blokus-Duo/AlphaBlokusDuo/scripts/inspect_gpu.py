from __future__ import annotations

import torch


def main() -> None:
    print(f"PyTorch: {torch.__version__}")
    print(f"CUDA available: {torch.cuda.is_available()}")
    if torch.cuda.is_available():
        print(f"CUDA runtime: {torch.version.cuda}")
        print(f"GPU: {torch.cuda.get_device_name(0)}")
        properties = torch.cuda.get_device_properties(0)
        print(f"GPU memory: {properties.total_memory / 1024**3:.1f} GiB")
    else:
        print("Device: CPU")


if __name__ == "__main__":
    main()
