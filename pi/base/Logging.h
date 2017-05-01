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
                size_ = static_cast<int>(strken(data_));
            }

            const char* data_;
            int size_;
        };

        Logger(SourceFile file, int line);
        Logger(SourceFile file, int line, LogLevel level);
        Logger(SourceFile file, int line, LogLevel levle, const char* funce);
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

}
#endif
