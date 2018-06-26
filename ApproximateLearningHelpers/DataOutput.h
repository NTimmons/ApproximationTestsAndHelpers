#pragma once
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>

std::string SanitiseFilename(std::string _string)
{
    std::replace(_string.begin(), _string.end(), ':', '_');
    return _string;
}

void ClearFile(std::string _name)
{
    std::ofstream ofs;
    ofs.open(_name.c_str(), std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

void WriteTextFile(std::string _string, std::string _filename)
{
    std::ofstream csvFile;
    csvFile.open(_filename.c_str(), std::ofstream::out | std::ofstream::trunc);

    csvFile << _string;

    std::cout << "Wrote " << _filename.c_str() << " \n";
}

template<typename T, typename U>
void WriteCSV2Var(std::vector<T> _inputVectorX, std::vector<U> _inputVectorY, std::string _name)
{
    std::ofstream csvFile;
    csvFile.open(_name.c_str(), std::ofstream::out | std::ofstream::trunc);
    for (unsigned int i = 0; i < _inputVectorX.size(); i++)
        csvFile << _inputVectorX[i] << ", " << _inputVectorY[i] << ",\n";

    csvFile << "\n";
    std::cout << "Wrote " << _name.c_str() << " \n";
}

template<typename T>
void WriteCSVPrecise(std::vector<T> _inputVectorX, std::vector<T> _inputVectorY, std::string _name)
{
    std::ofstream csvFile;
    csvFile.open(_name.c_str(), std::ofstream::out | std::ofstream::trunc);
    for (unsigned int i = 0; i < _inputVectorX.size(); i++)
        csvFile << std::setprecision(100) << _inputVectorX[i] << ", " << _inputVectorY[i] << ",\n";

    csvFile << "\n";
    std::cout << "Wrote " << _name.c_str() << " \n";
}

template<typename T>
void WriteCSVPrecise(std::vector<std::pair<T, T>> inputPair, std::string _name)
{
    std::ofstream csvFile;
    csvFile.open(_name.c_str(), std::ofstream::out | std::ofstream::trunc);
    csvFile << std::setprecision(100) << "Input" << ", " << "Results" << "\n";
    for (unsigned int i = 0; i < inputPair.size(); i++)
        csvFile << std::setprecision(100) << inputPair[i].first << ", " << inputPair[i].second << "\n";

    std::cout << "Wrote " << _name.c_str() << " \n";
}

void WriteLog(std::string _msg, std::string _name)
{
    std::ofstream csvFile;
    csvFile.open(_name.c_str(), std::ofstream::out | std::ofstream::app);

    csvFile << _msg.c_str() << "\n";
    std::cout << "Wrote " << _name.c_str() << " \n";
}

