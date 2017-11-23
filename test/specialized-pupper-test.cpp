#include <gtest/gtest.h>
#include "pupperdefs.h"
#include "specialized-pupper.h"
#include "util.h"

using pupene::EXPECTED_BEGIN_VEC2;
using std::string_literals::operator""s;
using Type = ::pupene::Meta::Type;

template <typename T>
T spec_pup(T&& t) {
    pupene::do_pup(pupene::SpecPupper{}, t, false);
    return t;
}

TEST(specialized_pupper_test, ad_hoc_specialization) {
    auto actual = spec_pup(glm::vec2{2.0f, 4.0f});
    ASSERT_EQ(actual, EXPECTED_BEGIN_VEC2);
}