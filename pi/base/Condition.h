#ifndef PI_BASE_CONDITION_H
#define PI_BASE_CONDITION_H

#include <pi/base/Mutex.h>

#include <pi/base/noncopyable.h>
#include <pthread.h>

namespace pi
{
    class Condition : boost::noncopyable
    {
    public:
        explicit Condition(MutexLock& mutex)
            : mutex_(mutex)
        {
            MCHECK(pthread_cond_init(&pcond_), NULL);
        }

        ~Condtion()
        {
            MCHECK(pthread_cond_destroy(&pcond_));
        }

        void wait()
        {
            MutexLock::UnassignGuard ug(mutex_);
            MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()));
        }

        void waitForSeconds(double seconds);

        void notify()
        {
            MCHECK(pthread_cond_signal(&pcond_));
        }

        void notifyAll()
        {
            MCHECK(pthread_cond_broadcast(&pcond_));
        }
    private:
        MutexLock& mutex_;
        pthread_cond_t pcond_;
    };
}
#endif
