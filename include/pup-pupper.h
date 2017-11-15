#pragma once

#include "pupper.h"
#include "traits.h"

/** \file
 * All functions pass their arguments directly to puppers.
 */
namespace pupene::fns {
    /**
     * Called when a new object is encountered; an object is any type
     * which is not floating point, integer or std::string.
     *
     * Forwards to Pupper::begin(). If the Pupper returns
     * PupPolicy::consume_object, the Pupper deals with the object
     * without invoking its associated `pup` function(s).
     *
     * @param value Passed to pupper.
     * @return PupPolicy::pup_object or PupPolicy::consume_object
     */
    template <typename P, typename T>
    PupPolicy begin(Pupper<P>& p,
                    T& value,
                    const Meta& meta) {

        return p.begin_impl(value, meta);
    }

    /**
     * Forwards value to the Pupper.
     *
     * @tparam T Matches std::string, integer and floating point types.
     * @param value Passed to pupper.
     * @see enable_if_puppable
     */
    template <typename P,
              typename T,
              typename = enable_if_puppable<T>>
    void pup(Pupper<P>& p, T& value, const Meta& meta) {
        p.pup_impl(value, meta);
    }

    /**
     * The opposite of pupene::fns::begin(p, value, meta). The `meta` object is
     * the same as was sent in when the corresponding `begin(Pupper<P>, T&, Meta&)`.
     */
    template <typename P>
    void end(Pupper<P>& p, const Meta& meta) {
        p.end_impl(meta);
    }
}
