#pragma once

#include "pup-pupper.h"

/** \file
 * Helpers for writing `pup` functions. pup_object() is the default go-to
 * for pup() implementations.
 */
namespace pupene::fns {

    /**
     * Describes a type by enumerating its members, used in conjunction
     * with pup() implementations.
     *
     *
     * #### Example: a `pup` for [glm][glm]'s vec2 type
     *
     * ```{.cpp}
     * template<typename P>
     * void pup(Pupper<P>& p, glm::vec2& v, const Meta& meta) {
     *     pup_object(p, v, meta, [&v](auto&& fpup) {
     *         fpup(v.x, "x");
     *         fpup(v.y, "y");
     *     });
     * }
     * ```
     *
     * `body_fn` is not invoked if begin() returns PupPolicy::consume_object.
     *
     * @tparam Fn has signature `[](auto&&)`
     * @param body_fn invoked with `[](auto& v, const Meta& meta)`,
     *                where `v` is a member of `object`
     *
     * [glm]: https://glm.g-truc.net/
     */
    template <typename P,
              typename T,
              typename Fn>
    void pup_object(Pupper<P>& p,
                    T& object,
                    const Meta& meta,
                    Fn&& body_fn) {

        const auto valid_meta = (Meta::Type::Value != meta.type)
            ? meta
            : Meta{meta.name, Meta::Type::Object};

        auto policy = begin(p, object, valid_meta);
        if (policy == PupPolicy::pup_object) {
            body_fn([&p](auto& v, const Meta& meta) {
                pup(p, v, meta);
            });
        }
        end(p, valid_meta);
    }

    /**
     *  A pup_object() for dealing with iterable types. Encodes
     * `container` as `size` followed by the contents.
     *
     * @tparam BodyFn has signature `[](auto&&, uint32_t size)`
     * @param body_fn invoked with `[](auto& v, const Meta& meta)`,
     *                where `v` is a member of `object`
     */
    template <typename P,
              typename Iterable,
              typename BodyFn>
    void pup_object_container(Pupper<P>& p,
                              Iterable container,
                              const Meta& meta,
                              BodyFn&& body_fn) {

        auto body = [&p, &vs = container, &body_fn](auto&& fpup) {
            auto size = static_cast<uint32_t>(vs.size());
            fpup(size, "size");

            const Meta data = {"data", Meta::Type::Array};
            pup_object(p, vs, data, [&body_fn, size](auto&& fpup_data) {
                body_fn(fpup_data, size);
            });
        };
        pup_object(p, container, meta, body);
    }

    /**
     * Pups most STL-like containers, including strings. This
     * function should only be called by pupper implementations.
     *
     * Puppers commonly deal in formats where strings are considered
     * fundamental/built-in types - if this is not the case, the pupper
     * must specialize on `std::string`, and pup to this function, e.g.:
     *
     * ```{.cpp}
     * template <>
     * void pupene::BinaryReader::pup(std::string& value, const Meta& meta) {
     *    pup_iterable(*this, value, meta);
     * }
     * ```
     *
     * The `container` object is encoded as `size` (element count) followed by its contents.
     *
     * #### Example/layout
     *
     * ```{.cpp}
     * using pupene;
     * using int_string_pair = std::pair<uint32_t, std::string>;
     *
     * std::vector<int_string_pair> pairs =
     *     {{1, "one"}, {2, "two"}, {4, "four"}};
     *
     * auto pupper = debug_pupper(std::cout);
     * pup(pupper, pairs, {});
     *
     * // alternatively:
     * do_pup(NullPupper{}, pairs, true);
     * ```
     *
     * DebugPupper output:
     *
     * ```
     * object()
     *   pup(size, 3)
     *   array(data)
     *     object()
     *       pup(key, 1)
     *       pup(value, one)
     *     ~object()
     *     object()
     *       pup(key, 2)
     *       pup(value, two)
     *     ~object()
     *     object()
     *       pup(key, 4)
     *       pup(value, four)
     *     ~object()
     *   ~array(data)
     * ~object()
     * ```
     *
     * @see enable_if_pup_iterable
     */
    template <typename P,
              typename Iterable,
              typename = enable_if_pup_iterable<Iterable>>
    void pup_iterable(Pupper<P>& p,
                      Iterable& container,
                      const Meta& meta) {

        auto pup_entries = [&vs = container](auto&& fpup, auto size) {
            vs.resize(size);

            for (auto& v : vs)
                fpup(v, {});
        };

        pup_object_container(p, container, meta, pup_entries);
    }
}