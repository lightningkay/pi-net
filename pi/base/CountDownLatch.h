#ifndef PI_BASE_COUNTDOWNLATCH_H
#define PI_BASE_COUNTDOWNLATCH_H

#include <pi/base/Mutex.h>
#include <pi/base/Condtion.h>

#include <boost/noncopyable.hpp>

namespace pi
{
    class CountDownLatch : boost::noncopyable
    {
    public:
        explicit CountDownLatch(int count);

        void wait();

        void countDown();

        void getCount() const;

    private:
        mutable MutexLock mutex_;
        Condition condition_;
        int count_;
    };
}

#endif
