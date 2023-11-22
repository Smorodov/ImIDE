#pragma once
#include <iostream>
#include <iomanip>
#include <thread>
#include <functional>
#include <memory>
#include <mutex>
#include <chrono>
#include <vector>
#include <algorithm>
#include <locale>
#include <codecvt>
#ifdef _WIN32
#include <windows.h>
#endif
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h" // support for loading levels from the environment variable
#include <ctime>

#include "MainWindow.h"

// https://i.voenmeh.ru/kafi5/Kam.loc/inform/UTF-8.htm
// convert UTF-8 string to wstring
inline std::wstring utf8_to_wstring(const std::string& str)
{
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
inline std::string wstring_to_utf8(const std::wstring& str)
{
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

inline void toUpper(std::string& str)
{
    transform(
        str.begin(), str.end(),
        str.begin(),
        toupper);
}

inline void setRussianConsole()
{
    // for russian console output in spdlog
    setlocale(LC_ALL, "ru_RU.UTF8");
    setlocale(LC_NUMERIC, "C");
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
}

std::chrono::steady_clock::time_point t_start;                    // Ќачало измерени€ времени
std::chrono::steady_clock::time_point t_finish;                   // конец измерени€ времени
