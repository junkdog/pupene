#pragma once

#include <glm/common.hpp>
#include "pup.h"
#include "models.h"

namespace pupene::fns {
    template<typename P>
    void pup(Pupper<P>& p, Mat4f& v, const Meta& meta) {
        pup_object(p, v, meta, [&v](auto&& fpup) {
            fpup(v.a, "a"); // dispatches to pup for vec2
            fpup(v.b, "b");
            fpup(v.c, "c");
            fpup(v.d, "d");
        });
    }

    template<typename P>
    void pup(Pupper<P>& p, glm::vec2& v, const Meta& meta) {
        pup_object(p, v, meta, [&v](auto&& fpup) {
            fpup(v.x, "x");
            fpup(v.y, "y");
        });
    }

    template<typename P>
    void pup(Pupper<P>& p, Tag& v, const Meta& meta) {
        pup_object(p, v, meta, [&v](auto&& fpup) {
            fpup(v.id, "id");
            fpup(v.name, "name");
        });
    }

    template<typename P>
    void pup(Pupper<P>& p, DictA& v, const Meta& meta) {
        pup_object(p, v, meta, [&v](auto&& fpup) {
            fpup(v.int_to_string, {"int_to_string"});
        });
    }

    template<typename P, typename K>
    void pup(Pupper<P>& p, DictB<K>& v, const Meta& meta) {
        pup_object(p, v, meta, [&v](auto&& fpup) {
            fpup(v.k_to_string, {"k_to_string"});
        });
    }
}