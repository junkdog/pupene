#include "json-puppers.h"

namespace pupene {
    std::string JsonWriter::quote(const std::string& s) {
        return '"' + s + '"';
    }

    void JsonWriter::end(const Meta& /*meta*/) {
        switch (graph.back()) {
            case Meta::Type::Array:
                stream << std::endl << ']';
                break;
            case Meta::Type::Object:
                stream << std::endl << '}';
                break;
            default:
                throw std::runtime_error("expected array or object");
        }

        stack.pop_back();
        graph.pop_back();
    }

    const json* JsonReader::resolve_child(const Meta& meta) {
        using Type = Meta::Type;

        if (parent_is(Type::Array)) {
            auto& parent = state.back();
            return &parent.dom->begin()[parent.idx++];
        } else { // NOLINT
            const auto current = !state.empty() ? state.back().dom : &dom;
            const auto& child = (strlen(meta.name) > 0)
                                ? current->at(meta.name)
                                : *current;
            return &child;
        }
    }

    void JsonReader::end(const Meta& /*meta*/) {
        state.pop_back();
    }

    bool JsonReader::parent_is(const Meta::Type type) const {
        return !state.empty() && state.back().meta.type == type;
    }

    std::ostream& operator<<(std::ostream& out,
                             const JsonReader& reader) {

        out << "JsonReader<" << &reader << ">" << std::endl;
        std::for_each(reader.state.crbegin(),
                      reader.state.crend(),
                      [&out](auto& s) {
                          out << "  " << s.idx << ": " << s.meta << std::endl;
                      });

        return out;
    }

    std::ostream& operator<<(std::ostream& out,
                             const Meta& meta) {
        using Type = Meta::Type;

        switch (meta.type) {
            case Type::Array:
                out << "(array)  ";
                break;
            case Type::Object:
                out << "(object) ";
                break;
            case Type::Value:
                out << "(value)  ";
                break;
        }

        if (strlen(meta.name) > 0)
            out << meta.name;

        return out;
    }
}
