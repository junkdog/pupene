#pragma once

#include <gtest/gtest.h>
#include <vector>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

namespace pupene::util {
    namespace io = boost::iostreams;

    using to_vector = io::back_insert_device<std::vector<char>>;
    using from_array = io::basic_array_source<char>;

    using mem_ostream = io::stream<to_vector>;
    using mem_istream = io::stream<from_array>;

    mem_ostream byte_ostream(std::vector<char>& buffer);
    mem_istream byte_istream(std::vector<char>& source);

    template <typename T>
    void validate(const T& orig, const T& copy) {
        // gtest asserts require void return
        ASSERT_EQ(orig, copy);
        ASSERT_NE(&orig, &copy) << "objects must be distinct instances";
    }
}

