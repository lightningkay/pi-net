#include <pi/base/Timestamp.h>
#include <iostream>
#include <time.h>
#include <assert.h>

using namespace pi;

int main()
{
    Timestamp now = Timestamp::now();
    std::cout << now.toString() << std::endl;
    std::cout << now.toFormattedString() << std::endl;

    time_t t = time(NULL);
    std::cout << "time_t = " << t << std::endl;
    std::cout << "time_t size = " << sizeof(t) << std::endl;
    Timestamp start_1000us = Timestamp::fromUnixTime(t, -1000);
    std::cout << start_1000us.toString() << std::endl;
    std::cout << start_1000us.toFormattedString() << std::endl;
    assert(start_1000us < now);
    std::cout << "difference is " << timeDifference(start_1000us, now) << std::endl;

    return 0;
}
