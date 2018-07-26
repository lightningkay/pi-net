#ifndef __MUTEX_H
#define __MUTEX_H

#include <pi/base/CurrentThread.h>
#include <boost/noncopyable.hpp>
#include <assert.h>
#include <pthread.h>

#ifdef CHECK_PTHREAD_RETURN_VALUE
#ifdef NDEBUG
    __BEGIN_DECLS
extern void __assert_perror_fail (int errnum,
                                  const char *file,
                                  unsigned int line,
                                  const char *function)
    __THROW __attribute__ ((__noreturn__));
    __END_DECLS
#endif

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);     \
        if (__builtin_expect(errnum != 0, 0))   \
        __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})
#else

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);     \
        assert(errnum == 0); (void) errnum;})

#endif

namespace pi
{
    class MutexLock : boost::noncopyable
    {
    public:
        MutexLock()
            : holder_(0)
        {
            MCHECK(pthread_mutex_init(&_mutex, NULL));
        }
        ~MutexLock()
        {
            assert(holder_ == 0);
            MCHECK(pthread_mutex_destroy(&_mutex));
        }

        bool isLockByThisThread() const
        {
            return holder_ == CurrentThread::tid();
        }

        void lock()
        {
            MCHECK(pthread_mutex_lock(&_mutex));
            assignHolder();
        }

        void unlock()
        {
            unassignHolder();
            MCHECK(pthread_mutex_unlock(&_mutex));
        }

        pthread_mutex_t* getPthreadMutex()
        {
            return &_mutex;
        }
    private:
        friend class Condition;

        class UnassignGuard : boost::noncopyable
        {
        public:
            UnassignGuard(MutexLock& owner)
                : _owner(owner)
            {
                _owner.unassignHolder();
            }

            ~UnassignGuard()
            {
                _owner.assignHolder();
            }
        private:
            MutexLock& _owner;
        };

        void unassignHolder()
        {
            holder_ = 0;
        }

        void assignHolder()
        {
            holder_ = CurrentThread::tid();
        }

        pthread_mutex_t _mutex;
        pid_t holder_;
    };

    class MutexLockGuard : boost::noncopyable
    {
    public:
        MutexLockGuard(MutexLock& mutex)
            : _mutex(mutex)
        {
            _mutex.lock();
        }
        ~MutexLockGuard()
        {
            _mutex.unlock();
        }
    private:
        MutexLock& _mutex;
    };
}
#endif //__MUTEX_H
