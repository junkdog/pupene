#pragma once

#include <iostream>
#include "pupper.h"
#include "pup-core.h"


/** \file
 * Puppers for binary serialization.
 */
namespace pupene {
    struct binary_writer_pup {
        template <typename T>
        static void pup(std::ostream& stream, T& value) {
            stream.write(reinterpret_cast<char*>(&value), sizeof(T)); //NOLINT
        }
    };

    struct binary_reader_pup {
        template <typename T>
        static void pup(std::istream& stream, T& value) {
            stream.read(reinterpret_cast<char*>(&value), sizeof(T)); // NOLINT
        }
    };

    /**
     * @tparam Stream e.g. `std::istream`, `std::ostream`
     * @tparam PupStream must provide a static function `pup(Stream, T)`
     *
     * @see BinaryWriter
     * @see BinaryReader
     */
    template <typename Stream, typename PupStream>
    class BinaryPupper : public Pupper<BinaryPupper<Stream, PupStream>> {
    public:
        explicit BinaryPupper(Stream& stream)
            : stream(stream) {}
        ~BinaryPupper() override = default;

        template <typename T>
        PupPolicy begin(T& /*value*/, const Meta& /*meta*/) {
            return PupPolicy::pup_object;
        }

        template <typename T>
        void pup(T& value, const Meta& /*meta*/) {
            if constexpr (std::is_same<std::string, T>{}) {
                pupene::fns::pup_iterable(*this, value, {});
            } else {
                PupStream::pup(stream, value);
            }
        }

        void end(const Meta& /*meta*/) {}

    private:
        Stream& stream;
    };

    using BinaryWriter = BinaryPupper<std::ostream, binary_writer_pup>;
    using BinaryReader = BinaryPupper<std::istream, binary_reader_pup>;
}