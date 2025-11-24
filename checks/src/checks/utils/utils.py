"""Module defining utility functions for mstd checks."""

from __future__ import annotations

from checks.rules import ResultType, ResultTypeEnum


def check_key_sequence_ordered(
    key_sequence: str,
    line: str,
    key_delimiter: str = " "
) -> ResultType:
    """Check if keys in key_sequence appear in order on the given line.

    Parameters
    ----------
    key_sequence: str
        The sequence of keys to check for order as a string.
    line: str
        The line of text to check.
    key_delimiter: str
        The delimiter used to find the keys

    Returns
    -------
    ResultType:
        Result of the check, Warning if keys are out of order, Ok otherwise

    """
    key_sequence = key_sequence.split(key_delimiter)

    line_elements = line.split(key_delimiter)
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
