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

"""Module defining how to handle rules for C++ checks."""

import logging
from pathlib import Path

from enums import FileType, ResultTypeEnum
from files import determine_file_type

rule_logger = logging.getLogger("mstd_cpp_checks.rules")


def check_file(filename: str, rules: list[set[FileType], callable]) -> None:
    """Perform checks based on the file type.

    Parameters
    ----------
    filename: str
        The name of the file to check.
    rules: list[callable]
        List of rule functions to apply.

    Returns
    -------
    None

    """
    file_type = determine_file_type(filename)

    with Path(filename).open("r", encoding="utf-8") as f:
        lines = f.readlines()

    for linenumber, line in enumerate(lines):
        for file_types, rule in rules:
            if file_types and file_type not in file_types:
                continue

            result = rule(line)
            if result.value == ResultTypeEnum.Warning:
                rule_logger.warning(
                    "Warning in %s:%s: %s", filename, linenumber, result.description
                )
            elif result.value == ResultTypeEnum.Error:
                rule_logger.error(
                    "Error in %s:%s: %s", filename, linenumber, result.description
                )
