#include "helpers.h"

namespace pupene::util {
    mem_ostream byte_ostream(std::vector<char>& buffer) {
        auto vector_out = io::back_inserter(buffer);
        return mem_ostream(vector_out);
    }

    mem_istream byte_istream(std::vector<char>& source) {
        auto array_in = io::array_source(source.data(), source.size());
        return mem_istream(array_in);
    }
}