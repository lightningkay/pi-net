#ifndef PI_BASE_BOUNDEDBLOCKINGQUEUE_H
#define PI_BASE_BOUNDEDBLOCKINGQUEUE_H

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
            : mutex_(),
              notEmpty_(mutex_),
              notFull_(mutex_),
              queue_(maxSize)
        {
        }

        void put(const T& x)
        {
            MutexLockGuard lock(mutex_);
            while (queue_.full())
            {
                notFull_.wait();
            }
            assert(!queue_.full());
            queue_.push_back(x);
            notEmpty_.notify();
        }

        T take()
        {
            MutexLockGuard lock(mutex_);
            while (queue_.empty())
            {
                notEmpty_.wait();
            }
            assert(!queue_.empty());
            T front(queue_.front());
            queue_.pop_front();
            notFull_.notify();
            return T;
        }

        bool empty() const
        {
            MutexLockGuard lock(mutex_);
            return queue_.empty();
        }

        bool full() const
        {
            MutexLockGuard lock(mutex_);
            return queue_.full();
        }

        size_t size() const
        {
            MutexLockGuard lock(mutex_);
            return queue_.size();
        }
    private:
        mutable Mutex mutex_;
        Condition notEmpty_;
        Condition notFull_;
        boost::circular_buffer<T> queue_;
    };
}

#endif
