#include <gtest/gtest.h>
#include <binary-puppers.h>
#include "traits.h"
#include "specialized-pupper.h"

using std::string_literals::operator""s;
using Type = pupene::Meta::Type;

using pupene::is_pup_impl;
using pupene::SpecPupper;

template <typename Pupper, typename T>
constexpr void assert_pupper_pups_type() {
    static_assert(
        is_pup_impl<Pupper, T>(),
        "missing pup_impl in pupper");
}

template <typename Pupper,
          typename T>
constexpr void assert_pupper_pups() {
    assert_pupper_pups_type<Pupper, T>();
};

template <typename Pupper,
          typename T,
          typename Next,
          typename... Tail>
constexpr void assert_pupper_pups() {
    assert_pupper_pups<Pupper, T>();
    assert_pupper_pups<Pupper, Next, Tail...>();
}

TEST(specialized_pupper_test, is_pup_impl_test) {
    assert_pupper_pups<SpecPupper, float,
            bool,
            short,
            uint64_t,
            std::string,
//            glm::vec2,
            double
    >();


    assert_pupper_pups<pupene::BinaryWriter, float,
            bool,
            short,
            uint64_t,
            std::string,
//            glm::vec2,
            double
    >();
}