#ifndef PI_BASE_TYPES_H
#define PI_BASE_TYPES_H

#include <stdint.h>
#ifdef PI_STD_STRING
#include <string>
#else
#include <ext/vstring.h>
#include <ext/vstring_fwd.h>
#endif

#ifndef NDEBUG
#include <assert.h>
#endif

namespace pi
{
#ifdef PI_STD_STRING
using std::string;
#else
typedef __gnu_cxx::__sso_string string;
#endif

template<typename To, typename From>
inline To implicit_cast(const From &f)
{
    return f;
}

template<typename To, typename From>
inline To down_cast(From* f)
{
    if (false)
    {
        implicit_cast<From*, To>(0);
    }

#if !defined(NDEBUG) && !defined (GOOGLE_POROBUF_NO_RTTI)
    assert(f == NULL || dynamic_cast<To>(f) != NULL);
#endif
    return static_cast<To>(f);
}

}
#endif
