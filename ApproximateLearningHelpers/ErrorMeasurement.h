#pragma once

template< typename T>
class ErrorPropagationType
{
public:
    ErrorPropagationType()
    {
        m_data = 0;
        m_error = 1;
    }

    ErrorPropagationType(T _in)
    {
        m_data = _in;
        m_error = 1;
    }

    ErrorPropagationType(T _in, long double _error)
    {
        m_data = _in;
        m_error = _error;
    }

    T& Get()
    {
        return m_data;
    }

    long double& GetError()
    {
        return m_error;
    }

    T	m_data;
    long double m_error;
};

template< typename T>
ErrorPropagationType<T> operator+(const ErrorPropagationType<T>& _lhs, const ErrorPropagationType<T>& _rhs)
{
    T data = _lhs.m_data + _rhs.m_data;

    double erL = (_lhs.m_error);
    double erR = (_rhs.m_error);
    double error = sqrt((erL*erL) + (erR*erR));

    return ErrorPropagationType<T>(data, error);
}

template< typename T>
ErrorPropagationType<T> operator-(const ErrorPropagationType<T>& _lhs, const ErrorPropagationType<T>& _rhs)
{
    T data = _lhs.m_data - _rhs.m_data;

    double erL = (_lhs.m_error);
    double erR = (_rhs.m_error);
    double error = sqrt((erL*erL) + (erR*erR));

    return ErrorPropagationType<T>(data, error);
}

template< typename T>
ErrorPropagationType<T> operator*(const ErrorPropagationType<T>& _lhs, const ErrorPropagationType<T>& _rhs)
{
    T data = _lhs.m_data * _rhs.m_data;

    double erL = (_lhs.m_error / _lhs.m_data);
    double erR = (_rhs.m_error / _rhs.m_data);
    double error = data * (sqrt((erL*erL) + (erR*erR)));

    return ErrorPropagationType<T>(data, error);
}

template< typename T>
ErrorPropagationType<T> operator/(const ErrorPropagationType<T>& _lhs, const ErrorPropagationType<T>& _rhs)
{
    T data = _lhs.m_data / _rhs.m_data;

    double erL = (_lhs.m_error / _lhs.m_data);
    double erR = (_rhs.m_error / _rhs.m_data);
    double error = data * (sqrt((erL*erL) + (erR*erR)));

    return ErrorPropagationType<T>(data, error);
}