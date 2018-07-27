#ifndef __BLOCKINGQUEUE_H
#define __BLOCKINGQUEUE_H
#include <pi/base/Condition.h>
#include <pi/base/Mutex.h>

#include <boost/noncopyable.hpp>
#include <deque>
#include <assert.h>

namespace pi
{
    template<typename T>
    class BlockingQueue : boost::noncopyable
    {
    public:
        BlockingQueue()
            : _mutex(),
              _notEmpty(_mutex()),
              _queue()
        {
        }

        void put(const T& x)
        {
            MutexLockGuard lock(_mutex);
            _queue.push_back(x);
            _notEmpty.notify();
        }

        T take()
        {
            MutexLockGuard lock(_mutex);

            while (_queue.empty())
            {
                _notEmpty.wait();
            }

            assert(!_queue.empty());
            T front(_queue.front());
            _queue.pop_front();
            return front;
        }

        size_t size() const
        {
            MutexLockGuard lock(_mutex);
            return _queue.size();
        }

    private:
        mutable Mutex _mutex;
        Condition _notEmpty;
        std::queue<T> _queue;
    };
}

#endif //__BLOCKINGQUEUE_H
