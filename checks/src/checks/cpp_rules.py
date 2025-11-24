"""C++ rules for mstd checks."""

from checks.rules import Rule, RuleInputType, RuleType
from checks.utils import check_key_sequence_ordered


class CheckKeySeqOrder(Rule):
    def __init__(self, key_sequence: str) -> None:
        super().__init__(
            name=key_sequence,
            func=lambda line: check_key_sequence_ordered(
                key_sequence,
                line
            ),
            rule_type=RuleType.CPP_STYLE,
            rule_input_type=RuleInputType.LINE,
            description=f'Use "{key_sequence}" only in this given order.'
        )


rule01 = CheckKeySeqOrder("static inline constexpr")

cpp_rules = []
cpp_rules.append(
    rule01
)
