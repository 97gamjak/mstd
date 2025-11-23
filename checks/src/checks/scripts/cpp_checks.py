"""Module defining C++ check rules."""

import sys

from checks.cpp_rules import cpp_rules
from checks.enums import FileType
from checks.files import __EXECUTION_DIR__, get_files_in_dirs, get_staged_files
from checks.logger import cpp_check_logger
from checks.rules import check_file

__CPP_DIRS__ = ["include", "test"]
__OTHER_DIRS__ = ["scripts"]
__EXCLUDE_DIRS__ = ["__pycache__", ".ruff_cache"]
__EXCLUDE_FILES__ = [".gitignore"]

__DIRS__ = __CPP_DIRS__ + __OTHER_DIRS__


def run_checks(rules: list[set[FileType], callable]) -> None:
    """Run C++ checks based on the provided rules."""
    if "full" in sys.argv:
        cpp_check_logger.info("Running full checks...")
        dirs = [__EXECUTION_DIR__ / dir_name for dir_name in __DIRS__]
        files = get_files_in_dirs(dirs, __EXCLUDE_DIRS__, __EXCLUDE_FILES__)
    else:
        cpp_check_logger.info("Running checks on staged files...")
        files = get_staged_files()

    for filename in files:
        check_file(filename, rules)


def main() -> None:
    """Run C++ checks."""
    run_checks(cpp_rules)
