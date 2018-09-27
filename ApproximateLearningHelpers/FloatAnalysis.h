#pragma once
#include <type_traits>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

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

template< typename L, typename H >
class TypeComparison;

template<typename L, typename H>
struct MinMaxData
{
    MinMaxData() : m_LMin(0.0f), m_LMax(999999999999.0f), m_HMin(0.0), m_HMax(999999999999.0) {}

    void Print()
    {
        std::cout << "Low-Precision Min|Max : (" << m_LMin << "|" << m_LMax << ")\n";
        std::cout << "High-Precision Min|Max: (" << m_HMin << "|" << m_HMax << ")\n";
    }

    L m_LMin;
    L m_LMax;

    H m_HMin;
    H m_HMax;
};
struct outputPrintingData
{
    outputPrintingData(MinMaxData<float,double> _inMinMax, double _inAbsoluteError)
        : _minMax(_inMinMax), _absoluteError(_inAbsoluteError) {}
    MinMaxData<float, double> _minMax;
    double     _absoluteError;
};

struct UIDCounterClass
{
    static unsigned int UIDCounter;
    static std::vector < std::pair<std::string, outputPrintingData> > s_createdObjects;

};

template< typename L = float, typename H = double>
class TypeComparison
{
public:
    static void PrintResults()
    {
        for (auto s : UIDCounterClass::s_createdObjects)
        {
            std::cout << s.first << ": " << std::fixed << s.second._absoluteError << " \n";
            s.second._minMax.Print();
            std::cout << "\n";
        }
    }

    void EmptyInit()
    {
        m_value         = 0.0;
        m_accurateValue = 0.0;
        m_absoluteError = 0.0;
    }
    void NameInit()
    {
        m_name = std::string("[L:") + typeid(L).name() + std::string("] [H:") + typeid(H).name() + std::string("] ") + std::to_string(UIDCounterClass::UIDCounter);
        UIDCounterClass::UIDCounter++;
    }

    TypeComparison(L _val)
    {
        EmptyInit();
        NameInit();
        m_value = (L)_val;
        m_accurateValue = (H)_val;
    }

    template< typename T>
    TypeComparison(T _val)
    {
        EmptyInit();
        NameInit();
        m_value = (L)_val;
        m_accurateValue = (H)_val;
    }

    //explicit TypeComparison(L _val)
    //{
    //    EmptyInit();
    //    NameInit();
    //    m_value = _val;
    //    m_accurateValue = (H)_val;
    //}

    TypeComparison(L val, H new_error, H accurateVal, MinMaxData<L,H>& _minMaxData)
    {
        EmptyInit();
        NameInit();

        m_value         = val;
        m_accurateValue = accurateVal;
        m_absoluteError = new_error;
        m_minMaxData    = _minMaxData;
    }

    TypeComparison(L val, H new_error, H accurateVal)
    {
        EmptyInit();
        NameInit();

        m_value = val;
        m_accurateValue = accurateVal;
        m_absoluteError = new_error;
    }

    TypeComparison()
    {
        EmptyInit();
        NameInit();
    }

    //Conversion Operator
    //operator L&()   { return m_value; }
    //
    template< typename T > 
    operator T&() { return m_value; }
/*
    TypeComparison(TypeComparison<L,H> _in)
    {
        *this = _in;
    }
*/
    ~TypeComparison()
    {
        UIDCounterClass::s_createdObjects.push_back(std::make_pair(m_name, outputPrintingData(m_minMaxData,m_absoluteError) ) );
    }

    H GetCumulativeError()
    {
        return ((H)m_value - m_accurateValue);
    }


    TypeComparison<L, H> operator=(const TypeComparison<L, H>& _in)
    {
        m_value = _in.m_value;
        m_absoluteError = _in.m_absoluteError;
        m_accurateValue = _in.m_accurateValue;

        UpdateMinMax();
        m_minMaxData = CombineMinMax(m_minMaxData, _in.m_minMaxData);

        return TypeComparison<L,H>(m_value, m_absoluteError, m_accurateValue, m_minMaxData);
    }

public:

    void UpdateMinMax()
    {
        if (m_value < m_minMaxData.m_LMin)
            m_minMaxData.m_LMin = m_value;

        if (m_value > m_minMaxData.m_LMax)
            m_minMaxData.m_LMax = m_value;

        if (m_accurateValue < m_minMaxData.m_HMin)
            m_minMaxData.m_HMin = m_accurateValue;

        if (m_accurateValue > m_minMaxData.m_HMax)
            m_minMaxData.m_HMax = m_accurateValue;
    }

    MinMaxData<L, H> CombineMinMax(const MinMaxData<L,H>& _L, const MinMaxData<L, H>& _R)
    {
        MinMaxData<L, H> newData;
        newData.m_LMin = min(_L.m_LMin, _R.m_LMin);
        newData.m_HMin = min(_L.m_HMin, _R.m_HMin);

        newData.m_LMax = max(_L.m_LMax, _R.m_LMax);
        newData.m_HMax = max(_L.m_HMax, _R.m_HMax);
        
        return newData;
    }

    L m_value;
    H m_accurateValue;
    H m_absoluteError;
    MinMaxData<L, H> m_minMaxData;
    std::string m_name;
};

template<typename L, typename H>
TypeComparison<L, H> operator+(const TypeComparison<L, H>& _RH, const TypeComparison<L, H>& _LH)
{
    TypeComparison<L, H> _inL(_RH);
    TypeComparison<L, H> _inR(_LH);

    L val = _inL.m_value + _inR.m_value;
    H accurateVal = _inL.m_accurateValue + _inR.m_accurateValue;
    H error = std::abs((H)accurateVal - (H)val);
    H new_error = error + _inL.m_absoluteError + +_inR.m_absoluteError;
    return TypeComparison<L, H>(val, new_error, accurateVal);

}

template<typename L, typename H, typename T, typename = std::enable_if<std::is_arithmetic<T>::value, T>::type>
TypeComparison<L, H> operator+(const TypeComparison<L, H>& _RH, const T& _LH)
{
    TypeComparison<L, H> _inL(_RH);
    TypeComparison<L, H> _inR(_LH);

    L val = _inL.m_value + _inR.m_value;
    H accurateVal = _inL.m_accurateValue + _inR.m_accurateValue;
    H error = std::abs((H)accurateVal - (H)val);
    H new_error = error + _inL.m_absoluteError + +_inR.m_absoluteError;
    return TypeComparison<L, H>(val, new_error, accurateVal);
}

template<typename L, typename H, typename T, typename = std::enable_if<std::is_arithmetic<T>::value>::type>
TypeComparison<L, H> operator+(const T& _LH, const TypeComparison<L, H>& _RH)
{
    TypeComparison<L, H> _inL(_RH);
    TypeComparison<L, H> _inR(_LH);

    L val = _inL.m_value + _inR.m_value;
    H accurateVal = _inL.m_accurateValue + _inR.m_accurateValue;
    H error = std::abs((H)accurateVal - (H)val);
    H new_error = error + _inL.m_absoluteError + +_inR.m_absoluteError;
    return TypeComparison<L, H>(val, new_error, accurateVal);
}

template<typename L, typename H, typename T>
TypeComparison<L, H> operator-(const TypeComparison<L, H>& _RH, const TypeComparison<L, H>& _LH)
{
    TypeComparison<L, H> _inL(_RH);
    TypeComparison<L, H> _inR(_LH);

    L val = _inL.m_value - _inR.m_value;
    H accurateVal = _inL.m_accurateValue - _inR.m_accurateValue;
    H error = std::abs((H)accurateVal - (H)val);
    H new_error = error + _inL.m_absoluteError + +_inR.m_absoluteError;
    return TypeComparison<L, H>(val, new_error, accurateVal);
}

template<typename L, typename H, typename T, typename = std::enable_if<std::is_arithmetic<T>::value, T>::type>
TypeComparison<L, H> operator-(const TypeComparison<L, H>& _RH, const T& _LH)
{
    TypeComparison<L, H> _inL(_RH);
    TypeComparison<L, H> _inR(_LH);

    L val = _inL.m_value - _inR.m_value;
    H accurateVal = _inL.m_accurateValue - _inR.m_accurateValue;
    H error = std::abs((H)accurateVal - (H)val);
    H new_error = error + _inL.m_absoluteError + +_inR.m_absoluteError;
    return TypeComparison<L, H>(val, new_error, accurateVal);
}

template<typename L, typename H, typename T, typename = std::enable_if<std::is_arithmetic<T>::value>::type>
TypeComparison<L, H> operator-(const T& _LH, const TypeComparison<L, H>& _RH)
{
    TypeComparison<L, H> _inL(_RH);
    TypeComparison<L, H> _inR(_LH);

    L val = _inL.m_value - _inR.m_value;
    H accurateVal = _inL.m_accurateValue - _inR.m_accurateValue;
    H error = std::abs((H)accurateVal - (H)val);
    H new_error = error + _inL.m_absoluteError + +_inR.m_absoluteError;
    return TypeComparison<L, H>(val, new_error, accurateVal);
}


template<typename L, typename H, typename T>
TypeComparison<L, H> operator*(const TypeComparison<L, H>& _RH, const TypeComparison<L, H>& _LH)
{
    TypeComparison<L, H> _inL(_RH);
    TypeComparison<L, H> _inR(_LH);

    L val = _inL.m_value * _inR.m_value;
    H accurateVal = _inL.m_accurateValue * _inR.m_accurateValue;
    H error = std::abs((H)accurateVal - (H)val);
    H new_error = error + _inL.m_absoluteError + +_inR.m_absoluteError;
    return TypeComparison<L, H>(val, new_error, accurateVal);
}

template<typename L, typename H, typename T, typename = std::enable_if<std::is_arithmetic<T>::value, T>::type>
TypeComparison<L, H> operator*(const TypeComparison<L, H>& _RH, const T& _LH)
{
    TypeComparison<L, H> _inL(_RH);
    TypeComparison<L, H> _inR(_LH);

    L val = _inL.m_value * _inR.m_value;
    H accurateVal = _inL.m_accurateValue * _inR.m_accurateValue;
    H error = std::abs((H)accurateVal - (H)val);
    H new_error = error + _inL.m_absoluteError + +_inR.m_absoluteError;
    return TypeComparison<L, H>(val, new_error, accurateVal);
}

template<typename L, typename H, typename T, typename = std::enable_if<std::is_arithmetic<T>::value>::type>
TypeComparison<L, H> operator*(const T& _LH, const TypeComparison<L, H>& _RH)
{
    TypeComparison<L, H> _inL(_RH);
    TypeComparison<L, H> _inR(_LH);

    L val = _inL.m_value * _inR.m_value;
    H accurateVal = _inL.m_accurateValue * _inR.m_accurateValue;
    H error = std::abs((H)accurateVal - (H)val);
    H new_error = error + _inL.m_absoluteError + +_inR.m_absoluteError;
    return TypeComparison<L, H>(val, new_error, accurateVal);
}

template<typename L, typename H, typename T>
TypeComparison<L, H> operator/(const TypeComparison<L, H>& _RH, const TypeComparison<L, H>& _LH)
{
    TypeComparison<L, H> _inL(_RH);
    TypeComparison<L, H> _inR(_LH);

    L val = _inL.m_value / _inR.m_value;
    H accurateVal = _inL.m_accurateValue / _inR.m_accurateValue;
    H error = std::abs((H)accurateVal - (H)val);
    H new_error = error + _inL.m_absoluteError + +_inR.m_absoluteError;
    return TypeComparison<L, H>(val, new_error, accurateVal);
}

template<typename L, typename H, typename T, typename = std::enable_if<std::is_arithmetic<T>::value, T>::type>
TypeComparison<L, H> operator/(const TypeComparison<L, H>& _RH, const T& _LH)
{
    TypeComparison<L, H> _inL(_RH);
    TypeComparison<L, H> _inR(_LH);

    L val = _inL.m_value / _inR.m_value;
    H accurateVal = _inL.m_accurateValue / _inR.m_accurateValue;
    H error = std::abs((H)accurateVal - (H)val);
    H new_error = error + _inL.m_absoluteError + +_inR.m_absoluteError;
    return TypeComparison<L, H>(val, new_error, accurateVal);
}

template<typename L, typename H, typename T, typename = std::enable_if<std::is_arithmetic<T>::value>::type>
TypeComparison<L, H> operator/(const T& _LH, const TypeComparison<L, H>& _RH)
{
    TypeComparison<L, H> _inL(_RH);
    TypeComparison<L, H> _inR(_LH);

    L val = _inL.m_value / _inR.m_value;
    H accurateVal = _inL.m_accurateValue / _inR.m_accurateValue;
    H error = std::abs((H)accurateVal - (H)val);
    H new_error = error + _inL.m_absoluteError + +_inR.m_absoluteError;
    return TypeComparison<L, H>(val, new_error, accurateVal);
}



#ifndef INIT_FLOATID_COUNTER
unsigned int UIDCounterClass::UIDCounter = 0;
std::vector < std::pair<std::string, outputPrintingData> > UIDCounterClass::s_createdObjects;

#define INIT_FLOATID_COUNTER
#endif // !INIT_FLOATID_COUNTER

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
        std::cout << m_name.c_str() << " error was: " << m_error << "\n";
        std::cout << m_name.c_str() << " cumulative error was: " << GetCumulativeError() << "\n";
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

