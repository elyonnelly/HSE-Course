#pragma once

template<typename Head, typename Tail>
struct TypeList {
    typedef Head T;
    typedef Tail U;
};

struct NullType {};