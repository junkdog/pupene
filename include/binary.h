#pragma once

#include "binary-puppers.h"
#include "util.h"

/** \file
 * Utility functions for serializing to/from binary.
 */
namespace pupene {
    template <typename T>
    T from_binary(std::istream& in, bool debug = false) {
        T t;
        do_pup(BinaryReader{in}, t, debug);
        return t;
    }

    template <typename T>
    void to_binary(T& obj, std::ostream& out, bool debug = false) {
        do_pup(BinaryWriter{out}, obj, debug);
    }
}