#include "specialized-pupper.h"

using pupene::PupPolicy;
using pupene::Meta;

namespace pupene {
    template<>
    PupPolicy SpecPupper::begin(glm::vec2& value, const Meta&) {
        value = pupene::EXPECTED_BEGIN_VEC2;
        return PupPolicy::consume_object;
    }
}
std::ostream& ::pupene::operator<<(std::ostream& stream, const glm::vec2& v) {
    return stream << "vec2(x=" << v.x << ". y=" << v.y << ")";
}
