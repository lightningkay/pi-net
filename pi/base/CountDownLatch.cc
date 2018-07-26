#include <pi/base/CountDownLatch.h>

pi::CountDownLatch(int count) : _mutex(), _condtion(_mutex), _count(count)
{
}

void pi::CountDownLatch::wait()
{
    MutexLockGuard lock(_mutex);
    while (_count > 0)
    {
        _cond.wait();
    }
}

void pi::CountDownLatch::countDown()
{
    MutexLockGuard lock(_mutex);
    if (--_count == 0)
        _condition.notifyAll();
}

int getCount() const
{
    MutexLockGuard lock(_mutex);
    return _count;
}
