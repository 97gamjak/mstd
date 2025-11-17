"""Module for file operations in C++ checks."""

from pathlib import Path

from enums import FileType


def determine_file_type(filename: str) -> FileType:
    """Determine the file type based on the filename extension.

    Parameters
    ----------
    filename: str
        The name of the file to check.

    Returns
    -------
    FileType:
        The determined file type.

    """
    if filename.endswith((".h", ".hpp")):
        return FileType.CPPHeader
    if filename.endswith((".cpp", ".cxx", ".cc", ".c")):
        return FileType.CPPSource
    if filename == "CMakeLists.txt":
        return FileType.CMakeLists

    msg = f"Unknown file type for filename: {filename}"
    raise ValueError(msg)


def get_files_in_dirs(dirs: list[str]) -> list[str]:
    """Get all files in the specified directories.

    Parameters
    ----------
    dirs: list[str]
        List of directory paths to search for files.

    Returns
    -------
    list[str]:
        List of file paths found in the specified directories.

    """
    all_files = []

    for dir_path in dirs:
        path_obj = Path(dir_path)
        if not path_obj.is_dir():
            msg = f"Provided path {dir_path} is not a directory."
            raise NotADirectoryError(msg)

        for file_path in path_obj.rglob("*"):
            if file_path.is_file():
                all_files.extend([str(file_path)])

    return all_files
