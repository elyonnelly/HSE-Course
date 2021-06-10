#include <cstdlib>
#include <type_traits>

#pragma once

namespace task {

struct NullOpt {
    explicit constexpr NullOpt(int) {
    }
};

constexpr NullOpt kNullOpt = NullOpt(0);

struct InPlace {
    explicit InPlace() = default;
};

constexpr InPlace kInPlace = InPlace();

template <typename T, bool = std::is_trivially_destructible_v<T>>
class OptionalDestructBase;

template <typename T>
class OptionalDestructBase<T, false> {
public:
    using value_type = T;

    constexpr OptionalDestructBase() noexcept : default_(), is_some_(false) {
    }

    constexpr OptionalDestructBase(NullOpt) noexcept : default_(), is_some_(false) {
    }

    template <typename... Args>
    constexpr OptionalDestructBase(InPlace, Args&&... args)
        : value_(std::forward<Args>(args)...), is_some_(true) {
    }

    template <typename U = T>
    constexpr OptionalDestructBase(U&& value) : value_(std::forward<U>(value)), is_some_(true) {
    }

    ~OptionalDestructBase() {
        if (is_some_) {
            value_.~value_type();
        }
    }

protected:
    void Reset() noexcept {
        if (is_some_) {
            value_.~value_type();
            is_some_ = false;
        }
    }

    template <typename U = T>
    void Set(U&& value) {
        if (is_some_) {
            value_.~value_type();
        }
        value_ = std::forward<U>(value);
        is_some_ = true;
    }

    union {
        char default_;
        value_type value_;
    };

    bool is_some_;
};

template <typename T>
class OptionalDestructBase<T, true> {
public:
    using value_type = T;

    constexpr OptionalDestructBase() noexcept : default_(), is_some_(false) {
    }

    constexpr OptionalDestructBase(NullOpt) noexcept : default_(), is_some_(false) {
    }

    template <typename... Args>
    constexpr OptionalDestructBase(InPlace, Args&&... args)
        : value_(std::forward<Args>(args)...), is_some_(true) {
    }

    template <typename U = T>
    constexpr OptionalDestructBase(U&& value) : value_(std::forward<U>(value)), is_some_(true) {
    }

protected:
    void Reset() noexcept {
        if (is_some_) {
            is_some_ = false;
        }
    }

    template <typename U = T>
    void Set(U&& value) {
        value_ = std::forward<U>(value);
        is_some_ = true;
    }

    union {
        char default_;
        value_type value_;
    };

    bool is_some_;
};

template <typename T>
class Optional : public OptionalDestructBase<T> {
public:
    using value_type = T;
    using base = OptionalDestructBase<T>;

    constexpr Optional() noexcept = default;

    template <typename U = value_type>
    constexpr explicit Optional(U&& value) : base(std::forward<U>(value)) {
    }

    constexpr explicit Optional(NullOpt) noexcept : base(kNullOpt) {
    }

    template <typename... Args>
    constexpr explicit Optional(InPlace, Args&&... args) : base(kInPlace, args...) {
    }

    void Reset() noexcept {
        base::Reset();
    }

    Optional& operator=(NullOpt) noexcept {
        base::Reset();
        return *this;
    }

    template <typename U = T>
    Optional& operator=(U&& value) {
        base::Set(value);
        return *this;
    }

    template <typename U>
    constexpr T ValueOr(U&& default_value) const& {
        return base::is_some_ ? base::value_ : default_value;
    }

    template <typename U>
    constexpr T ValueOr(U&& default_value) && {
        return base::is_some_ ? base::value_ : default_value;
    }

    constexpr bool HasValue() const noexcept {
        return base::is_some_;
    }

    constexpr explicit operator bool() const noexcept {
        return HasValue();
    }

    constexpr std::add_pointer_t<const value_type> operator->() const {
        return base::value_;
    }

    constexpr std::add_pointer_t<value_type> operator->() {
        return &(base::value_);
    }

    constexpr const value_type& operator*() const& {
        return base::value_;
    }

    constexpr value_type& operator*() & {
        return base::value_;
    }

    constexpr const value_type&& operator*() const&& {
        return std::move(base::value_);
    }

    constexpr value_type&& operator*() && {
        return std::move(base::value_);
    }
};
}  // namespace task