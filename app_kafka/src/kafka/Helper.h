#pragma once
#include <iostream>
#include <chrono>

namespace kafka
{
    class Helper
    {
    public:
        static long long int getTimeInNanoseconds();

        static long long int getTimeInMiliseconds();

        static long long int getTimeInMicroseconds();

        static long long int getTimeInSeconds();

        static void holdSeconds(int secs);

        static void holdMiliseconds(int miliseconds);
    };
}
