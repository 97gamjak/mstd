"""Module defining rules for mstd checks."""
from __future__ import annotations

from checks.files import FileType


class Rule:
    """Base class for defining a rule."""

    def __init__(
        self,
        name: str,
        file_types: set[FileType] | None = None,
        description: str | None = None
    ) -> None:
        """Initialize Rule with a name and optional description."""
        self.name = name
        self.description = description

        if file_types is None:
            self.file_types = FileType.all_types()

        self.file_types = file_types
