#ifndef __ASYNCLOGGING_H
#define __ASYNCLOGGING_H

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
        AsyncLogging(const string& basename, size_t rollSize, int flushInterval = 3);
        ~AsyncLogging()
        {
            if (_running)
            {
                stop();
            }
        }

        void append(const char* logline, int len);
        void start()
        {
            _running = true;
            _thread.start();
            _latch.wait();
        }

        void stop()
        {
            running_ = false;
            _cond.notify();
            _thread.join();
        }

    private:
        AsyncLogging(const AsyncLogging&);
        void operator=(const AsyncLogging&);

        void threadFunc();

        typedef pi::detail::FixedBuffer<pi::detail::kLargeBuffer> Buffer;
        typedef boost::ptr_vector<Buffer> BufferVector;
        typedef BufferVector::auto_type BufferPtr;

        const int _flushInterval;
        bool _running;
        string _basename;
        size_t _roolSize;
        pi::Thread _thread;
        pi::CountDownLatch _latch;
        pi::MutexLock _mutex;
        pi::Condition _cond;
        BufferPtr _currentBuffer;
        BufferPtr _nextBuffer;
        BufferVector _buffers;
    };
}

#endif //__ASYNCLOGGING_H
