#pragma once
#include<string>
#include<Windows.h>
#include <random>
#include <chrono>
#include <sys/stat.h>



class UtilityFuncs
{
public:
    static std::string unicodeToMultibyte(const std::wstring& wstr);
    static std::wstring MultibyteToWideChar(const std::string& str);
    static float GenerateRandomFloat(float r1, float r2);   //r1,r2 = range
    static bool Check_File_Exist(const std::string& name);
    static bool Check_File_Exist(const std::wstring& name);
};



class GenerateRandomFloatClass
{
public:
    GenerateRandomFloatClass()
    {
    };

    float generateRandom(float r1, float r2)
    {
        std::uniform_real_distribution<float> distribution(r1, r2);
        return distribution(gen);
    }

    std::default_random_engine gen;

};