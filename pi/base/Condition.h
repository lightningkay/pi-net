#ifndef __CONDITION_H
#define __CONDITION_H

#include <pi/base/Mutex.h>

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace pi
{
    class Condition : boost::noncopyable
    {
    public:
        explicit Condition(MutexLock& mutex) : _mutex(mutex)
        {
            MCHECK(pthread_cond_init(&_pond, NULL));
        }

        ~Condition()
        {
            MCHECK(pthread_cond_destroy(&_pcond));
        }

        void wait()
        {
            MutexLock::UnassignGuard ug(_mutex);
            MCHECK(pthread_cond_wait(&_pcond, _mutex.getPthreadMutex()));
        }

        bool waitForSeconds(double seconds);

        void notify()
        {
            MCHECK(pthread_cond_signal(&_pcond));
        }

        void notifyAll()
        {
            MCHECK(pthread_cond_broadcast(&_pcond));
        }

    private:
        MutexLock& _mutex;
        pthread_cond_t _pcond;
    };
}

#endif // __CONDITION_H
