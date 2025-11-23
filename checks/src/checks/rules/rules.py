"""Module defining rules for mstd checks."""
from __future__ import annotations

import typing

from checks.enums import StrEnum
from checks.files import FileType

if typing.TYPE_CHECKING:
    from .result_type import ResultType


class RuleType(StrEnum):
    """Enumeration of rule types for mstd checks."""

    GENERAL = "GENERAL"
    CPP_STYLE = "CPP_STYLE"


class Rule:
    """Base class for defining a rule."""

    def __init__(
        self,
        name: str,
        func: callable,
        rule_type: RuleType = RuleType.GENERAL,
        file_types: set[FileType] | None = None,
        description: str | None = None
    ) -> None:
        """Initialize Rule with a name and optional description."""
        self.name = name
        self.description = description
        self.func = func
        self.rule_type = rule_type

        if file_types is None:
            self.file_types = FileType.all_types()

        self.file_types = file_types

    def apply(self, kwargs: dict[str, typing.Any]) -> ResultType:
        """Apply the rule on a specific line."""
        return self.func(**kwargs)
