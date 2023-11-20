#!/bin/env python3
import os
import sys
from pathlib import Path

if len(sys.argv) < 4:
    raise ValueError(
        """Not enough arguments!
Syntax: python3 generate_sources.py <source file> <new prefix> <new type> (<dest dir>)
"""
    )


DEFAULT_TYPE = "ARL_VALUE_TYPE"
DEFAULT_PREFIX = "arl_"

file_path = sys.argv[1]
new_prefix = sys.argv[2] + "_"
new_type = sys.argv[3]


_THIS_DIR = os.path.dirname(os.path.abspath(__file__))
_SRC_DIR = os.path.join(_THIS_DIR, "..", "src")
_HEADERS_DIR = os.path.join(_THIS_DIR, "..", "include")


def regenerate_file(file_path):
    with open(file_path, "r") as src_fp:
        old_content = src_fp.read()

    new_content = regenerate_content(old_content)

    if len(sys.argv) == 5:
        file_path = os.path.join(sys.argv[4], os.path.basename(file_path))

    new_file_name = os.path.basename(file_path).replace(DEFAULT_PREFIX, new_prefix)

    new_file_path = os.path.join(os.path.dirname(file_path), new_file_name)

    with open(new_file_path, "w") as dst_fp:
        print(new_file_path)
        dst_fp.write(new_content)


def regenerate_content(file_content):
    new_content = file_content.replace(DEFAULT_TYPE, new_type)

    new_content = new_content.replace(
        DEFAULT_PREFIX.lower(), new_prefix.lower()
    ).replace(DEFAULT_PREFIX.upper(), new_prefix.upper())
    return new_content


if __name__ == "__main__":
    files_to_gen = [
        os.path.join(_SRC_DIR, Path(file_path).stem + ".c"),
        os.path.join(_HEADERS_DIR, Path(file_path).stem + ".h"),
    ]

    for file_ in files_to_gen:
        regenerate_file(file_)
