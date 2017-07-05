#ifndef PI_BASE_FILEUTIL_H
#define PI_BASE_FILEUTIL_H

#include <pi/base/StringPiece.h>
#include <boost/noncopyable.hpp>

namespace pi
{
    namespace FileUtil
    {
        class ReadSmallFile : boost::noncopyable
        {
        public:
            ReadSmallFile(StringArg filename);
            ~ReadSmallFile();

            template<typename String>
            int readToString(int maxSize,
                             String* content,
                             int64_t* fileSize,
                             int64_t* modifyTime,
                             int64_t* createTime);

            int readToBuffer(int* size);

            const char* buffer() const { return buf_; }

            static const int kBufferSize = 64*1024;
        private:
            int fd_;
            int err_;
            char buf_[kBufferSize];
        };

        template<typename String>
        int readFile(StringArg filename,
                     int maxSize,
                     String* content,
                     int64_t* fileSize = NULL,
                     int64_t* modifyTime = NULL,
                     int64_t* createTime = NULL)
        {
            ReadSmallFile file(filename);
            return file.readToString(maxSize, content, filesize, modifyTime, createTime);
        }

        class Appendfile : boost::noncopyable
        {
        public:
            explicit Appendfile(StringArg filename);

            ~Appendfile();

            void append(const char* logline, const size_t len);

            void flush();

            size_t writtenBytes() const { return writtenBytes_; }

        private:
            size_t write(const char* logline, size_t len);

            FILE *p_;
            char buffer_[64*1024];
            size_t writtenBytes_;
        };
    }
}
#endif
