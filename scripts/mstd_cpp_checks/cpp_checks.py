/*****************************************************************************
<GPL_HEADER>

    mstd library
    Copyright (C) 2025-now  Jakob Gamper

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

<GPL_HEADER>
******************************************************************************/

"""Module defining C++ check rules."""

import logging
import sys

from cpp_rules import cpp_rules
from enums import FileType
from files import __BASE_DIR__, get_files_in_dirs, get_staged_files
from rules import check_file

__CPP_DIRS__ = ["include", "test"]
__OTHER_DIRS__ = ["scripts"]
__EXCLUDE_DIRS__ = ["__pycache__", ".ruff_cache"]
__EXCLUDE_FILES__ = [".gitignore"]

__DIRS__ = __CPP_DIRS__ + __OTHER_DIRS__

cpp_check_logger = logging.getLogger("mstd_cpp_checks")


def run_checks(rules: list[set[FileType], callable]) -> None:

    if "full" in sys.argv:
        cpp_check_logger.info("Running full checks...")
        dirs = [__BASE_DIR__ / dir_name for dir_name in __DIRS__]
        files = get_files_in_dirs(dirs, __EXCLUDE_DIRS__, __EXCLUDE_FILES__)
    else:
        cpp_check_logger.info("Running checks on staged files...")
        files = get_staged_files()

    for filename in files:
        check_file(filename, rules)


if __name__ == "__main__":
    run_checks(cpp_rules)
