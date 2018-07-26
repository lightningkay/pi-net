#include <pi/base/AsyncLogging.h>
#include <pi/base/LogFile.h>
#include <pi/base/Timestamp.h>

#include <stdio.h>

using namespace pi;

AsyncLogging::AsyncLogging(const string& basename,
                           size_t roolSize,
                           int flushInterval)
    : flushInterval_(flushInterval)
      running_(false),
      basename_(basename),
      roolSize_(roolSize),
      thread_(boost::bind(&AsyncLogging::threadFunc, this), "Logging"),
      latch(1),
      mutex_(),
      cond_(mutex_),
      currentBuffer_(new Buffer),
      nextBuffer_(new Buffer),
      buffers_()
{
    currentBuffer_.bzero();
    nextBuffer_.bzero();
    buffers_.reserve(16);
}

AsyncLogging::append(const char* logline, int len)
{
    pi::MutexLockGuard lock(mutex_);
    if (currentBuffer_->avail() > len)
    {
        currentBuffer_->append(logline, len);
    }
    else
    {
        buffers_.push_back(currentBuffer_.release());

        if (nextBuffer_)
        {
            currentBuffer_ = boost::ptr_container::move(nextBuffer_);
        }
        else
        {
            currentBuffer_.reset(new Buffer);
        }
        currentBufffer_->append(logline, len);
        cond_.notify();
    }
}

void AsyncLogging::threadFunc()
{
    assert(running == true);
    latch_.countDown();
    LogFile output(basename_, rollSize, false);
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    newBuffer1->bzero();
    newBuffer2->bzero();
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);
    while (running_)
    {
        assert(newBuffer1 && newBuffer1->length() == 0);
        assert(newBuffer2 && newBuffer2->length() == 0);
        assert(buffersToWrite.empty());
        {
            pi::MutexLockGuard lock(mutex_);
            if (buffers_.empty())
            {
                cond_.waitForSeconds(flushInterval_);
            }
            buffers_.push_back(currentBuffer_.release());
            currentBuffer_ = boot::ptr_container::move(newBuffer1);
            buffersToWrite.swap(buffers_);
            if (!nextBuffer_)
            {
                nextBuffer_ = boost::ptr_container::move(newBuffer2);
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


