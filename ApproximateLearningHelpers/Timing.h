#pragma once

#include <vector>
#include <algorithm>
#include <chrono>

template <typename Y>
class Timer
{
    typedef std::chrono::high_resolution_clock	high_resolution_clock;
public:

    void Start()
    {
        m_startTime = high_resolution_clock::now();
    }

    Y GetElapsedTime()
    {
        return std::chrono::duration_cast<Y>(high_resolution_clock::now() - m_startTime);
    }

    template <typename T, typename Traits>
    friend std::basic_ostream<T, Traits>& operator<<(std::basic_ostream<T, Traits>& out, Timer<Y>& _timer)
    {
        return out << _timer.GetElapsedTime().count();
    }

    high_resolution_clock::time_point m_startTime;
};

struct TimeStruct
{
    std::vector<__int64> results;
    __int64 GetMin()
    {
        __int64 min = 999999999;
        for (auto i : results)
        {
            if (i < min)min = i;
        }

        return min;
    }
    __int64 GetMax()
    {
        __int64 max = 0;
        for (auto i : results)
        {
            if (i > max)max = i;
        }

        return max;
    }

    __int64 GetMedian()
    {
        std::sort(results.begin(), results.end());
        return results[results.size() / 2];
    }
};

template <typename T, T(*f)(T), typename UNIT, unsigned int TIMING_SAMPLE_COUNT, unsigned int TIMING_SAMPLES>
std::pair<TimeStruct, double> TimeFunction()
{
    Timer<UNIT> timer;
    unsigned int samples = TIMING_SAMPLE_COUNT;
    TimeStruct t;

    double  p = 0;
    for (int tt = 0; tt < TIMING_SAMPLES; tt++)
    {
        timer.Start();
        for (unsigned int i = 0; i < samples; i++)
        {
            p += f((T)i);
        }
        __int64 time = timer.GetElapsedTime().count();
        t.results.push_back(time);
    }

    auto ret = std::make_pair(t, p);
    return ret;
}

template <typename T, T(*f)(T), typename UNIT, unsigned int TIMING_SAMPLE_COUNT>
std::pair<TimeStruct, double> TimeFunctionInput(const std::vector<T>& _inputs)
{
    Timer<UNIT> timer;
    unsigned int samples = TIMING_SAMPLE_COUNT;
    TimeStruct t;

    double  p = 0;
    for (int tt = 0; tt < TIMING_SAMPLES; tt++)
    {
        timer.Start();
        for (unsigned int i = 0; i < _inputs.size(); i++)
        {
            p += f(_inputs[i]);
        }
        __int64 time = timer.GetElapsedTime().count();
        t.results.push_back(time);
    }

    auto ret = std::make_pair(t, p);
    return ret;
}