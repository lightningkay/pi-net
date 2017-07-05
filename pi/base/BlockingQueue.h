#ifndef PI_BASE_BLOCKINGQUEUE_H
#define PI_BASE_BLOCKINGQUEUE_H
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
            : mutex_(),
              notEmpty_(mutex_()),
              queue_()
        {
        }

        void put(const T& x)
        {
            MutexLockGuard lock(mutex_);
            queue_.push_back(x);
            notEmpty.notify();
        }

        T take()
        {
            MutexLockGuard lock(mutex_);

            while (queue_.empty())
            {
                notEmpty.wait();
            }

            assert(!queue_.empty());
            T front(queue_.front());
            queue_.pop_front();
            return front;
        }

        size_t size() const
        {
            MutexLockGuard lock(mutex_);
            return queue_.size();
        }

    private:
        mutable Mutex mutex_;
        Condition notEmpty_;
        std::queue<T> queue_;
    };
}

#endif
