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

        bool started() { return _started; }

        pid_t tid() const  { return _tid; }
        const string& name() const { return _name; }
        static int numCreated() { return _numCreated.get(); }

    private:
        void setDefaultName();

        bool _started;
        bool _joined;
        pthread_t _pthreadId;
        pid_t _tid;
        ThreadFunc _func;
        string _name;
        CountDownLatch _latch;

        static AtomicInt32 _numCreated;
    };
}

#endif
