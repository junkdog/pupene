#pragma once

#include "pupper.h"

/** \file
 * NopPupper was also a tempting name.
 */
namespace pupene {
    /**
     * Does nothing by itself. but can be used together with
     * DebugPupper when inspecting existing objects.
     *
     * @see to_debug
     */
    class NullPupper : public Pupper<NullPupper> {
    public:
        NullPupper() = default;
        ~NullPupper() override = default;

        template<typename T>
        PupPolicy begin(T& /*value*/, const Meta& /*meta*/) {
            return PupPolicy::pup_object;
        }

        void end(const Meta& /*meta*/) {}

        template<typename T>
        void pup(T& /*value*/, const Meta& /*meta*/) {}
    };

}