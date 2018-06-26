#pragma once


constexpr int TermCount = 5;
constexpr float a[] = { 1,2,3,4,5 };
constexpr float b[] = { 0.1, 0.5, 0.9, 1.0, 10.0 };

float ForLoopAccurate(int input)
{
    //Implementation For-Loop
    float output = 0;
    for (int i = 0; i < TermCount; i++)
    {
        output += pow(input, a[i])*b[i];
    }
    return output;
}

float UnrolledLoopAccurate(int input)
{
    //Implementation Unrolled For-Loop
    float output = 0;
    output = pow(input, a[0])*b[0]
        + pow(input, a[1])*b[1]
        + pow(input, a[2])*b[2]
        + pow(input, a[3])*b[3]
        + pow(input, a[4])*b[4];
    return output;
}

float HandEditedLoopApproximate(int input)
{
    //Implementation Perforated For-Loop
    float output = 0;
    for (int i = 2; i < TermCount; i++)
    {
        output += pow(input, a[i])*b[i];
    }
    return output;
}

float HandEditedUnrolledApproximate(int input)
{
    float output = 0;
    output = pow(input, a[2])*b[2]
        + pow(input, a[3])*b[3]
        + pow(input, a[4])*b[4];
    return output;
}

//Simple Controlling Function
template<int index>
constexpr bool ShouldRun()
{
    if constexpr(index < 2)
        return false;
    else
        return true;
}

template<int i>
void RunIt(const float input, float& output)
{
    if constexpr(ShouldRun<i>())
        output += pow(input, a[i])*b[i];
}
template <std::size_t... Is>
void Unwinder(const float input, float& output, std::index_sequence<Is...>)
{
    //(RunIt<Is>(input, output), ...);
    using expandit = int[];
    expandit{ 0, (RunIt<Is>(input, output), 0) ... };
}
template <std::size_t N>
void ForStarter(const float _input, float& _output)
{
    Unwinder(_input, _output, std::make_index_sequence<N>());
}

float ConstExprApproximate(float input)
{
    //Approximate Implementation Perforated For-Loop
    float output = 0;
    ForStarter<TermCount>(input, output);
    return output;
}

std::vector<int> testList = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };
//Binary Search Example
int ForLoopBinSearch(const std::vector<int>& _inVec, int _val)
{
    int top = _inVec.size() - 1;
    int bottom = 0;
    int mid = top / 2;

    int maxSteps = log2(_inVec.size());
    for (int i = 0; i < maxSteps; i++)
    {
        if (_inVec[mid] == _val)
        {
            return mid;
        }
        else if (_inVec[mid] < _val)
        {
            bottom = mid + 1;
        }
        else
        {
            top = mid - 1;
        }
        mid = (top + bottom) / 2;
    }

    return mid;
}

//Perforated Binary Search
int ForLoopBinSearchPerforated(const std::vector<int>& _inVec, int _val, int stepReduction = 0)
{
    int top = _inVec.size() - 1;
    int bottom = 0;
    int mid = top / 2;

    int maxSteps = log2(_inVec.size()) - (stepReduction);
    for (int i = 0; i < maxSteps; i++)
    {
        if (_inVec[mid] == _val)
        {
            return mid;
        }
        else if (_inVec[mid] < _val)
        {
            bottom = mid + 1;
        }
        else
        {
            top = mid - 1;
        }
        mid = (top + bottom) / 2;
    }

    return mid;
}

//Max error is + and negative. So absolute error of 4 steps is +-4 steps.
int GetMaxStepSkip(int _size, int _maxStepSize, int _maxError)
{
    if (_maxError == 0)
        return 0;
    else if (_maxError == 1)
        return 2;

    return std::min(log2(_size), log2(_maxError / _maxStepSize)) + 1;
}