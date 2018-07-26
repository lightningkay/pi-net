#ifndef __COUNTDOWNLATCH_H
#define __COUNTDOWNLATCH_H

#include <pi/base/Condition.h>
#include <pi/base/Mutex.h>

#include <boost/noncopyable.hpp>

namespace pi
{
    class CountDownLatch : boost::noncopyable
    {
    public:
        explicit CountDownLatch(int count);

        void wait();

        void countDown();

        int getCount() const;

    private:
        mutable MutexLock _mutex;
        Condition _condition;
        int _count;
    }
}
#endif
