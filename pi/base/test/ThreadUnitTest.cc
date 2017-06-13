#include <pi/base/Thread.h>
#include <iostream>

void testThrfunc1()
{
    std::cout << "test thread func 1" << std::endl;
}

int main()
{
    pi::Thread t1(testThrfunc1, "test");
    std::cout << t1.name() << std::endl;
    std::cout << "start" << std::endl;
    t1.start();
    t1.join();
    std::cout << t1.name() << std::endl;
    std::cout << "end" << std::endl;
    return 0;
}
