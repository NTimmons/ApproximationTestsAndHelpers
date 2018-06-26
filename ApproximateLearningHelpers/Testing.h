#pragma once

template<typename T>
struct ErrorDetails
{
    ErrorDetails(T min, T max,
        long double minRoundingErr, long double maxRoundingErr,
        long double minCumulativeErr, long double maxCumulativeErr)
        : _minRoundingErr(minRoundingErr), _maxRoundingErr(maxRoundingErr),
        _minCumulativeErr(minCumulativeErr), _maxCumulativeErr(maxCumulativeErr),
        _min(min), _max(max) {}

    T _min;
    T _max;
    long double _minRoundingErr;
    long double _maxRoundingErr;
    long double _minCumulativeErr;
    long double _maxCumulativeErr;

    static std::string GetTitles()
    {
        std::stringstream ss;
        ss << "min					" << "," <<
            "max					" << "," <<
            "minRoundingErr			" << "," <<
            "maxRoundingErr			" << "," <<
            "minCumulativeErr		" << "," <<
            "maxCumulativeErr		" << "," <<
            "minPercentageRound		" << "," <<
            "maxPercentageRound		" << "," <<
            "minPercentageCumulative" << "," <<
            "maxPercentageCumulative" << "\n";

        return ss.str();
    }

    std::string ToString()
    {
        long double _minPercentageRound = _minRoundingErr / (long double)(_max - _min);
        long double _maxPercentageRound = _maxRoundingErr / (long double)(_max - _min);
        long double _minPercentageCumulative = _minCumulativeErr / (long double)(_max - _min);
        long double _maxPercentageCumulative = _maxCumulativeErr / (long double)(_max - _min);

        std::stringstream ss;
        ss << std::setprecision(16) <<
            _min << "," <<
            _max << "," <<
            _minRoundingErr << "," <<
            _maxRoundingErr << "," <<
            _minCumulativeErr << "," <<
            _maxCumulativeErr << "," <<
            _minPercentageRound << "," <<
            _maxPercentageRound << "," <<
            _minPercentageCumulative << "," <<
            _maxPercentageCumulative << "";

        return ss.str();
    }
};