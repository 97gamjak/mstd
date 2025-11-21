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

"""Module for file operations in C++ checks."""

import subprocess
from pathlib import Path

from enums import FileType

__BASE_DIR__ = Path(__file__).parent.parent.parent.resolve()


def determine_file_type(filename: str | Path) -> FileType:
    """Determine the file type based on the filename extension.

    Parameters
    ----------
    filename: str | Path
        The name of the file to check.

    Returns
    -------
    FileType:
        The determined file type.

    """
    filename = str(filename)

    if filename.endswith((".h", ".hpp")):
        return FileType.CPPHeader
    if filename.endswith((".cpp", ".cxx", ".cc", ".c")):
        return FileType.CPPSource
    if filename == "CMakeLists.txt":
        return FileType.CMakeLists

    return FileType.UNKNOWN


def get_files_in_dirs(
    paths: list[Path],
    exclude_dirs: list[str] | None = None,
    exclude_files: list[str] | None = None
) -> list[Path]:
    """Get all files in the specified directories.

    Parameters
    ----------
    paths: list[Path]
        List of directory paths to search for files.
    exclude_dirs: list[str] | None
        List of directory names to exclude from the search. Defaults to None.
    exclude_files: list[str] | None
        List of file names to exclude from the search. Defaults to None.

    Returns
    -------
    list[Path]:
        List of file paths found in the specified directories.

    """
    if exclude_dirs is None:
        exclude_dirs = []

    if exclude_files is None:
        exclude_files = []

    all_files = []
    for path in paths:
        if path.is_dir() and path.name not in exclude_dirs:
            all_files.extend(
                get_files_in_dirs(path.iterdir(), exclude_dirs, exclude_files)
            )
        elif path.is_file() and path.name not in exclude_files:
            all_files.append(path)

    return all_files


def get_staged_files() -> list[str]:
    """Get the list of staged files in the git repository.

    Returns
    -------
    list[str]:
        List of staged file paths.

    """
    result = subprocess.run(
        ["git", "diff", "--name-only", "--cached"],
        capture_output=True,
        text=True,
        check=True
    )

    if result.returncode != 0:
        msg = "Failed to get staged files from git."
        raise RuntimeError(msg)

    files = result.stdout.strip().split("\n")
    return [file for file in files if file]
