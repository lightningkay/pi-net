#ifndef __BOUNDEDBLOCKINGQUEUE_H
#define __BOUNDEDBLOCKINGQUEUE_H

#include <pi/base/Mutex.h>
#include <pi/base/Condition.h>

#include <boost/circular_buffer.hpp>
#include <boost/noncopyable.hpp>
#include <assert.h>

namespace pi
{
    template<typename T>
    class BoundedBlockingQueue : boost::noncopyable
    {
    public:
        explicit BoundedBlockdingQueue(int maxSize)
            : _mutex(),
              _notEmpty(mutex_),
              _notFull(mutex_),
              _queue(maxSize)
        {
        }

        void put(const T& x)
        {
            MutexLockGuard lock(_mutex);
            while (_queue.full())
            {
                _notFull.wait();
            }
            assert(!_queue.full());
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
            _notFull.notify();
            return T;
        }

        bool empty() const
        {
            MutexLockGuard lock(_mutex);
            return _queue.empty();
        }

        bool full() const
        {
            MutexLockGuard lock(_mutex);
            return _queue.full();
        }

        size_t size() const
        {
            MutexLockGuard lock(_mutex);
            return _queue.size();
        }
    private:
        mutable Mutex _mutex;
        Condition _notEmpty;
        Condition _notFull;
        boost::circular_buffer<T> _queue;
    };
}

#endif //__BOUNDEDBLOCKINGQUEUE_H
