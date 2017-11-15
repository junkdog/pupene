#pragma once

#include <string>

namespace pupene {
    /** Holds name and type of objects. */
    struct Meta {
        enum class Type {
            Array = 1, Object, Value
        };

        Meta() : Meta("") {}
        Meta(const char* name, Type type = Type::Value)
            : name(name), type(type) {}
        ~Meta() = default;

        /** The name typically matches the name of the represented field. */
        const char* name;
        const Type type = Type::Value;
    };

    // https://blog.tartanllama.xyz/detection-idiom/
    // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf
    template <template<typename...> typename Trait, typename Enabler, typename... Args>
    struct is_detected
            : std::false_type {};

    template <template<typename...> typename Trait, typename... Args>
    struct is_detected<Trait, std::void_t<Trait<Args...>>, Args...>
            : std::true_type {};

    namespace detect {
        using std::void_t;
        using std::declval;

        template<typename T>
        using begin_member = decltype(declval<T>().begin());
        template<typename T>
        using has_begin = is_detected<begin_member, void, T>;

        template<typename T>
        using end_member = decltype(declval<T>().end());
        template<typename T>
        using has_end = is_detected<end_member, void, T>;

        template<typename T>
        using resize_member = decltype(declval<T>().resize(declval<uint32_t>()));
        template<typename T>
        using has_resize = is_detected<resize_member, void, T>;

        template<typename T>
        using size_member = decltype(declval<T>().size());
        template<typename T>
        using has_size = is_detected<size_member, void, T>;

        template <typename T, typename = void_t<>>
        struct is_map_impl : std::false_type {};

        template <typename T>
        struct is_map_impl<T, std::void_t<
            typename T::key_type,
            typename T::mapped_type>>
            : std::true_type {};

        template<typename P, typename T, typename = std::void_t<>>
        struct is_pup_impl
            : std::false_type {};

        template<typename P, typename T>
        struct is_pup_impl<P, T, void_t<
            decltype(declval<P>().pup_impl(declval<T&>(), declval<const Meta&>()))>>
            : std::true_type {};
    }

    template <typename T>
    constexpr bool is_map() {
        return detect::is_map_impl<T>::value;
    }

    template<typename T>
    constexpr bool is_pup_iterable() {
        return detect::has_begin<T>::value
            && detect::has_end<T>::value
            && detect::has_resize<T>::value
            && detect::has_size<T>::value;
    }

    template<typename T>
    constexpr bool is_pup_container() {
        return is_pup_iterable<T>()
            && !std::is_same<std::string, T>::value;
    }

    template<typename P, typename T>
    constexpr bool is_pup_impl() {
        return detect::is_pup_impl<P, T>{};
    }

    template<typename T>
    constexpr bool is_puppable() {
        return std::is_integral<T>()
            || std::is_floating_point<T>()
            || std::is_same<std::string, T>();
    }

    template <typename T>
    using enable_if_pup_iterable = typename std::enable_if_t<is_pup_iterable<T>()>;

    template <typename T>
    using enable_if_puppable_container = typename std::enable_if_t<is_pup_container<T>()>;

    template<typename T>
    using enable_if_puppable = typename std::enable_if_t<is_puppable<T>()>;

    template<typename T>
    using enable_if_not_puppable = typename std::enable_if_t<!is_puppable<T>()>;

    template<typename T>
    using enable_if_puppable_map = typename std::enable_if_t<is_map<T>()>;
}