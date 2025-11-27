"""Unit tests for C++ style rules in mstd checks."""

from checks.cpp.style_rules import CheckKeySeqOrder, cpp_style_rules, rule01
from checks.rules import ResultTypeEnum, RuleInputType, RuleType


class TestCheckKeySeqOrder:
    """Test suite for CheckKeySeqOrder rule class."""

    def test_rule_initialization(self):
        """Test that CheckKeySeqOrder is initialized correctly."""
        rule = CheckKeySeqOrder("static inline constexpr")

        assert rule.name == "static inline constexpr"
        assert rule.rule_type == RuleType.CPP_STYLE
        assert rule.rule_input_type == RuleInputType.LINE
        assert rule.description == 'Use "static inline constexpr" only in this given order.'

    def test_rule_initialization_custom_key_sequence(self):
        """Test CheckKeySeqOrder with a different key sequence."""
        rule = CheckKeySeqOrder("const static")

        assert rule.name == "const static"
        assert rule.rule_type == RuleType.CPP_STYLE
        assert rule.rule_input_type == RuleInputType.LINE
        assert rule.description == 'Use "const static" only in this given order.'

    def test_apply_correct_order(self):
        """Test that rule passes when keys are in correct order."""
        rule = CheckKeySeqOrder("static inline constexpr")
        result = rule.apply("static inline constexpr int x = 42;")

        assert result.value == ResultTypeEnum.Ok

    def test_apply_incorrect_order(self):
        """Test that rule fails when keys are out of order."""
        rule = CheckKeySeqOrder("static inline constexpr")
        result = rule.apply("inline static constexpr int x = 42;")

        assert result.value == ResultTypeEnum.Error

    def test_apply_partial_keys_present(self):
        """Test that rule passes when only some keys are present."""
        rule = CheckKeySeqOrder("static inline constexpr")
        # Only "static" and "constexpr" are present, missing "inline"
        result = rule.apply("static constexpr int x = 42;")

        assert result.value == ResultTypeEnum.Ok

    def test_apply_no_keys_present(self):
        """Test that rule passes when no keys are present."""
        rule = CheckKeySeqOrder("static inline constexpr")
        result = rule.apply("int x = 42;")

        assert result.value == ResultTypeEnum.Ok

    def test_apply_single_key_present(self):
        """Test that rule passes when only a single key is present."""
        rule = CheckKeySeqOrder("static inline constexpr")
        result = rule.apply("static int x = 42;")

        assert result.value == ResultTypeEnum.Ok


class TestStaticInlineConstexprRule:
    """Test suite for the 'static inline constexpr' rule (rule01)."""

    def test_rule01_exists_and_configured(self):
        """Test that rule01 is properly configured."""
        assert rule01 is not None
        assert rule01.name == "static inline constexpr"
        assert rule01.rule_type == RuleType.CPP_STYLE
        assert rule01.rule_input_type == RuleInputType.LINE

    def test_rule01_in_cpp_style_rules(self):
        """Test that rule01 is in the cpp_style_rules list."""
        assert rule01 in cpp_style_rules
        assert len(cpp_style_rules) >= 1

    def test_static_inline_constexpr_correct_order(self):
        """Test static inline constexpr in correct order."""
        result = rule01.apply("static inline constexpr int value = 10;")
        assert result.value == ResultTypeEnum.Ok

    def test_static_inline_constexpr_wrong_order_inline_first(self):
        """Test inline static constexpr (wrong order)."""
        result = rule01.apply("inline static constexpr int value = 10;")
        assert result.value == ResultTypeEnum.Error

    def test_static_inline_constexpr_wrong_order_constexpr_first(self):
        """Test constexpr static inline (wrong order)."""
        result = rule01.apply("constexpr static inline int value = 10;")
        assert result.value == ResultTypeEnum.Error

    def test_static_inline_constexpr_wrong_order_constexpr_inline_static(self):
        """Test constexpr inline static (wrong order)."""
        result = rule01.apply("constexpr inline static int value = 10;")
        assert result.value == ResultTypeEnum.Error

    def test_static_constexpr_only(self):
        """Test static constexpr without inline (should pass - not all keys present)."""
        result = rule01.apply("static constexpr int value = 10;")
        assert result.value == ResultTypeEnum.Ok

    def test_inline_constexpr_only(self):
        """Test inline constexpr without static (should pass - not all keys present)."""
        result = rule01.apply("inline constexpr int value = 10;")
        assert result.value == ResultTypeEnum.Ok

    def test_no_keywords(self):
        """Test line with no relevant keywords."""
        result = rule01.apply("int value = 10;")
        assert result.value == ResultTypeEnum.Ok

    def test_with_template(self):
        """Test static inline constexpr in a template context."""
        result = rule01.apply("template<typename T> static inline constexpr T default_value{};")
        assert result.value == ResultTypeEnum.Ok

    def test_in_function_declaration(self):
        """Test static inline constexpr in a function declaration."""
        result = rule01.apply("static inline constexpr auto compute() -> int { return 42; }")
        assert result.value == ResultTypeEnum.Ok

    def test_wrong_order_in_function(self):
        """Test wrong order in function declaration."""
        result = rule01.apply("inline static constexpr auto compute() -> int { return 42; }")
        assert result.value == ResultTypeEnum.Error
