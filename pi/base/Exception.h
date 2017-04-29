#ifndef PI_BASE_EXCEPTION_H
#define PI_BASE_EXCEPTION_H

#include <muduo/base/Types.h>
#include <exception>

namespace pi
{
    class Exception : public std::exception
    {
    public:
        explicit Exception(const char* what);
        explicit Exception(const string& what);
        virtual ~Exception() throw();
        virtrual const char* what() const throw();
        const char* stackTrace() cosnt throw();

    private:
        void fillStackTrace();

        string message_;
        string stack_;
    };
}

#endif
