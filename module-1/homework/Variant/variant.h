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
    typedef Head head;
    typedef TypeList<Tail...> tail;
};

template <typename TList, size_t Index>
struct TypeAt;

template <typename Head, typename... Tail>
struct TypeAt<TypeList<Head, Tail...>, 0> {
    typedef Head target_type;
};

template <typename Head, typename... Tail, size_t Index>
struct TypeAt<TypeList<Head, Tail...>, Index> {
    typedef typename TypeAt<TypeList<Tail...>, Index - 1>::target_type target_type;
};

// end of TypeList

// Find position of type

const static size_t kTypeNotFound = -1;
const static size_t kMoreThanOneMatch = -2;
const static size_t kMoreThanOneConvertible = -2;

constexpr static bool CheckLegalIndex(size_t index) {
    return index != kTypeNotFound && index != kMoreThanOneMatch && index != kMoreThanOneConvertible;
}

template <size_t SizeOfFounded>
constexpr size_t FindPosition(size_t current_position, size_t founded_position,
                              const bool (&same_type)[SizeOfFounded],
                              const bool (&convertible_type)[SizeOfFounded]) {
    if (current_position == SizeOfFounded) {
        return founded_position;
    }

    if (same_type[current_position] &&
        (founded_position == kTypeNotFound || founded_position == kMoreThanOneConvertible ||
         convertible_type[founded_position])) {
        founded_position = current_position;
    } else if (same_type[current_position] && same_type[founded_position]) {
        founded_position = kMoreThanOneMatch;
    } else if (founded_position == kTypeNotFound && convertible_type[current_position]) {
        founded_position = current_position;
    } else if (convertible_type[current_position] && convertible_type[founded_position] &&
               !same_type[founded_position]) {
        founded_position = kMoreThanOneConvertible;
    }

    return FindPosition(current_position + 1, founded_position, same_type, convertible_type);
}

template <typename TargetType, typename... Types>
struct FindExactlyOneChecked {
    constexpr static bool kFoundedSameTypes[sizeof...(Types)] = {
        std::is_same<TargetType, Types>::value...};
    constexpr static bool kFoundedConvertibleTypes[sizeof...(Types)] = {
        std::is_convertible<TargetType, Types>::value...};
    constexpr static size_t kFoundedPosition =
        FindPosition(0, kTypeNotFound, kFoundedSameTypes, kFoundedConvertibleTypes);
};

template <typename T>
struct FindExactlyOneChecked<T> {
    static_assert(!std::is_same<T, T>::value, "Type not in empty type list");
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
    using type = typename TypeAt<TypeList<Types...>, Index>::target_type;
};

template <typename... Types>
class Variant {
public:
    constexpr Variant() noexcept {
    }

    template <typename T, size_t Position = FindExactlyOneType<T, Types...>::kFoundedPosition>
    Variant& operator=(T&& t) noexcept {
        if (CheckLegalIndex(Position)) {
            SetToUnionList(std::forward<T>(t), kInPlaceIndex<Position>, union_list_);
        }
        return *this;
    }

    template <typename T, typename... VariantTypes>
    friend constexpr auto&& GenericGet(Variant<VariantTypes...>& v);

private:
    UnionList<0, Types...> union_list_;
};

template <typename T, typename... Types>
constexpr auto&& GenericGet(Variant<Types...>& v) {
    return GetFromUnionList(std::forward<Variant<Types...>>(v).union_list_,
                            InPlaceIndex<FindExactlyOneType<T, Types...>::kFoundedPosition>());
}

template <size_t Index, typename... Types>
constexpr const variant_alternative_t<Index, Variant<Types...>>& Get(Variant<Types...>& v) {
    return GenericGet<typename TypeAt<TypeList<Types...>, Index>::target_type>(v);
}

template <size_t Index, typename... Types>
constexpr variant_alternative_t<Index, Variant<Types...>>&& Get(Variant<Types...>&& v) {
    return GenericGet<typename TypeAt<TypeList<Types...>, Index>::target_type>(v);
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