#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include "pupper.h"

using json = nlohmann::json;

/** \file
 * Optional JSON serialization support
 */
namespace pupene {
    using std::literals::string_literals::operator""s;

    class JsonWriter : public Pupper<JsonWriter> {
    public:
        explicit JsonWriter(std::ostream& stream) : stream(stream) {}
        ~JsonWriter() override = default;

        template <typename T>
        PupPolicy begin(T& /*value*/, const Meta& meta) {
            if (!stack.empty()) {
                if (stack.back() > 0)
                    stream << ", " << std::endl;

                stack.back()++;
            }

            if (strlen(meta.name) > 0)
                stream << quote(meta.name) << ": ";

            switch (meta.type) {
                case Meta::Type::Array:
                    stream << '[';
                    break;
                case Meta::Type::Object:
                    stream << '{';
                    break;
                default:
                    throw std::runtime_error("expected array or object");
            }

            stack.emplace_back(0);
            graph.emplace_back(meta.type);

            return PupPolicy::pup_object;
        }
        void end(const Meta& /*meta*/);

        template <typename T,
                  typename = enable_if_puppable<T>>
        void pup(T& value, const Meta& meta) {
            if (stack.empty())
                throw std::invalid_argument("begin(meta) not called");

            if (stack.back() > 0)
                stream << ',';

            stream << std::endl;

            if (strlen(meta.name) > 0)
                stream << quote(meta.name) << ": ";

            if constexpr (is_puppable<T>() && sizeof(T) > 4) {
                stream << '"' << value << '"';
            } else {
                stream << value;
            }

            stack.back()++;
        }

    private:
        static std::string quote(const std::string& s);

        std::ostream& stream;
        std::vector<uint16_t> stack {};
        std::vector<pupene::Meta::Type> graph {};
    };

    class JsonReader : public Pupper<JsonReader> {
    public:
        explicit JsonReader(std::istream& in)
                : dom(json::parse(in)) {}
        ~JsonReader() override = default;

        friend std::ostream& operator<<(std::ostream& out,
                                        const JsonReader& reader);

        template <typename T>
        PupPolicy begin(T&, const Meta& meta) {
            using Type = Meta::Type;

            if (meta.type == Type::Value)
                throw std::invalid_argument("must be Array or Object");

            const auto child = resolve_child(meta);
            state.emplace_back(JsonState{child, meta, 0});

            return PupPolicy::pup_object;
        }

        void end(const Meta& /*meta*/);

        template <typename T,
                  typename = enable_if_puppable<T>>
        void pup(T& value, const Meta& meta) {
            using Type = pupene::Meta::Type;

            if (!parent_is(Type::Array)) {
                value = state.back().dom->at(meta.name);
            } else {
                auto& next = state.back();
                value = next.dom->begin()[next.idx++];;
            }
        }

    private:
        struct JsonState {
            const json* dom;
            const Meta meta;
            uint16_t idx = 0;
        };

        const json dom;
        std::vector<JsonState> state;

        bool parent_is(Meta::Type type) const;
        const json* resolve_child(const Meta& meta);
    };

    std::ostream& operator<<(std::ostream& out,
                             const Meta& meta);
}