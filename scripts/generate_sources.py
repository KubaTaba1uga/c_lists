import os
import sys

DEFAULT_PREFIX = "arl_"
NEW_PREFIX = os.environ["NEW_PREFIX"]


def regenerate_file(file_path):
    with open(file_path, "r") as src_fp:
        source = src_fp.read()

    new_source = source.replace(DEFAULT_PREFIX, NEW_PREFIX)

    new_file_name = NEW_PREFIX + os.path.basename(file_path)
    new_file_path = os.path.join(os.path.dirname(file_path), new_file_name)

    with open(new_file_path, "w") as dst_fp:
        dst_fp.write(new_source)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        raise ValueError("Provide at least one file path!")

    for arg in sys.argv[1:]:
        regenerate_file(arg)
