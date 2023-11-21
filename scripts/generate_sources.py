#!/bin/env python3
import os
import re
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
# TO-DO get DEFAULT_PREFIX and DEFAULT_TYPE dyanmically, based on file_path

file_path = sys.argv[1]
new_prefix = sys.argv[2] + "_"
new_type = sys.argv[3]


_THIS_DIR = os.path.dirname(os.path.abspath(__file__))
_SRC_DIR = os.path.join(_THIS_DIR, "..", "src")
_HEADERS_DIR = os.path.join(_THIS_DIR, "..", "include")


def main():
    files_to_gen = [
        os.path.join(_SRC_DIR, Path(file_path).stem + ".c"),
        os.path.join(_HEADERS_DIR, Path(file_path).stem + ".h"),
    ]

    for file_ in files_to_gen:
        regenerate_file(file_)


def regenerate_file(file_path: str) -> None:
    with open(file_path, "r") as src_fp:
        old_content = src_fp.read()

    new_content = regenerate_content(old_content)

    if len(sys.argv) == 5:
        file_path = os.path.join(sys.argv[4], os.path.basename(file_path))

    new_file_name = os.path.basename(file_path).replace(DEFAULT_PREFIX, new_prefix)

    new_file_path = os.path.join(os.path.dirname(file_path), new_file_name)

    with open(new_file_path, "w") as dst_fp:
        dst_fp.write(new_content)
        print("Generated:", new_file_path)


def regenerate_content(file_content: str) -> str:
    regeneration_functions = [
        sanitize_content,
        lambda string: string.replace(DEFAULT_TYPE, new_type),
        lambda string: string.replace(
            DEFAULT_PREFIX.lower(), new_prefix.lower()
        ).replace(DEFAULT_PREFIX.upper(), new_prefix.upper()),
    ]

    for func in regeneration_functions:
        file_content = func(file_content)

    return file_content


def sanitize_content(file_content: str) -> str:
    # definitons need to be deleted, they break compilation
    regex = r"#ifndef ARL_VALUE_TYPE(\n^(?!#endif$).*)+\n#endif"
    return re.sub(regex, "\n", file_content, flags=re.M)


if __name__ == "__main__":
    main()
