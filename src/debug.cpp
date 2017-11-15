#include <ostream>
#include "debug.h"

namespace pupene {
    DebugPupper<NullPupper> debug_pupper(std::ostream& out) {
        NullPupper p;
        return DebugPupper<NullPupper>{p, out};
    }
}