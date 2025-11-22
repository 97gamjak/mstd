"""Module defining enumerations used in C++ checks."""

from enum import Enum


class FileType(Enum):
    """Enumeration of file types for C++ checks."""

    UNKNOWN = 0
    CPPHeader = 1
    CPPSource = 2
    CMakeLists = 3


class ResultTypeEnum(Enum):
    """Enumeration of result types for C++ type checks."""

    Ok = 0
    Warning = 1
    Error = 2
