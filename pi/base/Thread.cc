#include <pi/base/Thread.h>
#include <pi/base/CurrentThread.h>
#include <pi/base/Exception.h>
#include <pi/base/Logging.h>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/weak_ptr.hpp>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace pi
{
    namespace CurrentThread
    {
        __thread int t_cachedTid = 0;
        __thread char t_tidString[32];
        __thread int t_tidStringLength = 6;
        __thread const char* t_threadName = "unknown";
        const bool sameType = boost::is_same<int, pid_t>::value;
        BOOST_STATIC_ASSERT(sameType);
    }

    namespace detail
    {
        pid_t gettid()
        {
            return static_cast<pid_t>(::syscall(SYS_gettid));
        }

        void afterFork()
        {
            pi::CurrentThread::t_cachedTid = 0;
            pi::CurrentThread::t_threadName = "main";
            CurrentThread::tid();
        }

        class ThreadNameInitializer
        {
            public:
                ThreadNameInitializer()
                {
                    pi::CurrentThread::t_threadName = "main";
                    pi::CurrentThread::tid();
                    pthread_atfork(NULL, NULL, &afterFork);
                }
        };

        ThreadNameInitializer init;

        struct ThreadData
        {
            typedef pi::Thread::ThreadFunc ThreadFunc;
            ThreadFunc _func;
            string _name;
            pid_t* _tid;
            CountDownLatch* _latch;

            ThreadData(const ThreadFunc& func,
                    const string &name,
                    pid_t* tid,
                    CountDownLatch* latch)
                : _func(func),
                  _name(name),
                  _tid(tid),
                  _latch(latch)
            {  }

            void runInThread()
            {
                *_tid = pi::CurrentThread::tid();
                _tid = NULL;
                _latch->countDown();
                _latch = NULL;

                pi::CurrentThread::t_threadName = _name.empty() ? "muduoThread" : _name.c_str();
                ::prctl(PR_SET_NAME, pi::CurrentThread::t_threadName);

                try
                {
                    _func();
                    pi::CurrentThread::t_threadName = "finished";
                }
                catch (const Exception& ex)
                {
                    pi::CurrentThread::t_threadName = "crashed";
                    fprintf(stderr, "exception caught in Thread %s\n", _name.c_str());
                    fprintf(stderr, "reason: %s\n", ex.what());
                }
                catch (...)
                {
                    pi::CurrentThread::t_threadName = "crashed";
                    fprintf(stderr, "unknown exception caught in Thread %s\n", _name.c_str());
                    throw;
                }
            }

        };

        void* startThread(void *obj)
        {
            ThreadData* data = static_cast<ThreadData*>(obj);
            data->runInThread();
            delete data;
            return NULL;
        }
    }
}

using namespace pi;
void CurrentThread::cacheTid()
{
    if (t_cachedTid == 0)
    {
        t_cachedTid = detail::gettid();
        t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
    }
}

bool CurrentThread::isMainThread()
{
    return tid() == ::getpid();
}

void CurrentThread::sleepUsec(int64_t usec)
{
    struct timespec ts = {0, 0};
    ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicroSecondsPerSecond * 1000);
    ::nanosleep(&ts, NULL);
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc& func, const string& n)
    : _started(false),
      _joined(false),
      _pthreadId(0),
      _tid(0),
      _func(func),
      _name(n),
      _latch(1)
{
    setDefaultName();
}

Thread::~Thread()
{
    if (_started && !_joined)
    {
        pthread_detach(_pthreadId);
    }
}

void Thread::setDefaultName()
{
    int num = _numCreated.incrementAndGet();
    if (_name.empty())
    {
        char buf[32];
        snprintf(buf, sizeof buf, "Thread%d", num);
        _name = buf;
    }
}

void Thread::start()
{
    assert(!_started);
    _started = true;
    detail::ThreadData* data = new detail::ThreadData(_func, _name, &_tid, &_latch);
    if (pthread_create(&_pthreadId, NULL, &detail::startThread, data))
    {
        _started = false;
        delete data;
        LOG_SYSFATAL << "Faied in pthread_create";
    }
}

int Thread::join()
{
    assert(_started);
    assert(!_joined);
    _joined = true;
    return pthread_join(_pthreadId, NULL);
}
