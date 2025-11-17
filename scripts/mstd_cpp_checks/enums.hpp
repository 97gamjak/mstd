"""
Module defining enumerations used in C++ checks.
"""

from enum import Enum

class FileType(Enum):
    """Enumeration of file types for C++ checks."""

    CPPHeader = 0
    CPPSource = 1
    CMakeLists = 2

class ResultTypeEnum(Enum):
    """Enumeration of result types for C++ type checks."""

    Ok = 0
    Warning = 1
    Error = 2
