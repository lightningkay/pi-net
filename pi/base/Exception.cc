#include <pi/base/Exception.h>

using namespace pi;

Exception::Exception(const char* msg)
    : message_(msg)
{
    fillStackTrace();
}

Exception::Exception(const string& msg)
    : message_(msg)
{
    fillStackTrace();
}

Exception::~Exception() throw()
{
}

cosnt char* Exception::what() const throw()
{
    return message_.c_str();
}

const char* Exception::stackTrace() const throw()
{
    return stack_.c_str();
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
            stack_.append(strings[i]);
            stack_.push_back('\n');
        }
        free(strings);
    }
}
