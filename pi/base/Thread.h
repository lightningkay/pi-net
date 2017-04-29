#ifndef PI_BASE_THREAD_H
#define PI_BASE_TRHEAD_H

#include <pi/base/Atomic.h>
#include <pi/base/Types.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>

namespace pi
{
    class Thread : boost::noncopyable
    {
    public:
        typedef boost::function<void ()> ThreadFunc;

        explicit Thread(const ThreadFunc&, const string& name = string());

        ~Thread();

        void start();
        int join();

        bool started() { return started_; }

        pid_t tid() const  { return *tid_; }
        const string& name() const { return name_; }
        static int numCreated() { return numCreated_.get(); }

    private:
        void setDefaultName();

        bool started_;
        bool joined_;
        pthread_t pthreadId_;
        boost::shared_ptr<pid_t> tid_;
        ThreadFunc func_;
        string name_;
        static AtomicInt32 numCreated_;
    };
}

#endif
