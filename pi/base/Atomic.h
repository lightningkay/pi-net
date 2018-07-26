#ifndef __ATOMIC_H
#define __ATOMIC_H

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
            AtomicIntegerT() :_value(0)
            {
            }

            T get()
            {
                // gcc >= 4.7
                // return __atomic_load_n(&_value, __ATOMIC_SEQ_CST);
                return __sync_val_compare_and_swap(&_value, 0, 0);
            }
            T getAndAdd(T x)
            {
                // gcc >= 4.7
                // return __atomic_fetch_add(&_value, x, __ATOMIC_SEQ_CST);
                return __sync_fetch_and_add(&_value, x);
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
                return __sync_lock_test_and_set(&_value, newValue);
            }
        private:
            volatile T _value;
        };
    }
    typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
    typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
}

#endif //__ATOMIC_H
