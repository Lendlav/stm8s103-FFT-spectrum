#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Compile the production FFT implementation for host-side signal checks."""

import os
import shlex
import subprocess
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
CC = shlex.split(os.environ.get("CC", "cc"))


def main() -> int:
    for size in (32, 64, 128):
        with tempfile.TemporaryDirectory(prefix="stm8-fft-test-") as temp_dir:
            executable = Path(temp_dir) / "test_fft"
            command = [
                *CC,
                "-std=c99",
                "-O2",
                "-Wall",
                "-Wextra",
                "-Werror",
                f"-DN_SAMPLE={size}",
                "-I",
                str(ROOT / "inc"),
                str(ROOT / "src" / "fft.c"),
                str(ROOT / "tests" / "test_fft.c"),
                "-lm",
                "-o",
                str(executable),
            ]
            subprocess.run(command, check=True)
            subprocess.run([str(executable)], check=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
