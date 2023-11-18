#!/bin/env python3
import os
import sys
from pathlib import Path

DEFAULT_PREFIX = "arl_"
NEW_PREFIX = os.environ["NEW_PREFIX"]

_THIS_DIR = os.path.dirname(os.path.abspath(__file__))
_SRC_DIR = os.path.join(_THIS_DIR, "..", "src")
_HEADERS_DIR = os.path.join(_THIS_DIR, "..", "include", "c_lists")


def regenerate_file(file_path):
    with open(file_path, "r") as src_fp:
        source = src_fp.read()

    if len(sys.argv) == 3:
        file_path = os.path.join(sys.argv[2], os.path.basename(file_path))
        print(file_path)

    # New file's source creation
    new_content = source.replace(DEFAULT_PREFIX, NEW_PREFIX)
    # New file's name creation
    new_file_name = os.path.basename(file_path).replace(DEFAULT_PREFIX, NEW_PREFIX)

    new_file_path = os.path.join(os.path.dirname(file_path), new_file_name)
    with open(new_file_path, "w") as dst_fp:
        dst_fp.write(new_content)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        raise ValueError("Providing file path is required!")

    input_file = sys.argv[1]

    files_to_gen = [
        os.path.join(_SRC_DIR, Path(input_file).stem + ".c"),
        os.path.join(_HEADERS_DIR, Path(input_file).stem + ".h"),
    ]

    for file_ in files_to_gen:
        regenerate_file(file_)
