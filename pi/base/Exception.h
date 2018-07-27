#ifndef PI_BASE_EXCEPTION_H
#define PI_BASE_EXCEPTION_H

#include <pi/base/Types.h>
#include <exception>

namespace pi
{
    class Exception : public std::exception
    {
    public:
        explicit Exception(const char* what);
        explicit Exception(const string& what);
        virtual ~Exception() throw();
        virtual const char* what() const throw();
        const char* stackTrace() const throw();

    private:
        void fillStackTrace();

        string _message;
        string _stack;
    };
}
#endif
