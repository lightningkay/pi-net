#ifndef PI_BASE_THREADPOOL_H
#define PI_BASE_THREADPOOL_H

#include <pi/base/Condition.h>
#include <pi/base/Mutex.h>
#include <pi/base/Thread.h>
#include <pi/base/Types.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <deque>

namespace pi
{

class ThreadPool : boost::noncopyable
{
public:
    typedef boost::function<void ()> Task;

    explicit ThreadPool(const string& nameArg = string("ThreadPool"));
    ~ThreadPool();

    void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
    void setThreadInitCallback(const Task& cb)
    { threadInitCallback_ = cb; }

    void start(int numThreads);
    void stop();

    const string& name() const
    { return name_; }

    size_t queueSize() const;

    void run(const Task& f);

private:
    bool isFull() const;
    void runInThread();
    Task take();

    mutable MutexLock mutex_;
    Condition notEmpty_;
    Condition notFull_;
    string name_;
    Task threadInitCallback_;
    boost::ptr_vector<muduo::Thread> threads_;
    std::deque<Task> queue_;
    size_t maxQueueSize_;
    bool running_;
};

}
#endif
