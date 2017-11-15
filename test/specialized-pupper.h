#pragma once

#include <iostream>
#include <glm/common.hpp>
#include "pupper.h"

namespace pupene {
    const glm::vec2 EXPECTED_BEGIN_VEC2 = {10.f, 10.f};

    std::ostream& operator<<(std::ostream& stream, const glm::vec2& v);

    class SpecPupper : public Pupper<SpecPupper> {
    public:
        explicit SpecPupper() {}
        ~SpecPupper() override = default;

        template <typename T>
        PupPolicy begin(T& /*value*/, const Meta& /*meta*/) {
            return PupPolicy::pup_object;
        }


        void end(const Meta& /*meta*/) {}

        template <typename T,
                  typename = enable_if_puppable<T>>
        void pup(T& /*value*/, const Meta& /*meta*/) {}
    };

    template<>
    PupPolicy SpecPupper::begin(glm::vec2& value, const Meta&);
}
