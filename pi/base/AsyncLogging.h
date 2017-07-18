#ifndef PI_BASE_ASYNCLOGGING_H
#define PI_BASE_ASYNCLOGGING_H

#include <pi/base/BlockingQueue.h>
#include <pi/base/BoundedBlockingQueue.h>
#include <pi/base/CountDownLatch.h>
#include <pi/base/Mutex.h>
#include <pi/base/Thread.h>
#include <pi/base/LogStream.h>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/prt_container/ptr_vector.hpp>

namespace pi
{
    class AsyncLogging : boost::noncopyable
    {
    public:
        AsyncLogging(const string& basename,
                     size_t rollSize,
                     int flushInterval = 3);
        ~AsyncLogging()
        {
            if (running_)
            {
                stop();
            }
        }

        void append(const char* logline, int len);
        void start()
        {
            running_ = true;
            thread_.start();
            latch_.wait();
        }

        void stop()
        {
            running_ = false;
            cond_.notify();
            thread_.join();
        }

    private:
        AsyncLogging(const AsyncLogging&);
        void operator=(const AsyncLogging&);

        void threadFunc();

        typedef pi::detail::FixedBuffer<pi::detail::kLargeBuffer> Buffer;
        typedef boost::ptr_vector<Buffer> BufferVector;
        typedef BufferVector::auto_type BufferPtr;

        const int flushInterval_;
        bool running_;
        string basename_;
        size_t roolSize_;
        pi::Thread thread_;
        pi::CountDownLatch latch_;
        pi::MutexLock mutex_;
        pi::Condition cond_;
        BufferPtr currentBuffer_;
        BufferPtr nextBuffer_;
        BufferVector buffers_;
    };
}

#endif
