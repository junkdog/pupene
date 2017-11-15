#include <string>
#include <gtest/gtest.h>
#include "pupperdefs.h"
#include "puptest-binary.h"
#include "testdata.h"

using Type = ::pupene::Meta::Type;

TEST(binary_pupper_test, string) {
    std::string s = "hello";
    test_binary_pupper(s);
}

TEST(binary_pupper_test, simple) {
    test_binary_pupper(VEC2);
}

TEST(binary_pupper_test, compounded) {
    test_binary_pupper(MAT4F);
}

TEST(binary_pupper_test, array) {
    test_binary_pupper(INTS);
}

TEST(binary_pupper_test, pairs) {
    test_binary_pupper(PAIR);
    test_binary_pupper(PAIRS);
}

TEST(binary_pupper_test, with_string_field) {
    test_binary_pupper(TAG);
    test_binary_pupper(TAG_SHORT);
}

TEST(binary_pupper_test, dict_plain) {
    test_binary_pupper(DICT_PLAIN);
}

TEST(binary_pupper_test, dict_template) {
    test_binary_pupper(DICT_K);
}

