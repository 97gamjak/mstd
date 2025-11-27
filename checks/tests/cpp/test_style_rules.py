"""Unit tests for C++ style rules."""

from checks.cpp.style_rules import CheckKeySeqOrder, cpp_style_rules, rule01
from checks.files import FileType
from checks.rules import ResultTypeEnum, Rule, RuleInputType, RuleType


class TestCheckKeySeqOrder:
    """Tests for CheckKeySeqOrder rule."""

    def setup_method(self):
        """Reset rule counters before each test."""
        Rule.cpp_style_rule_counter = 0
        Rule.general_rule_counter = 0

    def test_check_key_seq_order_creation(self):
        """Test CheckKeySeqOrder rule creation."""
        rule = CheckKeySeqOrder("static inline constexpr")
        assert rule.name == "static inline constexpr"
        assert rule.rule_type == RuleType.CPP_STYLE
        assert rule.rule_input_type == RuleInputType.LINE
        assert rule.description == 'Use "static inline constexpr" only in this given order.'
        assert rule.file_types == FileType.cpp_types()

    def test_check_key_seq_order_correct_order(self):
        """Test CheckKeySeqOrder returns Ok for correct order."""
        rule = CheckKeySeqOrder("static inline constexpr")
        line = "static inline constexpr int x = 42;"
        result = rule.apply(line)
        assert result.value == ResultTypeEnum.Ok

    def test_check_key_seq_order_incorrect_order(self):
        """Test CheckKeySeqOrder returns Error for incorrect order."""
        rule = CheckKeySeqOrder("static inline constexpr")
        line = "inline static constexpr int x = 42;"
        result = rule.apply(line)
        assert result.value == ResultTypeEnum.Error

    def test_check_key_seq_order_missing_keys(self):
        """Test CheckKeySeqOrder returns Ok when keys are missing."""
        rule = CheckKeySeqOrder("static inline constexpr")
        line = "int x = 42;"
        result = rule.apply(line)
        assert result.value == ResultTypeEnum.Ok

    def test_check_key_seq_order_partial_keys(self):
        """Test CheckKeySeqOrder returns Ok when only some keys are present."""
        rule = CheckKeySeqOrder("static inline constexpr")
        line = "static int x = 42;"
        result = rule.apply(line)
        assert result.value == ResultTypeEnum.Ok

    def test_check_key_seq_order_different_sequence(self):
        """Test CheckKeySeqOrder with different key sequence."""
        rule = CheckKeySeqOrder("const int")
        line = "const int x = 42;"
        result = rule.apply(line)
        assert result.value == ResultTypeEnum.Ok

        line = "int const x = 42;"
        result = rule.apply(line)
        assert result.value == ResultTypeEnum.Error


class TestCppStyleRulesModule:
    """Tests for cpp_style_rules module."""

    def test_rule01_is_check_key_seq_order(self):
        """Test that rule01 is a CheckKeySeqOrder instance."""
        assert isinstance(rule01, CheckKeySeqOrder)

    def test_rule01_checks_static_inline_constexpr(self):
        """Test that rule01 checks for static inline constexpr ordering."""
        assert rule01.name == "static inline constexpr"

    def test_cpp_style_rules_list_not_empty(self):
        """Test that cpp_style_rules list is not empty."""
        assert len(cpp_style_rules) > 0

    def test_cpp_style_rules_contains_rule01(self):
        """Test that cpp_style_rules contains rule01."""
        assert rule01 in cpp_style_rules

    def test_all_cpp_style_rules_are_cpp_style_type(self):
        """Test that all rules in cpp_style_rules have CPP_STYLE type."""
        for rule in cpp_style_rules:
            assert rule.rule_type == RuleType.CPP_STYLE


class TestStaticInlineConstexprRule:
    """Tests for static inline constexpr ordering rule."""

    def setup_method(self):
        """Reset rule counters before each test."""
        Rule.cpp_style_rule_counter = 0
        Rule.general_rule_counter = 0

    def test_static_inline_constexpr_correct(self):
        """Test correct static inline constexpr order."""
        rule = CheckKeySeqOrder("static inline constexpr")
        test_cases = [
            "static inline constexpr int x = 42;",
            "    static inline constexpr auto value = 10;",
            "static inline constexpr double PI = 3.14159;",
        ]
        for line in test_cases:
            result = rule.apply(line)
            assert result.value == ResultTypeEnum.Ok, f"Failed for: {line}"

    def test_static_inline_constexpr_wrong_order(self):
        """Test wrong static inline constexpr order returns Error."""
        rule = CheckKeySeqOrder("static inline constexpr")
        test_cases = [
            "inline static constexpr int x = 42;",
            "constexpr static inline int x = 42;",
            "constexpr inline static int x = 42;",
            "inline constexpr static int x = 42;",
        ]
        for line in test_cases:
            result = rule.apply(line)
            assert result.value == ResultTypeEnum.Error, f"Expected Error for: {line}"

    def test_static_inline_constexpr_partial_present(self):
        """Test lines with only some keywords present."""
        rule = CheckKeySeqOrder("static inline constexpr")
        test_cases = [
            "static int x = 42;",
            "inline void func();",
            "constexpr int y = 10;",
            "static constexpr int z = 5;",
            "inline constexpr int w = 3;",
        ]
        for line in test_cases:
            result = rule.apply(line)
            assert result.value == ResultTypeEnum.Ok, f"Expected Ok for: {line}"
