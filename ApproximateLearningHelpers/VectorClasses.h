#pragma once

template<typename T, unsigned int elemCount>
struct vecN
{
public:
    vecN() {}

    template<class... Args>
    vecN(Args... args)
    {
        Set(args...);
    }

    template<class... Args>
    void Set(Args... args)
    {
        const int n = sizeof...(Args);
        static_assert(n == elemCount, "Invalid number of arguments for vector type");

        data = { { args... } };
    }

    void Multiply(T _mulVal)
    {
        std::for_each(data.begin(), data.end(), [&_mulVal](T& elem) { elem *= _mulVal; });
    }

    void Multiply(vecN<T, elemCount> _mulVal)
    {
        int counter = 0;
        std::for_each(data.begin(), data.end(), [&counter, &_mulVal](T& elem) { elem *= _mulVal.data[counter++]; });
    }

    void Add(T _addVal)
    {
        std::for_each(data.begin(), data.end(), [&_addVal](T& elem) { elem += _addVal; });
    }

    void Add(vecN<T, elemCount> _addVal)
    {
        int counter = 0;
        std::for_each(data.begin(), data.end(), [&counter, &_addVal](T& elem) { elem += _addVal.data[counter++]; });
    }

    float Length()
    {
        std::array<T, elemCount> dataSqr = data;
        std::for_each(dataSqr.begin(), dataSqr.end(), [](T& elem) { elem = elem*elem; });
        T sum = std::accumulate(dataSqr.begin(), dataSqr.end(), (T)0);
        return sqrt(sum);
    }

    void Normalise()
    {
        T len = 1.0 / Length();
        std::for_each(data.begin(), data.end(), [&len](T& elem) { elem *= len; });
    }

    vecN<T, elemCount - 1> PopElem()
    {
        vecN<T, elemCount - 1> output;
        std::copy_n(data.begin(), elemCount - 1, output.data.begin());
        return output;
    }

    vecN<T, elemCount + 1> PushElem(T _value = 0.0)
    {
        vecN<T, elemCount + 1> output;
        std::copy_n(data.begin(), elemCount, output.data.begin());
        output.data[elemCount] = _value;
        return output;
    }

    template<int _size>
    constexpr vecN<T, _size> GetResizedVector()
    {
        vecN<T, _size> output;
        std::copy_n(data.begin(), std::min((unsigned int)(_size), elemCount), output.data.begin());
        return output;
    }

    constexpr T& GetElement(int _index)
    {
        return data[_index];
    }

    template<int _index>
    constexpr T& GetElement()
    {
        return data[_index];
    }

    template<class... Indexs>
    constexpr vecN<T, sizeof...(Indexs)> GetOrderedArrayOfIndices(Indexs... indxs)
    {
        vecN<T, sizeof...(Indexs)> output;
        output.data = { { data[indxs]... } };
        return output;
    }

    constexpr T& x()
    {
        static_assert(1 <= elemCount, "Invalid number of arguments for vector type");
        return data[0];
    }

    constexpr T& y()
    {
        static_assert(2 <= elemCount, "Invalid number of arguments for vector type");
        return data[1];
    }

    constexpr T& z()
    {
        static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
        return data[2];
    }

    constexpr T& w()
    {
        static_assert(4 <= elemCount, "Invalid number of arguments for vector type");
        return data[3];
    }

    constexpr std::array<T, 3> xxx()
    {
        static_assert(1 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 3> xyz = { data[0], data[0], data[0] };
        return xyz;
    }

    constexpr std::array<T, 3> yyy()
    {
        static_assert(2 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 3> xyz = { data[1], data[1], data[1] };
        return xyz;
    }

    constexpr std::array<T, 3> zzz()
    {
        static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 3> xyz = { data[2], data[2], data[2] };
        return xyz;
    }

    constexpr std::array<T, 3> xyz()
    {
        static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 3> xyz = { data[0], data[1], data[2] };
        return xyz;
    }
    constexpr std::array<T, 3> xzy()
    {
        static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 3> xyz = { data[0], data[2], data[1] };
        return xyz;
    }

    constexpr std::array<T, 3> yzx()
    {
        static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 3> xyz = { data[1], data[2], data[0] };
        return xyz;
    }

    constexpr std::array<T, 3> yxz()
    {
        static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 3> xyz = { data[1], data[0], data[2] };
        return xyz;
    }

    constexpr std::array<T, 3> zyx()
    {
        static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 3> xyz = { data[2], data[1], data[0] };
        return xyz;
    }

    constexpr std::array<T, 3> zxy()
    {
        static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 3> xyz = { data[2], data[0], data[1] };
        return xyz;
    }

    constexpr std::array<T, 2> xx()
    {
        static_assert(1 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 2> xyz = { data[0], data[0] };
        return xyz;
    }

    constexpr std::array<T, 2> xy()
    {
        static_assert(2 <= elemCount, "Invalid number of arguments for vector type");

        std::array<T, 2> xyz = { data[0], data[1] };
        return xyz;
    }

    constexpr std::array<T, 2> xz()
    {
        static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 2> xyz = { data[0], data[2] };
        return xyz;
    }

    constexpr std::array<T, 2> yx()
    {
        static_assert(2 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 2> xyz = { data[1], data[0] };
        return xyz;
    }

    constexpr std::array<T, 2> yy()
    {
        static_assert(2 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 2> xyz = { data[1], data[1] };
        return xyz;
    }

    constexpr std::array<T, 2> yz()
    {
        static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 2> xyz = { data[1], data[2] };
        return xyz;
    }

    constexpr std::array<T, 2> zx()
    {
        static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 2> xyz = { data[2], data[0] };
        return xyz;
    }
    constexpr std::array<T, 2> zy()
    {
        static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 2> xyz = { data[2], data[1] };
        return xyz;
    }
    constexpr std::array<T, 2> zz()
    {
        static_assert(3 <= elemCount, "Invalid number of arguments for vector type");
        std::array<T, 2> xyz = { data[2], data[1] };
        return xyz;
    }

    std::array<T, elemCount> data;
};
typedef vecN<float, 4> tfloat4;
typedef vecN<float, 3> tfloat3;
typedef vecN<float, 2> tfloat2;
typedef vecN<float, 1> tfloat1;

template<typename T>
struct vec2
{
    vec2() {}

    void Scale(T _scale)
    {
        x *= _scale;
        y *= _scale;
    }

    vec2(T _x, T _y) : x(_x), y(_y) {}
    T x;
    T y;
};

template<typename T>
struct vec3
{
    vec3() {}

    void Scale(T _scale)
    {
        x *= _scale;
        y *= _scale;
        z *= _scale;
    }

    vec3(T _x, T _y) : x(_x), y(_y), z(_z) {}
    T x;
    T y;
    T z;
};

template<typename T>
struct vec4
{
    vec4() {}

    void Scale(T _scale)
    {
        x *= _scale;
        y *= _scale;
        z *= _scale;
        w *= _scale;
    }

    vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
    T x;
    T y;
    T z;
    T w;
};

struct int2
{
    int2(int _x, int _y) : x(_x), y(_y) {}
    int x, y;
};

struct ubyte4
{
    ubyte4() :x(0), y(0), z(0), w(0) {}
    ubyte4(unsigned char _x, unsigned char _y, unsigned char _z, unsigned char _w) :x(_x), y(_y), z(_z), w(_w) {}
    unsigned char x, y, z, w;
};

struct int4
{
    int4() :x(0), y(0), z(0), w(0) {}
    int4(int _x, int _y, int _z, int _w) :x(_x), y(_y), z(_z), w(_w) {}
    int x, y, z, w;
};

struct float4
{
    float4() :x(0.f), y(0.f), z(0.f), w(0.f) {}
    float4(float _x, float _y, float _z, float _w) :x(_x), y(_y), z(_z), w(_w) {}
    float x, y, z, w;
};
