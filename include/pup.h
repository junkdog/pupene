#pragma once

#include "pup-core.h"


/** \file
 * Core `pup` functions for working with `STL` types.
 */
namespace pupene::fns {
    /**
     * Pups STL-like sequential containers, but not `std::string`.
     *
     * @see enable_if_puppable_container
     */
    template <typename P,
              typename Container, // excluding std::string
              typename = void,    // avoiding redefinition of pup
              typename = enable_if_puppable_container<Container>>
    void pup(Pupper<P>& p,
             Container& container,
             const Meta& meta) {

        pup_iterable(p, container, meta);
    }

    /**
     * Pups STL-like associative maps.
     *
     * @see enable_if_puppable_map
     */
    template <typename P,
              template <typename ...> typename Map,
              typename K,
              typename V,
              typename = enable_if_puppable_map<Map<K, V>>>
    void pup(Pupper<P>& p,
             Map<K, V>& map,
             const Meta& meta) {

        pup_object_container(p, map, meta, [&map](auto&& fpup, auto size) {
            const Meta meta = {"", Meta::Type::Object};
            if (map.empty()) { // assume deserialization
                for (auto i = 0u; size > i; i++) {
                    std::pair<K, V> kv;
                    fpup(kv, meta);
                    map[kv.first] = kv.second;
                }
            } else {           // assume serialization
                for (auto& entry : map) {
                    std::pair<K, V> kv = entry;
                    fpup(kv, meta);
                }
            }
        });
    }

    /**
     * Pups std::pair, used for pupping associative maps.
     */
    template <typename P, typename K, typename V>
    void pup(Pupper<P>& p, std::pair<K, V>& pair, const Meta& meta) {
        pup_object(p, pair, meta, [&pair](auto&& fpup) {
            fpup(pair.first, "key");
            fpup(pair.second, "value");
        });
    }
}