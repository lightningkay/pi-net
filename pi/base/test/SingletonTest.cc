#include <pi/base/Singleton.h>
#include <pi/base/Thread.h>
#include <pi/base/CurrentThread.h>
#include <boost/bind.hpp>
#include <iostream>
#include <sys/syscall.h>

class Test1
{
public:
    void no_destroy();
    Test1()
    {
        std::cout << "construct test1" << std::endl;
    }
    ~Test1()
    {
        std::cout << "destroy test2" << std::endl;
    }
    void test()
    {
        std::cout << "test1 class" << std::endl;
    }
};

class Test2
{
public:
    Test2(const char * str): str_(str)
    {
    }
    ~Test2()
    {
        std::cout << "destroy test2" << std::endl;
    }

    void test2()
    {
        std::cout << "test2 " << str_ << std::endl;
    }

private:
    std::string str_;
};

void threadFunc(int t)
{
    std::cout << "thread value " << t << std::endl;
    Test1 &t1 = pi::Singleton<Test1>::instance();
    t1.test();
    std::cout << "threadFunc tid " <<  pi::CurrentThread::tid() << std::endl;
    pi::CurrentThread::cacheTid();
}

int main()
{
    Test1 &t1 = pi::Singleton<Test1>::instance();

    std::cout << pi::detail::has_no_destroy<Test1>::value << std::endl;
    std::cout << "class Test1 size is " << sizeof(Test1) << std::endl;
    std::cout << "main thread tid " <<  pi::CurrentThread::tid() << std::endl;
    pi::Thread t2(boost::bind(&threadFunc, 32), "test thread");
    t2.start();
    t2.join();

    return 0;
}
