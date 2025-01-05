/*
 * Copyright (c) 2025. Doomhowl Interactive - bramtechs/brambasiel
 * MIT License. Absolutely no warranty.
 *
 * File created on: 05-01-2025
 */

#pragma once
#include <type_traits>
#include <utility>

namespace howl {
// Utility types for internal use
namespace detail {
// Calculate the maximum size of the types
template<typename... Types>
struct max_sizeof;

template<typename First, typename... Rest>
struct max_sizeof<First, Rest...> {
    static constexpr size_t value = sizeof(First) > max_sizeof<Rest...>::value ? sizeof(First) : max_sizeof<Rest...>::value;
};

template<>
struct max_sizeof<> {
    static constexpr size_t value = 0;
};

// Calculate the maximum alignment of the types
template<typename... Types>
struct max_alignof;

template<typename First, typename... Rest>
struct max_alignof<First, Rest...> {
    static constexpr size_t value = alignof(First) > max_alignof<Rest...>::value ? alignof(First) : max_alignof<Rest...>::value;
};

template<>
struct max_alignof<> {
    static constexpr size_t value = 0;
};
}

/**
 * @brief Faster std::variant-like type without type checking.
 *
 * This variant allows storing one of multiple possible types in aligned storage,
 * but provides no runtime type checking, making it lightweight and fast.
 * The user must ensure they know the correct type stored before accessing it.
 */
template<typename... Types>
class unsafe_variant {
    using Storage = typename std::aligned_storage<
        detail::max_sizeof<Types...>::value,
        detail::max_alignof<Types...>::value>::type;

    Storage storage_;

public:
    unsafe_variant() = default;

    template<typename T, typename std::enable_if_t<(... || std::is_same_v<std::decay_t<T>, Types>), int> = 0>
    unsafe_variant(T&& value)
    {
        set(value);
    }

    // Sets the variant with a new value of type T
    template<typename T, typename std::enable_if_t<(... || std::is_same_v<std::decay_t<T>, Types>), int> = 0>
    void set(T&& value)
    {
        new (&storage_) std::decay_t<T>(std::forward<T>(value)); // Construct object of type T in place
    }

    // Retrieves the value of type T (non-const version)
    template<typename T>
    T& get()
    {
        return *reinterpret_cast<T*>(&storage_); // Direct access, no type checking
    }

    // Retrieves the value of type T (const version)
    template<typename T>
    const T& get() const
    {
        return *reinterpret_cast<const T*>(&storage_); // Direct access, no type checking
    }

    // Destructor (trivial in this case, but could be extended)
    ~unsafe_variant() = default;
};

}
