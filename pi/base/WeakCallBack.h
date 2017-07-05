#ifndef PI_BASE_WEAKCALLBACK_H
#define PI_BASE_WEAKCALLBACK_H

#include <functional>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace pi
{

#ifdef __GXX_EXPERIMENTAL_CXX0X__
    template<typename CLASS, typename...ARGS>
    class WeakCallBack
    {
    public:
        WeakCallBack(const boost::weak_ptr<CLASS>& object,
                     const boost::function<void (CLASS*, ARGS...)>& function)
            :object_(object), function_(function)
        {
        }

        void operater()(ARGS&&... args) const
        {
            boost::shared_ptr<CLASS> ptr(object_.lock());
            if (ptr)
            {
                function_(ptr.get(), std::forward<ARGS>(args)...);
            }
        }
    private:
        boost::weak_ptr<CLASS> object_;
        boost::function<void (CLASS*, ARGS...) function_;
    };

    template<typename CLASS, typename...ARGS>
    WeakCallBack<CLASS, ARGS...> makeWeakCallBack(const boost::shared_ptr<CLASS>& object,
                                         void (CLASS::* function)(ARGS...))
    {
        return WeakCallBack<CLASS, ARGS...>(object, function);
    }

    template<typename CLASS, typename...ARGS>
   WeakCallBack<CLASS, ARGS...> makeWeakCallBack(const boost::shared_ptr<CLASS>& object,
                                        void (CLASS::* function)(ARGS...))
   {
       return WeakCallBack<CLASS, ARGS...>(object, function);
   }
}

#else
    template<typename CLASS>
    class WeakCallBack
    {
    public:
        WeakCallBack(const boost::weak_ptr<CLASS>& object,
                     const boost::function<void (CLASS*)>& function)
            :object_(object), function_(function)
        {
        }

        void operater()() const
        {
            boost::shared_ptr<CLASS> ptr(object_.lock());
            if (ptr)
            {
                function_(ptr.get());
            }
        }
    private:
        boost::weak_ptr<CLASS> object_;
        boost::function<void (CLASS*) function_;
    };

    template<typename CLASS>
    WeakCallBack<CLASS> makeWeakCallBack(const boost::shared_ptr<CLASS>& object,
                                         void (CLASS::* function)())
    {
        return WeakCallBack<CLASS>(object, function);
    }

    template<typename CLASS>
   WeakCallBack<CLASS> makeWeakCallBack(const boost::shared_ptr<CLASS>& object,
                                        void (CLASS::* function))
   {
       return WeakCallBack<CLASS>(object, function);
   }
}

#endif

