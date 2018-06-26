#pragma once
#include <array>

// Array lookup table building -> need to expand with variadics to allow for function arguments other than index number.
/////////////////////

template<class Function, std::size_t... Indices>
constexpr auto MakeLookUpTableHelper(Function f, const float _min, const float _stepSize, std::index_sequence<Indices...>)
->std::array<typename std::result_of<Function(float)>::type, sizeof...(Indices)>
{
    return { { f(_min + (Indices*_stepSize))... } };
}

template<int N, class Function>
constexpr auto MakeLookUpTable(Function f, const float _min, const float _stepSize)
->std::array<typename std::result_of<Function(std::size_t)>::type, N>
{
    return MakeLookUpTableHelper(f, _min, _stepSize, std::make_index_sequence<N>{});
}
