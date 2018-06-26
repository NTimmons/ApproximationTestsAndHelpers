#pragma once
#include <type_traits>
#include <iostream>
#include <string>
#include <vector>

unsigned long NumberOfFloatStepsBetween(float _min, float _max)
{
    unsigned long counter = 0;

    float f = _min;
    while (f < _max)
    {
        f = std::nextafterf(f, _max + std::numeric_limits< float >::epsilon());
        counter++;
    }

    return counter;
}

unsigned long NumberOfDoubleStepsBetween(double _min, double _max)
{
    unsigned long counter = 0;

    double f = _min;
    while (f < _max)
    {
        f = std::nextafter(f, _max + std::numeric_limits< double >::epsilon());
        counter++;
    }
    return counter;
}



struct OperationDef
{
    OperationDef(std::string _op, long double _added, long double _current)
        :_operation(_op), _addedError(_added), _currentError(_current)
    {}

    std::string _operation;
    long double _addedError;
    long double _currentError;
};

class FloatError
{
public:
    FloatError(float _x)
    {
        m_name = "FloatID_";
        m_name.append(std::to_string(floatIDCounter));

        m_value = _x;
        m_accurateValue = _x;
        m_error = 0.0;
    }

    FloatError(float _x, long double _error)
    {
        m_name = "FloatID_";
        m_name.append(std::to_string(floatIDCounter));
        m_value = _x;
        m_accurateValue = _x;
        m_error = _error;
    }

    FloatError(float _x, long double _error, long double _accurateVal)
    {
        m_name = "FloatID_";
        m_name.append(std::to_string(floatIDCounter));

        m_value = _x;
        m_accurateValue = _accurateVal;
        m_error = _error;
    }

    FloatError()
    {
        static int floatIDCounter = 0;
        m_name = "FloatID_";
        m_name.append(std::to_string(floatIDCounter));

        m_value = 0.f;
        m_accurateValue = 0.f;
        m_error = 0.0;
    }

    ~FloatError()
    {
        //std::cout << m_name.c_str() << " error was: " << m_error << "\n";
    }

    long double GetCumulativeError()
    {
        return ((long double)m_value - m_accurateValue);
    }

    //Operators overload
    FloatError operator+(const FloatError& _in)
    {
        float val = m_value + _in.m_value;
        long double accurateVal = m_accurateValue + _in.m_accurateValue;

        long double error = ((long double)m_value + (long double)_in.m_value);

        error = abs((long double)val - error);
        m_error += error + _in.m_error;
        m_operationList.push_back(OperationDef("Add", error, m_error));
        return FloatError(val, m_error, accurateVal);
    }

    FloatError operator-(const FloatError& _in)
    {
        float val = m_value - _in.m_value;
        long double accurateVal = m_accurateValue - _in.m_accurateValue;

        long double error = ((long double)m_value - (long double)_in.m_value);

        error = abs((long double)val - error);
        m_error += error + _in.m_error;
        m_operationList.push_back(OperationDef("Sub", error, m_error));
        return FloatError(val, m_error, accurateVal);
    }

    FloatError operator*(const FloatError& _in)
    {
        float val = m_value * _in.m_value;
        long double accurateVal = m_accurateValue * _in.m_accurateValue;

        long double error = ((long double)m_value * (long double)_in.m_value);

        error = abs((long double)val - error);
        m_error += error + _in.m_error;
        m_operationList.push_back(OperationDef("Mul", error, m_error));
        return FloatError(val, m_error, accurateVal);
    }

    FloatError operator/(const FloatError& _in)
    {
        float val = m_value / _in.m_value;
        long double accurateVal = m_accurateValue / _in.m_accurateValue;

        long double error = ((long double)m_value / (long double)_in.m_value);

        error = abs((long double)val - error);
        m_error += error + _in.m_error;
        m_operationList.push_back(OperationDef("Div", error, m_error));
        return FloatError(val, m_error, accurateVal);
    }

    FloatError operator=(const FloatError& _in)
    {
        m_value = _in.m_value;
        m_error = _in.m_error;
        m_accurateValue = _in.m_accurateValue;


        m_operationList.push_back(OperationDef("Assignment", 0.0, m_error));
        return FloatError(m_value, m_error, m_accurateValue);
    }

    float m_value;
    long double m_accurateValue;
    long double m_error;

    // Additional output information.
    std::string m_name;
    std::vector<OperationDef> m_operationList;

    static int floatIDCounter;
};

#ifndef INIT_FLOATID_COUNTER
    int FloatError::floatIDCounter = 0;
    #define INIT_FLOATID_COUNTER
#endif // !INIT_FLOATID_COUNTER

