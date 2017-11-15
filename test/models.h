#pragma once

#include <glm/common.hpp>
#include <string>

struct Mat4f {
    glm::vec2 a;
    glm::vec2 b;
    glm::vec2 c;
    glm::vec2 d;
};

struct Tag {
    uint32_t id = 0;
    std::string name;
};

struct DictA {
    std::map<uint32_t, std::string> int_to_string;

    bool operator==(const DictA& rhs) const {
        return int_to_string == rhs.int_to_string;
    }

    bool operator!=(const DictA& rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const DictA& rhs) const {
        return int_to_string < rhs.int_to_string;
    }

    bool operator>(const DictA& rhs) const {
        return rhs.int_to_string < int_to_string;
    }

    bool operator<=(const DictA& rhs) const {
        return !(rhs.int_to_string < int_to_string);
    }

    bool operator>=(const DictA& rhs) const {
        return !(int_to_string < rhs.int_to_string);
    }
};

template<typename K>
struct DictB {
    std::map<K, std::string> k_to_string;

    bool operator==(const DictB& rhs) const {
        return k_to_string == rhs.k_to_string;
    }

    bool operator!=(const DictB& rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const DictB& rhs) const {
        return k_to_string < rhs.k_to_string;
    }

    bool operator>(const DictB& rhs) const {
        return rhs.k_to_string < k_to_string;
    }

    bool operator<=(const DictB& rhs) const {
        return !(rhs.k_to_string < k_to_string);
    }

    bool operator>=(const DictB& rhs) const {
        return !(k_to_string < rhs.k_to_string);
    }
};

[[maybe_unused]]
static bool operator==(const Mat4f& a, const Mat4f& b) {
    return a.a == b.a
        && a.b == b.b
        && a.c == b.c
        && a.d == b.d;
}

[[maybe_unused]]
static bool operator==(const Tag& a, const Tag& b) {
    return a.name == b.name
        && a.id == b.id;
}


