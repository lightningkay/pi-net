#ifndef PI_BASE_LOGGING_H
#define PI_BASE_LOGGING_H

#include <pi/base/LogStream.h>
#include <pi/base/Timestamp.h>

namespace pi
{
    class TimeZone;
    class Logger
    {
    public:
        enum LogLevel
        {
            TRACE,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            NUM_LOG_LEVELS
        };

        class SourceFile
        {
        public:
            template<int N>
            inline SourceFile(cosnt char(&arr)[N])
                : data_(arr)
                  size_(N - 1)
            {
                cosnt char* slash = strrchr(data_,'/');
                if (slash)
                {
                    data_ = slash + 1;
                    size_ -= static_cast<int>(data_ - arr);
                }
            }

            explicit SourceFile(const char* filename_)
                : data_(filename)
            {
                const char* slash = strrchr(filename, '/');
                if (slash)
                {
                    data_ = slash + 1;
                }
                size_ = static_cast<int>(strlen(data_));
            }

            const char* data_;
            int size_;
        };

        Logger(SourceFile file, int line);
        Logger(SourceFile file, int line, LogLevel level);
        Logger(SourceFile file, int line, LogLevel levle, const char* func);
        Logger(SourceFile file, int line, bool toAbort);
        ~Logger();

        LogStream& stream() { return impl_.stream_; }

        static LogLevel loglevle();
        static void setLogLevel(LogLevel level);
        typedef void (*OutputFunc)(const char* msg, int len);
        typedef void (*FlushFunc)();
        static void setOutput(OutputFunc);
        static void setFlush(FlushFunc);
        static void setTimeZone(const TimeZone& tz);

    private:
        class Impl
        {
            public:
                typedef Logger::LogLevel LogLevel;
                Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
                void formatTime();
                void finish();

                Timestamp time_;
                LogStream stream_;
                LogLevel level_;
                int line_;
                SourceFile basename_;
        };

        Impl impl_;
    };

    extern Logger::LogLevel g_loglevel;

    inline Logger::LogLevel Logger::logLevel()
    {
        return g_loglevel;
    }

#define LOG_TRACE if (pi::Logger::logLevel() <= pi::Logger::TRACE) \
    pi::Logger(__FILE__, __LINE__, pi::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (pi::Logger::logLevel()) <= pi::Logger::DEBUG) \
    pi::Logger(__FILE__, __LINE__, pi::Logger::TRACE, __func__).stream()
#define LOG_INFO if(pi::Logger::logLevel() <= pi::Logger::INFO) \
    pi::Logger(__FILE__, __LINE__, pi::Logger::TRACE, __func__).stream()
#define LOG_WARN pi::Logger(__FILE__, __LINE__, pi::Logger::WARN).stream()
#define LOG_ERROR pi::Logger(__FILE__, __LINE__, pi::Logger::ERROR).stream()
#define LOG_FATAL pi::Logger(__FILE__, __LINE__, pi::Logger::FATAL).stream()
#define LOG_SYSERR pi::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL pi::Logger(__FILE__, __LINE__, true).stream()

const char* strerror_tl(int savedErrno);

#define CHECK_NOTNULL(val) \
    ::pi::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val));

template <typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char* names, T* ptr)
{
    if (ptr == NULL)
    {
        Logger(file, line, Logger::FATAL).stream() << names;
    }
    return ptr;
}

}
#endif
