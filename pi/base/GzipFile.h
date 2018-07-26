#ifndef PI_BASE_GZIPFILE_H
#define PI_BASE_GZIPFILE_H

#include <pi/base/StringPiece>
#include <boost/noncopyable.hpp>
#include <zlib.h>

namespace pi
{
    class GzipFile : boost::noncopyable
    {
    public:
        GzipFile(GzipFile&& rhs)
            : file_(rhs.file_)
        {
            rhs.file_ = NULL;
        }

        ~GzipFile()
        {
            if (file_)
            {
                ::gzclose(file_);
            }
        }

        GzipFile& operator=(GzipFile&& rhs)
        {
            swap(rhs);
            return *this;
        }

        bool valid() const { return file_ != NULL; }
        void swap(GzipFile& rhs) { std::swap(file_, rhs.file_); }

#if ZLIB_VERNUM >= 0x1240
        bool setBuffer(int size) { return ::gzbuffer(file_, size) == 0; }
#endif
        int read(void* buf, int len) { return ::gzread(file_, buf, len); }
        int write(StringPiece buf) { return ::gzwrite(file_, buf.data(), buf.size()); }
        off_t tell() const { return ::gztell(file_); }

#if ZLIB_VERNUM >= 0x1240
        off_t offset() const { return ::gzoffset(file_); }
#endif

        static GzipFile openForRead(StringArg filename)
        {
            return GzipFile(::gzopen(filename.c_str(), "rbe"));
        }

        static GzipFile openForAppend(StringArg filename)
        {
            return GzipFile(::gzopen(filename.c_str(), "abe"));
        }

        static GzipFile openForWriteExclusive(StringArg filename)
        {
            return GzipFile(::gzopen(filename.c_str(), "wbxe"));
        }

        static GzipFile openForWriteTruncate(StringArg filename)
        {
            return GzipFile(::gzopen(filename.c_str(), "wbe"));
        }

    private:
        explicit GzipFile(gzFile file)
            : file_(file)
        {
        }

        gzFile file_;
    };
}
#endif
