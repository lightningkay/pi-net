#include <iostream>
#include <pi/base/Atomic.h>

using namespace pi;

int main()
{
	{
		AtomicInt32 a;
		std::cout << a.get() << std::endl;
		for (int i = 0; i < 32; ++i)
			a.increment();
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
