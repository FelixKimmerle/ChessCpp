#pragma once
#include <array>

template <class T, std::size_t I, std::size_t... J>
struct MultiDimArray_t
{
    using Nested = typename MultiDimArray_t<T, J...>::type;
    using type = std::array<Nested, I>;
};

template <class T, std::size_t I>
struct MultiDimArray_t<T, I>
{
    using type = std::array<T, I>;
};

template <class T, std::size_t I, std::size_t... J>
using MultiDimArray = typename MultiDimArray_t<T, I, J...>::type;