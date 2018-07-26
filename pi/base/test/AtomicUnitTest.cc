#include <iostream>
#include <pi/base/Atomic.h>
#include <assert.h>

using namespace pi;

int main()
{
    {
        AtomicInt32 a;
        std::cout << a.get() << std::endl;
        a.increment();
        assert(a.get() == 1);
        assert(a.getAndAdd(5) == 1);
        assert(a.get() == 6);
        std::cout << a.get() << std::endl;
    }

    {
        AtomicInt64 b;
        std::cout << b.get() << std::endl;
        for (int i = 0; i < 32; ++i)
            b.increment();
        std::cout << b.get() << std::endl;
    }
}
