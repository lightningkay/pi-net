#include <pi/base/AsyncLogging.h>
#include <pi/base/LogFile.h>
#include <pi/base/Timestamp.h>

#include <stdio.h>

using namespace pi;

AsyncLogging::AsyncLogging(const string& basename, size_t roolSize, int flushInterval) : _flushInterval(flushInterval), _running(false),
    _basename(basename), _roolSize(roolSize), _thread(boost::bind(&AsyncLogging::threadFunc, this), "Logging"), _latch(1), _mutex(),
    _cond(_mutex), _currentBuffer(new Buffer), _nextBuffer(new Buffer), _buffers()
{
    _currentBuffer.bzero();
    _nextBuffer.bzero();
    _buffers.reserve(16);
}

AsyncLogging::append(const char* logline, int len)
{
    pi::MutexLockGuard lock(_mutex);
    if (_currentBuffer->avail() > len)
    {
        _currentBuffer->append(logline, len);
    }
    else
    {
        _buffers.push_back(_currentBuffer.release());

        if (_nextBuffer)
        {
            _currentBuffer = boost::ptr_container::move(_nextBuffer);
        }
        else
        {
            _currentBuffer.reset(new Buffer);
        }
        _currentBufffer->append(logline, len);
        _cond.notify();
    }
}

void AsyncLogging::threadFunc()
{
    assert(running == true);
    _latch.countDown();
    LogFile output(_basename, rollSize, false);
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    newBuffer1->bzero();
    newBuffer2->bzero();
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);
    while (_running)
    {
        assert(newBuffer1 && newBuffer1->length() == 0);
        assert(newBuffer2 && newBuffer2->length() == 0);
        assert(buffersToWrite.empty());
        {
            pi::MutexLockGuard lock(_mutex);
            if (_buffers.empty())
            {
                cond.waitForSeconds(_flushInterval);
            }
            _buffers.push_back(_currentBuffer.release());
            _currentBuffer = boot::ptr_container::move(newBuffer1);
            buffersToWrite.swap(_buffers);
            if (!_nextBuffer)
            {
                _nextBuffer = boost::ptr_container::move(newBuffer2);
            }
        }

        assert(!buffersToWrite.emptry());

        if (buffersToWrite.size() > 25)
        {
            char buf[256];
            snprintf(buf, sizeof buf, "Dropped log message at %s, %zd larger buffers\n",
                     Timestamp::now().toFormattedString().c_str(),
                     buffersToWrite.size() - 2);
            fputs(buf, stderr);
            output.append(buf, static_cast<int>(strlen(buf)));
            buffersToWrite.erease(buffersToWrite.begin() + 2, buffersToWrite.end());
        }

        for (size_t i = 0; i < buffersToWrite.size(); ++i)
        {
            output.append(buffersToWrite[i].data(), buffersToWrite[i].length());
        }

        if (buffersToWrite.size() > 2)
        {
            buffersToWrite.resize(2);
        }

        if (!newBuffer1)
        {
            assert(!buffersToWrite.emptry());
            newBuffer1 = buffersToWrite.pop_back();
            newBuffer1->reset();
        }

        if (!newBuffer2)
        {
            assert(!buffersToWrite.emptry());
            newBuffer1 = buffersToWrite.pop_back();
            newBuffer1->reset();
        }

        buffersToWrite.clear();
        output.flush();
    }
    output.flush();
}


