"""Module defining C++ check rules."""

from enums import FileType

from cpp_checks import ResultType, check_key_sequence_ordered


def static_constexpr_inline_rule(line: str) -> ResultType:
    """Check for usage of static, constexpr, and inline in variable declarations.

    Parameters
    ----------
    line: str
        The line of code to check.

    Returns
    -------
    cpp_types.ResultType:
        Result of the check, Warning if all keywords are not present, Ok otherwise

    """
    key_sequence = ["static", "inline", "constexpr"]
    return check_key_sequence_ordered(key_sequence, line)


rules = []
rules.append(
    [
        {FileType.CPPHeader, FileType.CPPSource},
        static_constexpr_inline_rule
    ]
)
