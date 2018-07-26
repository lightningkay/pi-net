#include <pi/base/ThreadPool.h>

#include <pi/base/Exception.h>

#include <boost/bind.hpp>
#include <assert.h>
#include <stdio.h>

using namespace pi;

ThreadPool::ThreadPool(const string& nameArg)
    : mutex_(),
      notEmptey_(mutex_),
      notFull_(mutex_),
      name_(nameArg),
      maxQueueSize_(0),
      running_(false)
{
}

ThreadPool::~ThreadPool()
{
    if (running_)
    {
        stop();
    }
}
