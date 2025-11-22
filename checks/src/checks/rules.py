"""Module defining how to handle rules for C++ checks."""

import logging
from pathlib import Path

from checks.files import FileType, determine_file_type
from checks.results import ResultTypeEnum

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
