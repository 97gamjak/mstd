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

"""Module defining C++ type checking utilities."""

from __future__ import annotations

from enums import ResultTypeEnum


class ResultType:
    """Class representing the result of a C++ type check."""

    def __init__(
        self,
        value: ResultTypeEnum,
        description: str | None = None
    ) -> None:
        """Initialize ResultType with a value and optional description."""
        self.value = value
        self.description = description


def check_key_sequence_ordered(key_sequence: list[str], line: str) -> ResultType:
    """Check if keys in key_sequence appear in order on the given line.

    Parameters
    ----------
    key_sequence: list[str]
        The sequence of keys to check for order.
    line: str
        The line of text to check.
    filename: str
        The name of the file containing the line.
    linenumber: int
        The line number in the file.

    Returns
    -------
    ResultType:
        Result of the check, Warning if keys are out of order, Ok otherwise

    """
    if not isinstance(key_sequence, list):
        msg = f"key_sequence {key_sequence} must be a list"
        raise TypeError(msg)

    line_elements = line.split()
    indices = [
        line_elements.index(key)
        for key in key_sequence
        if key in line_elements
    ]

    if len(indices) == len(key_sequence) and sorted(indices) != indices:
        return ResultType(
            ResultTypeEnum.Warning,
            f"key_sequence {key_sequence} not ordered correctly "
            f"in line {line}."
        )

    return ResultType(ResultTypeEnum.Ok)
