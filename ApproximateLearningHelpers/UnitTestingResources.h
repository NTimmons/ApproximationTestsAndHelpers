#pragma once


template<typename T>
T XCubed(T x)
{
    return x * x * x;
}

int Hello()
{
    return 40;
}

int World()
{
    return 2;
}

float SomeMathFloat(float input)
{
    TypeComparison<float, double> a = Hello();
    TypeComparison<float, double> b = 1.2f;
    auto aa = a + b;
    auto aaa = a + 0.1f;


    auto e = b + World();
    auto c = World() + b;

    TypeComparison<float, double> d = a + c;
    return input * ((a + 2 + 3.f) / (4 + 5.f + 6)) + d;
}