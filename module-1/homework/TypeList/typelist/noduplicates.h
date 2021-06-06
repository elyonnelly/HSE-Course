#pragma once

#include "erase.h"
#include "typelist.h"

template<typename TList>
struct NoDuplicates;

template<typename Head, typename Tail>
struct NoDuplicates<TypeList<Head, Tail>> {
    typedef TypeList<Head, typename NoDuplicates<typename EraseAll<Tail, Head>::NewTypeList>::NewTypeList> NewTypeList;
};

template<>
struct NoDuplicates<NullType> {
    typedef NullType NewTypeList;
};