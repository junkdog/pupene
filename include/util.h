#pragma once

#include "debug.h"
#include "pup.h"

namespace pupene {
    using Type = Meta::Type;
    using fns::pup;

    template <typename P, typename T>
    void do_pup(P&& pupper, T& obj, bool debug) {
        if (debug) {
            auto debugged = debug_pupper(pupper, std::cout);
            pup(debugged, obj, {"", Type::Object});
        } else {
            pup(pupper, obj, {"", Type::Object});
        }
    }
}