#pragma once

#include "debug-pupper.h"
#include "pup.h"
#include <string>
#include <sstream>

/** \file
 * Introspection utilities.
 *
 * @see DebugPupper
 */
namespace pupene {
    /**
     * Wraps existing `pupper` in a DebugPupper. Adheres to the
     * [PupPolicy] of wrapped pupper.
     */
    template <typename Pupper>
    DebugPupper<Pupper> debug_pupper(Pupper& pupper,
                                     std::ostream& out) {

        return DebugPupper<Pupper>{pupper, out};
    }

    /**
     * DebugPupper wrapping a NullPupper.
     */
    DebugPupper<NullPupper> debug_pupper(std::ostream& out = std::cout);

    /** Returns a string representation of `object`. */
    template <typename T>
    std::string to_debug(T& object) {
        std::ostringstream out;

        auto pupper = debug_pupper(out);
        fns::pup(pupper, object, {});
        out.flush();

        return out.str();
    }

    /** Returns a string representation of `object`. */
    template <typename T>
    std::string to_debug(const T& object) {
        return to_debug(const_cast<T&>(object));
    }
}