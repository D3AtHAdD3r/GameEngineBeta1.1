#include"utilityFunctions.h"


std::string UtilityFuncs::unicodeToMultibyte(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::wstring UtilityFuncs::MultibyteToWideChar(const std::string& str)
{
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

float UtilityFuncs::GenerateRandomFloat(float r1, float r2)
{
    std::mt19937_64 rng;
    // initialize the random number generator with time-dependent seed
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
    rng.seed(ss);
    // initialize a uniform distribution between 0 and 1
    std::uniform_real_distribution<float> unif(r1, r2);
    float currentRandomNumber = unif(rng);
    return currentRandomNumber;
}

bool UtilityFuncs::Check_File_Exist(const std::string& name)
{
    struct _stat64 buffer;
    return (_stat64(name.c_str(), &buffer) == 0);
}

bool UtilityFuncs::Check_File_Exist(const std::wstring& name)
{
    struct _stat64 buffer;
    return (_wstat64(name.c_str(), &buffer) == 0);
}
