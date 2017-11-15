#include <string>

#include <gtest/gtest.h>
#include "pupperdefs.h"
#include "puptest-json.h"
#include "testdata.h"

using Type = ::pupene::Meta::Type;

TEST(json_pupper_test, simple) {
    test_json_pupper(VEC2);
}

TEST(json_pupper_test, compounded) {
    test_json_pupper(MAT4F);
}

TEST(json_pupper_test, array) {
    test_json_pupper(INTS);
}

TEST(json_pupper_test, pairs) {
    test_json_pupper(PAIR);
    test_json_pupper(PAIRS);
}

TEST(json_pupper_test, with_string_field) {
    test_json_pupper(TAG);
    test_json_pupper(TAG_SHORT);
}

TEST(json_pupper_test, dict_plain) {
    test_json_pupper(DICT_PLAIN);
}

TEST(json_pupper_test, dict_template) {
    test_json_pupper(DICT_K);
}

