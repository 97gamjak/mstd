"""Module defining enumerations used in C++ checks."""

from enum import Enum


class ResultTypeEnum(Enum):
    """Enumeration of result types for C++ type checks."""

    Ok = 0
    Warning = 1
    Error = 2
