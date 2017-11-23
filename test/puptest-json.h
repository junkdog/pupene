#pragma once

#include "json.h"
#include "helpers.h"

using std::string_literals::operator""s;
using pupene::util::validate;

template <typename T>
T test_json_pupper(const T& src, bool debug = false) {
    if (debug) std::cout << std::endl << std::endl;

    T t = src;
    const std::string raw_json = pupene::to_json(t, debug);

    if (debug) std::cout << std::endl;

    auto copy = pupene::from_json<T>(raw_json, debug);
    validate(t, copy);

    return copy;
}

