#ifndef PI_BASE_SINGLETON_H
#define PI_BASE_SINGLETON_H
#include <boost/noncopyable.h>
#include <assert.h>
#include <stdlib.h>
#include <pthread.h>

namespace pi
{
    namespace detail
    {
        template <typename T>
        struct has_no_destroy
        {
            template <typename C> static char test(typeof(&C:no_destroy));
            template <typename C> static int32_t test(...);
            const static bool value = sizeof(test<T>(0) == 1);
        };
    }

    template <typename T>
    class Singleton : boost::noncopyable
    {
    public:
        static T& instance()
        {
            pthread_once(&ponce_, &Singleton::init);
            assert(value != NULL);
            return *value_;
        }
    private:
        Singleton();
        ~Singleton();

        static void init()
        {
            value_ = new T();
            if (!detail::has_no_destroy<T>::value)
            {
                atexit(destroy);
            }
        }
        static void destroy()
        {
            typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1: 1] ;
            T_must_be_complete_type dummy; (void) dummy;
            delete value_;
            value_ = NULL;
        }

        static pthread_once_t ponce_;
        static T* value_;
    };

    template <typename T>
    pthread_once_t ponce_ = PTHREAD_ONCE_INIT;

    template <typename T>
    T* value_ = NULL;
}

#endif

