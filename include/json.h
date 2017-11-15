#pragma once

#include "json-puppers.h"
#include "util.h"
#include <sstream>

/** \file
 * Convert to/from JSON.
 */
namespace pupene {
    template<typename T>
    T from_json(std::istream& in, bool debug = false) {
        T t;
        do_pup(JsonReader{in}, t, debug);
        return t;
    }

    template<typename T>
    T from_json(const std::string& s, bool debug = false) {
        std::istringstream in{s};
        return from_json<T>(in, debug);
    }

    template<typename T>
    void to_json(T& obj, std::ostream& out, bool debug = false) {
        JsonWriter pupper{out};
        do_pup(pupper, obj, debug);
    }

    template<typename T>
    std::string to_json(T& obj, bool debug = false) {
        std::ostringstream out;
        to_json(obj, out, debug);
        return out.str();
    }
}