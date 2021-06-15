#pragma once

#include <type_traits>
#include <utility>

#include "utility.h"

template<typename T>
add_rvalue_reference_t<T> Declval() noexcept;

template<typename T, typename... Args>
struct LibCppIsConstructible;

template<typename Derived, typename Base>
struct IsInvalidBaseToDerivedCast {
    using base = uncvref_t<Base>;
    using derived = uncvref_t<Derived>;

    using type = typename std::integral_constant<bool,
            !std::is_same_v<base, derived> &&
            std::is_base_of_v<base, derived> &&
            !LibCppIsConstructible<derived, Base>::type::value>;
};

template<typename To, typename From>
struct IsInvalidLvalueToRvalueCast : std::false_type {
};

template<typename RefTo, typename RefFrom>
struct IsInvalidLvalueToRvalueCast<RefTo &&, RefFrom &> {
    using from = uncvref_t<RefFrom>;
    using to = uncvref_t<RefTo>;

    using type = std::integral_constant<bool,
            std::is_same_v<from, to> &&
            std::is_base_of_v<to, from>>;
};

struct IsConstructibleHelper {
    template<typename T, typename A, typename = decltype(static_cast<T>(Declval<A>()))>
    static std::integral_constant<bool, !(IsInvalidLvalueToRvalueCast<T, A>::type::value) &&
                                        !(IsInvalidBaseToDerivedCast<A, T>::type::value)>
    ReferenceConstructible(int) {};

    template<typename T, typename A>
    static std::false_type ReferenceConstructible(...) {};

    template<typename T, typename... Arg, typename = decltype(T(Declval<Arg>()...))>
    static std::true_type HasNAryConstructor(int) {};

    template<typename T, typename... Arg>
    static std::false_type HasNAryConstructor(...) {};

    template<typename T, typename A, typename = decltype(::new T(Declval<A>()))>
    static std::true_type HasUnaryConstructor(int) {};

    template<typename T, typename A>
    static std::false_type HasUnaryConstructor(...) {};
};

// LibCppIsConstructible - partial specializations
template<typename T, typename... Args>
struct LibCppIsConstructible {
    using type = decltype(IsConstructibleHelper::HasNAryConstructor<T, Args...>(0));
};

template<typename T, typename A>
struct LibCppIsConstructible<T, A> {
    using type = decltype(IsConstructibleHelper::HasUnaryConstructor<T, A>(0));
};

template<class T, class A>
struct LibCppIsConstructible<T &, A> {
    using type = decltype(IsConstructibleHelper::ReferenceConstructible<T &, A>(0));
};

template<class T, class A>
struct LibCppIsConstructible<T &&, A> {
    using type = decltype(IsConstructibleHelper::ReferenceConstructible<T &&, A>(0));
};
// LibCppIsConstructible - partial specializations

template<typename T, typename... Args>
struct IsConstructible : public LibCppIsConstructible<T, Args...>::type {
};

template<typename T>
struct IsCopyConstructible : public LibCppIsConstructible<T, const T &>::type {
};