#pragma once

#include <iostream>

#define LEARNINGSTEPS 10

template< typename T>
class TrendingEntry
{
public:

    TrendingEntry() :m_learning(true), m_value(T(0)), m_HitCount(1) {}
    TrendingEntry(T _val) :m_learning(true), m_value(T(_val)), m_HitCount(1) {}

    bool IsLearning()
    {
        return m_learning;
    }

    void Write(T _value)
    {
        if (!m_learning)
            return;
        else
        {
            m_HitCount++;
            m_value += _value;

            if (m_HitCount >= LEARNINGSTEPS)
            {
                m_value /= (float)m_HitCount;
                m_learning = false;
            }
        }
    }

    T Read()
    {
        if (m_learning)
            return m_value / m_HitCount;
        else
            return m_value;
    }
    
    bool    m_learning;
    int     m_HitCount;
    T       m_value;

};

template<typename T, T(*function)(int), int(*hashFunction)(int)>
T TrendingMemoiseFunction(int input)
{
    static std::map<int, TrendingEntry<T> > sMap;


    int hash = hashFunction(input);
    auto entry = sMap.find(hash);
    if (entry != sMap.end())
    {
        if ((*entry).second.IsLearning())
        {
            T res = function(input);
            (*entry).second.Write(res);
            return res;
        }
        else
        {
            return (*entry).second.Read();
        }
    }
    else
    {
        T res = function(input);
        sMap[hash] = TrendingEntry<T>(res);
        return res;
    }
}



int HalfHashFunction(int input)
{
    return input / 2;
}

float TestFunc(int input)
{
    return pow((float)input, 2);
}

void TestTheTrend()
{

    //0-8
    //4 slots
    //0:1 20:80 -> 0.8
    //2:3 100:0 -> 2
    //4:5 50:50 -> 4.5
    //6:7 90:10 -> 6.1

    std::vector<int> input = {      0,0,
                                    1,1,1,1,1,1,1,1,
                                    2,2,2,2,2,2,2,2,2,2,
                                    
                                    4,4,4,4,4,
                                    5,5,5,5,5,
                                    6,6,6,6,6,6,6,6,6,
                                    7
    };

    for (auto i : input)
    {
        std::cout << TrendingMemoiseFunction<float, TestFunc, HalfHashFunction>(i) << "\n";
    }

    std::cout << "0 " << TrendingMemoiseFunction<float, TestFunc, HalfHashFunction>(0) << "\n";
    std::cout << "1 " << TrendingMemoiseFunction<float, TestFunc, HalfHashFunction>(1) << "\n";
    std::cout << "2 " << TrendingMemoiseFunction<float, TestFunc, HalfHashFunction>(2) << "\n";
    std::cout << "3 " << TrendingMemoiseFunction<float, TestFunc, HalfHashFunction>(3) << "\n";
    std::cout << "4 " << TrendingMemoiseFunction<float, TestFunc, HalfHashFunction>(4) << "\n";
    std::cout << "5 " << TrendingMemoiseFunction<float, TestFunc, HalfHashFunction>(5) << "\n";
    std::cout << "6 " << TrendingMemoiseFunction<float, TestFunc, HalfHashFunction>(6) << "\n";
    std::cout << "7 " << TrendingMemoiseFunction<float, TestFunc, HalfHashFunction>(7) << "\n";


    int ii = 0;
}