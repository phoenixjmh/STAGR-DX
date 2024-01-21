#pragma once
#include <iostream>

namespace Logger
{
    template<typename T>
    void LogNext() {}


    template <typename T, typename... Additional>
    void LogNext(const T& single, const Additional&... next)
    {
        std::cout << single;
        if constexpr (sizeof...(next) > 0)
        {
            std::cout << " ";
            LogNext(next...);
        }
    }

    template <typename... Args>
    void Log(const Args&... args)
    {
        std::cout << "\n--------------------------------------------------------------\n";
        LogNext(args...);
        std::cout << "\n--------------------------------------------------------------\n";
    }


}

