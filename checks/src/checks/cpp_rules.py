"""C++ rules for mstd checks."""

from checks.files import FileType
from checks.rules import ResultType
from checks.utils import check_key_sequence_ordered


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


cpp_rules = []
cpp_rules.append(
    [
        {FileType.CPPHeader, FileType.CPPSource},
        static_constexpr_inline_rule
    ]
)
