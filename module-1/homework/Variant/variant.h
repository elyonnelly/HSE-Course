#include <cstdlib>
#include <utility>

#pragma once

namespace task {
template <size_t Index>
struct InPlaceIndex {
    explicit InPlaceIndex() = default;
};

template <size_t Index>
constexpr InPlaceIndex<Index> kInPlaceIndex{};

// UnionList

template <size_t Index, typename... Types>
union UnionList;

template <size_t Index>
union UnionList<Index> {};

template <size_t Index, typename T, typename... Types>
union UnionList<Index, T, Types...> {
public:
    T head;
    UnionList<Index + 1, Types...> tail;

    UnionList() : tail() {
        if (!std::is_trivially_constructible<T>()) {
            new (&head) T();
        }
    }

    ~UnionList() {
        if (!std::is_trivially_destructible<T>()) {
            head.~T();
        }
    }
};

// UnionList utilities

template <size_t AssignIndex, typename U, size_t CurrentIndex, typename Head, typename... Tail>
static void SetToUnionList(U&& value, InPlaceIndex<0>, UnionList<CurrentIndex, Head, Tail...>& u) {
    u.head = value;
}

template <size_t Index, typename U, size_t CurrentIndex, typename Head, typename... Tail>
static void SetToUnionList(U&& value, InPlaceIndex<Index>,
                           UnionList<CurrentIndex, Head, Tail...>& u) {
    SetToUnionList<Index - 1>(std::forward<U>(value), kInPlaceIndex<Index - 1>, u.tail);
}

template <typename U>
static constexpr auto&& GetFromUnionList(U&& v, InPlaceIndex<0>) {
    return std::forward<U>(v).head;
}

template <typename U, std::size_t AccessIndex>
static constexpr auto&& GetFromUnionList(U&& v, InPlaceIndex<AccessIndex>) {
    return GetFromUnionList(std::forward<U>(v).tail, kInPlaceIndex<AccessIndex - 1>);
}

// end of UnionList

// TypeList

template <typename Head, typename... Tail>
struct TypeList {
    typedef Head T;
    typedef TypeList<Tail...> U;
};

template <typename TList, unsigned int index>
struct TypeAt;

template <typename Head, typename... Tail>
struct TypeAt<TypeList<Head, Tail...>, 0> {
    typedef Head TargetType;
};

template <typename Head, typename... Tail, unsigned int index>
struct TypeAt<TypeList<Head, Tail...>, index> {
    typedef typename TypeAt<TypeList<Tail...>, index - 1>::TargetType TargetType;
};

// end of TypeList

// Find position of type

const static size_t typeNotFound = -1;
const static size_t moreThanOneMatch = -2;

template <size_t SizeOfFounded>
constexpr size_t FindPosition(size_t currentPosition, size_t foundedPosition,
                              const bool (&founded)[SizeOfFounded],
                              const bool (&convertible)[SizeOfFounded]) {
    if (currentPosition == SizeOfFounded) {
        return foundedPosition;
    }

    if (founded[currentPosition] || convertible[currentPosition]) {
        foundedPosition = foundedPosition == typeNotFound ? currentPosition : moreThanOneMatch;
    }

    return FindPosition(currentPosition + 1, foundedPosition, founded, convertible);
}

template <typename TargetType, typename... Types>
struct FindExactlyOneChecked {
    constexpr static bool foundedSameTypes[sizeof...(Types)] = {
        std::is_same<TargetType, Types>::value...};
    constexpr static bool foundedConvertibleTypes[sizeof...(Types)] = {
        std::is_convertible<TargetType, Types>::value...};
    constexpr static size_t foundedPosition =
        FindPosition(0, typeNotFound, foundedSameTypes, foundedConvertibleTypes);
};

template <typename T>
struct FindExactlyOneChecked<T> {
    static_assert(!std::is_same<T, T>::value && !std::is_convertible<T, T>::value, "Type not in variant");
};

template <typename TargetType, typename... Types>
struct FindExactlyOneType : public FindExactlyOneChecked<TargetType, Types...> {};

template <typename... Types>
class Variant;

template <size_t Index, typename T>
struct VariantAlternative;

template <size_t Index, typename T>
using variant_alternative_t = typename VariantAlternative<Index, T>::type;

template <size_t Index, typename... Types>
struct VariantAlternative<Index, Variant<Types...>> {
    using type = typename TypeAt<TypeList<Types...>, Index>::TargetType;
};

template <typename... Types>
class Variant {
public:
    constexpr Variant() noexcept {
    }

    template <typename T, size_t Position = FindExactlyOneType<T, Types...>::foundedPosition>
    Variant& operator=(T&& t) noexcept {
        SetToUnionList(std::forward<T>(t), kInPlaceIndex<Position>, unionList_);
        return *this;
    }

    template <typename T, typename... Types_>
    friend constexpr auto&& GenericGet(Variant<Types_...>& v);

private:
    UnionList<0, Types...> unionList_;
};

template <typename T, typename... Types>
constexpr auto&& GenericGet(Variant<Types...>& v) {
    return GetFromUnionList(std::forward<Variant<Types...>>(v).unionList_,
                            InPlaceIndex<FindExactlyOneType<T, Types...>::foundedPosition>());
}

template <size_t Index, typename... Types>
constexpr const variant_alternative_t<Index, Variant<Types...>>& Get(Variant<Types...>& v) {
    return GenericGet<typename TypeAt<TypeList<Types...>, Index>::TargetType>(v);
}

template <size_t Index, typename... Types>
constexpr variant_alternative_t<Index, Variant<Types...>>&& Get(Variant<Types...>&& v) {
    return GenericGet<typename TypeAt<TypeList<Types...>, Index>::TargetType>(v);
}

template <typename T, typename... Types>
constexpr const T& Get(Variant<Types...>& v) {
    return GenericGet<T>(v);
}

template <typename T, typename... Types>
constexpr T&& Get(Variant<Types...>&& v) {
    return GenericGet<T>(v);
}
};