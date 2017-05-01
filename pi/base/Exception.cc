#include <pi/base/Exception.h>

#include <execinfo.h>
#include <stdlib.h>

using namespace pi;

Exception::Exception(const string& what)
    : message_(what)
{
    fillStackTrace();
}

Exception::Exception(const char* what)
    : message_(what)
{
    fillStackTrace();
}
Exceptoin::~Exception() throw()
{
}

cosnt char* Exception::what()
{
    return message_.c_str();
}

const char* stackTrace()
{
    return stack_.c_str();
}

void Exceptoin::fillStackTrace()
{
    const int len = 200;
    void* buffer[len];
    int nptrs = ::backtrace(buffer, len);
    char** strings = ::backtrace_symbols(buffer, len);
    if (strings)
    {
        for (int i = 0; i < nptrs; ++i)
        {
            stack_.append(strings[i]);
            stack_.push_back('\n');
        }
        free(strings);
    }
}
