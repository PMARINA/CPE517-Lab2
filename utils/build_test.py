#!/usr/bin/python3
"""Compile tests for easy testing."""
import codecs
import os
import shutil
import subprocess
import tempfile
from typing import Generator

import magic  # python-magic
from loguru import logger

TEST_DIRECTORY = os.path.abspath("test/")
COMPILE_FILETYPE = ".asm"
OUTPUT_FILETYPE = ".txt"
PATH_WITHOUT_TEST = os.path.join(*(os.path.split(TEST_DIRECTORY)[:-1]))
OUTPUT_DIRECTORY = os.path.join(PATH_WITHOUT_TEST, "build", "test")


def paths_to_compile(
    in_dir: str = TEST_DIRECTORY, file_type: str = COMPILE_FILETYPE
) -> Generator[str, None, None]:
    """Return filepaths that are compileable.

    Args:
        in_dir (str, optional): The directory that should be recursively parsed. Defaults to TEST_DIRECTORY.
        file_type (str, optional): The filetype that should be returned exclusively. Blank for no selectivity. Defaults to COMPILE_FILETYPE.

    Yields:
        Generator[str, None, None]: Generator that returns filepaths.
    """
    for root, _, files in os.walk(in_dir):
        for file_name in files:
            if file_name.strip().lower().endswith(file_type):
                yield os.path.join(root, file_name)


def setup_output(out: str = OUTPUT_DIRECTORY) -> None:
    """Empty the output directory. Create it if it doesn'temp_output exist.

    Args:
        out (str, optional): The filepath to the directory. Defaults to OUTPUT_DIRECTORY.
    """
    if os.path.exists(out):
        shutil.rmtree(out)
    os.makedirs(out)


def mips_compile(input_file: str, output_file: str) -> bool:
    """Compile the assembly into hextext.

    Args:
        input_file (str): File to compile
        output_file (str): Filepath to output to

    Returns:
        bool: Whether or not the file compiled successfully
    """
    try:
        result = subprocess.run(
            f"java -jar utils/Mars4_5.jar a {input_file} dump .text HexText {output_file}",
            check=True,  # If the process ends with error code, raise an exception.
            shell=True,  # Apparently it's important
            capture_output=True,
        )
        if result.stdout.decode("utf-8").lower().contains("error"):
            print(result.stdout)
            return False
        return True
    except subprocess.CalledProcessError:
        return False


def fix_encoding(input_path: str) -> None:
    """Ensure encoding of input is utf-8.

    Note that this only modifies files if they are not in utf-8 format.

    Args:
        input_path (str): The path to the file to fix
    """
    blob = open(input_path, "rb").read()
    magic_inst = magic.open(magic.MAGIC_MIME_ENCODING)
    magic_inst.load()
    encoding = magic_inst.buffer(blob)
    if encoding.strip().lower() != "utf-8":
        with tempfile.TemporaryFile() as temp_output:
            with open(input_path, "rb") as input_file:
                temp_output.write(input_file.read().decode(encoding).encode("utf-8"))
            temp_output.seek(0)
            with open(input_path, "wb") as output_file:
                content = temp_output.read()
                if content.startswith(codecs.BOM_UTF8):
                    content = content[len(codecs.BOM_UTF8)]
                output_file.write(content)


def ensure_output_dir(filepath: str) -> None:
    """Ensure output directory exists for writing to.

    Args:
        filepath (str): The path to the file to be created.
    """
    outdir = os.path.join(*(os.path.split(filepath)[:-1]))
    if not os.path.exists(outdir):
        os.makedirs(outdir)


def main():
    """Compile all assembly files in test directory."""
    setup_output()
    for input_path in paths_to_compile():
        path_relative_test = os.path.relpath(input_path, start=TEST_DIRECTORY)
        fix_encoding(input_path)

        if COMPILE_FILETYPE and OUTPUT_FILETYPE:
            output_file = os.path.join(
                OUTPUT_DIRECTORY,
                path_relative_test.replace(COMPILE_FILETYPE, OUTPUT_FILETYPE),
            )
        elif OUTPUT_FILETYPE:
            output_file = os.path.join(
                OUTPUT_DIRECTORY, "".join([path_relative_test, OUTPUT_FILETYPE])
            )
        else:
            output_file = os.path.join(OUTPUT_DIRECTORY, path_relative_test)

        ensure_output_dir(output_file)
        if mips_compile(input_path, output_file):
            logger.success(path_relative_test)
        else:
            logger.error(path_relative_test)


if __name__ == "__main__":
    main()