#include <pi/base/Condition.h>

#include <error.h>

bool pi::Condition::waitForSeconds(double seconds)
{
    struct tiemspec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);

    const int64_t kNanoSecondsPerSecons = 1e9;
    int64_t nanseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecons);
    abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanseconds) / kNanoSecondsPerSecons);
    abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanseconds) % kNanoSecondsPerSecons);
    MutexLock::UnassignGuard ug(_mutex);
    return ETIMEOUT == pthread_cond_timewait(&_pcond, _mutex.getPthreadMutex(), abstime);
}
