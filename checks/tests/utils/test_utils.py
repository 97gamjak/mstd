from checks.utils import check_key_sequence_ordered
from checks.rules import ResultTypeEnum


def test_check_key_sequence_ordered():
    keys = "static inline constexpr"
    line = "static inline constexpr int x = 42;"
    result = check_key_sequence_ordered(keys, line)
    assert result.value == ResultTypeEnum.Ok

    line = "inline static constexpr int x = 42;"
    result = check_key_sequence_ordered(keys, line)
    assert result.value == ResultTypeEnum.Error
