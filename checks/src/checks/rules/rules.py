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

    @classmethod
    def cpp_rules(cls) -> set[RuleType]:
        return set(RuleType.CPP_STYLE)


class RuleInputType(StrEnum):
    """Enumeration of rule input types for mstd checks."""

    NONE = "NONE"
    LINE = "LINE"
    FILE = "FILE"


class Rule:
    """Base class for defining a rule."""

    def __init__(
        self,
        name: str,
        func: callable,
        rule_type: RuleType = RuleType.GENERAL,
        rule_input_type: RuleInputType = RuleInputType.NONE,
        file_types: set[FileType] | None = None,
        description: str | None = None
    ) -> None:
        """Initialize Rule with a name and optional description."""
        self.name = name
        self.description = description
        self.func = func
        self.rule_type = rule_type
        self.rule_input_type = rule_input_type

        if file_types is None and rule_type == RuleType.CPP_STYLE:
            # use only cpp file types if user gives explicit CPP_STYLE rule
            self.file_types = FileType.cpp_types()
        elif file_types is None:
            self.file_types = FileType.all_types()

        self.file_types = file_types

    def apply(self, kwargs: dict[str, typing.Any]) -> ResultType:
        """Apply the rule on a specific line."""
        return self.func(**kwargs)


def filter_cpp_rules(rules: list[Rule]) -> list[Rule]:
    return [rule for rule in rules if rule.rule_type in RuleType.cpp_rules()]


def filter_line_rules(rules: list[Rule]) -> list[Rule]:
    return [rule for rule in rules if rule.rule_input_type == RuleInputType.LINE]
