#include <pi/base/Exception.h>

#include <execinfo.h>
#include <stdlib.h>

using namespace pi;

Exception::Exception(const char* what)
    : _message(what)
{
    fillStackTrace();
}

Exception::Exception(const string& what)
    : _message(what)
{
    fillStackTrace();
}

Exception::~Exception() throw()
{
}

const char* Exception::what() const throw()
{
    return _message.c_str();
}

const char* Exception::stackTrace() const throw()
{
    return _stack.c_str();
}

void Exception::fillStackTrace()
{
    const int len = 200;
    void* buffer[len];
    int nptrs = ::backtrace(buffer, len);
    char** strings = ::backtrace_symbols(buffer, nptrs);
    if (strings)
    {
        for (int i = 0; i < nptrs; ++i)
        {
            _stack.append(strings[i]);
            _stack.push_back('\n');
        }
        free(strings);
    }
}
