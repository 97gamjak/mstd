"""Module defining C++ check rules."""

import sys

from checks.cpp_rules import cpp_rules
from checks.files import (
    __EXECUTION_DIR__,
    FileType,
    get_files_in_dirs,
    get_staged_files,
)
from checks.logger import cpp_check_logger

__CPP_DIRS__ = ["include", "test"]
__OTHER_DIRS__ = ["scripts"]
__EXCLUDE_DIRS__ = ["__pycache__", ".ruff_cache"]
__EXCLUDE_FILES__ = [".gitignore"]

__DIRS__ = __CPP_DIRS__ + __OTHER_DIRS__


def run_checks(rules: list[set[FileType], callable]) -> None:
    """Run C++ checks based on the provided rules."""
    if "full" in sys.argv:
        cpp_check_logger.info("Running full checks...")
        cpp_check_logger.debug(f"Checking directories: {__DIRS__}")
        dirs = [__EXECUTION_DIR__ / dir_name for dir_name in __DIRS__]
        files = get_files_in_dirs(dirs, __EXCLUDE_DIRS__, __EXCLUDE_FILES__)
    else:
        cpp_check_logger.info("Running checks on staged files...")
        files = get_staged_files()

    if not files:
        cpp_check_logger.warning("No files to check.")
        return

    for filename in files:
        cpp_check_logger.info(
            f"Checking file: {filename}, rules: {len(rules)}")


def main() -> None:
    """Run C++ checks."""
    run_checks(cpp_rules)
