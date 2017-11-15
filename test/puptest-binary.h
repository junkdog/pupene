#pragma once

#include <gtest/gtest.h>
#include "pupene.h"
#include "helpers.h"

using pupene::util::validate;

template <typename T>
std::vector<char> to_buffer(T src, bool debug);

template <typename T>
T from_buffer(std::vector<char>& buffer, bool debug);

template <typename T>
void test_binary_pupper(const T& src, bool debug = false) {
    std::vector<char> buffer = to_buffer(src, debug);
    ASSERT_NE(0, buffer.size()) << "nothing read from stream";
    const T copy = from_buffer<T>(buffer, debug);
    validate(src, copy);
}


template <typename T>
std::vector<char> to_buffer(T src, bool debug) {
    std::vector<char> buffer {};

    auto out = pupene::util::byte_ostream(buffer);
    pupene::to_binary(src, out, debug);

    return buffer;
}

template <typename T>
T from_buffer(std::vector<char>& buffer, bool debug) {
    auto in = pupene::util::byte_istream(buffer);
    return pupene::from_binary<T>(in, debug);
}
