#pragma once

#include "traits.h"

/** \file
 * Pupper base class.
 */
namespace pupene {

    /**
     * Controls an object's pup() behavior. It's possible
     * for a Pupper to opt-out of invoking pup on its members.
     *
     * @see pupene::fns::begin
     * @see pupene::fns::pup_object
     */
    enum class PupPolicy {
        /** pup object using its component pup functions */
        pup_object,

        /**
         * Pupper deals with object as-is, no associated
         * pup functions will be invoked.
         */
        consume_object
    };

    /**
     * Base class for working with _puppable_ types. Puppers can conceptually
     * be divided into serialization and utility-based.
     *
     * #### Serialization puppers
     *
     * `pupene` does not internally distinguish between serialization/deserialization;
     * each target format is therefore implemented as a reader/writer pair:
     * - [BinaryReader](@ref BinaryReader) and [BinaryWriter](@ref BinaryWriter)
     * - JsonReader and JsonWriter
     *
     *
     * #### Utility puppers
     * Custom and utility-related puppers are usually singular:
     * - DebugPupper: Wraps another Pupper and prints everything reaching it.
     * - NullPupper: Does nothing. Can be combined with DebugPupper.
     * - [EditorPupper](https://github.com/junkdog/pupene_example), auto-generates editor
     *   UI:s for objects, using [imgui](https://github.com/ocornut/imgui).
     *
     *
     * #### Creating puppers: Minimal exemplar
     *
     * ```{.cpp}
     * class MinimalPupper : public Pupper<MinimalPupper> {
     * public:
     *
     *     template<typename T>
     *     PupPolicy begin(T& value, const Meta& meta) {
     *         return PupPolicy::pup_object;
     *     }
     *
     *     template<typename T>
     *     void pup(T& value, const Meta& meta) {
     *         // T is integer, floating point or std::string
     *     }
     *
     *     void end(const Meta& meta) {}
     * };
     * ```
     *
     * When a pupper is better equipped to deal with an object as-is, without
     * pupping its component parts, begin() can return PupPolicy::consume_object, e.g.
     * by using a template specialization:
     *
     * ```{.cpp}
     * template <>
     * inline void MinimalPupper::begin(Color& value, const Meta& meta) {
     *     set_background(value);
     *     return PupPolicy::consume_object; // pup not invoked for members
     * }
     * ```
     *
     * @tparam Derived
     */
    template<typename Derived>
    class Pupper {

    public:

        explicit Pupper() = default;
        virtual ~Pupper() = default;

        template<typename T>
        PupPolicy begin_impl(T& value, const Meta& meta) {
            return static_cast<Derived*>(this)->begin(value, meta);
        }

        template<typename T, typename = enable_if_puppable<T>>
        void pup_impl(T& value, const Meta& meta) {
            static_cast<Derived*>(this)->pup(value, meta);
        }

        void end_impl(const Meta& meta) {
            static_cast<Derived*>(this)->end(meta);
        }
    };
}