#pragma once

#include "null-pupper.h"
#include <iostream>

/** \file
 * Introspect objects as they reach puppers.
 */
namespace pupene {
    /**
     * This pupper wraps another pupper, logging each call to
     * Wrapped::begin(), Wrapped::pup() and wrapped::end(). If the
     * wrapped pupper returns PupPolicy::consume_object for any
     * object/member-of-object, the associated pup() functions will
     * not be invoked.
     *
     * #### Example
     *
     * ```{.cpp}
     * using pupene;
     * using int_string_pair = std::pair<uint32_t, std::string>;
     *
     * std::vector<int_string_pair> pairs =
     *     {{1, "one"}, {2, "two"}, {4, "four"}};
     *
     * // alternative 1 - pups to std::cout
     * auto pupper = debug_pupper();
     * pup(pupper, pairs, {});
     *
     * // alternative 2 - save output to string
     * std::string debug_str = to_debug(pairs);
     * ```
     *
     * Annotated DebugPupper output:
     *
     * ```
     * object()               // begin Meta::Type::Object
     *   pup(size, 3)         // pup
     *   array(data)          // begin Meta::Type::Array
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
     *   ~array(data)         // end Meta::Type::Array
     * ~object()              // end Meta::Type::Object
     * ```
     *
     * @see debug_pupper
     * @see do_pup
     * @tparam Wrapped
     */
    template<typename Wrapped>
    class DebugPupper : public Pupper<DebugPupper<Wrapped>> {
    public:
        explicit DebugPupper(Wrapped& pupper,
                             std::ostream& out)
                : wrapped(pupper), out(out) {}

        ~DebugPupper() override = default;

        template <typename T>
        PupPolicy begin(T& value, const Meta& meta) {
            out << indent()
                << to_string(meta)
                << "(" << meta.name << ")"
                << std::endl;

            depth++;

            return wrapped.begin(value, meta);
        }

        void end(const Meta& meta) {
            depth--;

            out << indent()
                << "~" << to_string(meta)
                << "(" << meta.name << ")"
                << std::endl;

            wrapped.end(meta);
        }

        template<typename T>
        void pup(T& value, const Meta& meta) {
            out << indent() << "pup(" << meta.name;
            wrapped.pup(value, meta);
            if constexpr (is_puppable<T>()) {
                out << ", " << value << ")" << std::endl;
            } else {
                out << ", " << "PUPPER" << ")" << std::endl;
            }
        }

    private:
        Wrapped& wrapped;
        std::ostream& out;
        uint16_t depth = 0;

        std::string indent() {
            return std::string(depth * 2ul, ' ');
        }

        std::string to_string(const Meta& meta) const {
            switch (meta.type) {
                case Meta::Type::Array:
                    return "array";
                case Meta::Type::Object:
                    return "object";
                case Meta::Type::Value:
                    return "value";
            }
        }
    };
}