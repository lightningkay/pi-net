#ifndef PI_BASE_ATOMIC_H
#define PI_BASE_ATOMIC_H

#include <boost/noncopyable.hpp>
#include <stdint.h>

namespace pi
{
    namespace detail
    {
        template<typename T>
            class AtomicIntegerT : boost::noncopyable
        {
            public:
                AtomicIntegerT()
                    :value_(0)
                {
                }

                T get()
                {
                    return __atomic_load_n(&value_, __ATOMIC_SEQ_CST);
                }
                T getAndAdd(T x)
                {
                    return __atomic_fetch_add(&value_, x, __ATOMIC_SEQ_CST);
                }
                T addAndGet(T x)
                {
                    return getAndAdd(x) + x;
                }
                T incrementAndGet()
                {
                    return addAndGet(1);
                }
                T decrementAndGet()
                {
                    return addAndGet(-1);
                }
                void Add(T x)
                {
                    getAndAdd(x);
                }
                void increment()
                {
                    incrementAndGet();
                }
                void decrement()
                {
                    decrementAndGet();
                }
                T getAndSet(T newValue)
                {
                    return __sync_lock_test_and_set(&value_, newValue);
                }
            private:
                volatile T value_;
        };
    }
    typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
    typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
}

#endif //PI_BASE_ATOMIC_H
