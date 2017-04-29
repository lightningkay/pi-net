#include <iostream>
#include <pthread.h>
#include <pi/base/Mutex.h>

void CurrentThread::cacheTid()
{
  if (t_cachedTid == 0)
  {
    t_cachedTid = detail::gettid();
    t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
  }
}


int count = 0;
pi::MutexLock lock;

void * start(void *)
{
    for (int i = 0; i < 50; ++i)
    {
        pi::MutexLockGuard lockguard(lock);
        count++;
        std::cout << "cout = " << count << std::endl;
    }
}

using namespace std;

int main()
{
    cout << "Hello C++ !" << endl;
    pthread_t tid[2];
    for (int i = 0; i < 2; ++i)
        pthread_create(&tid[i], NULL, start, NULL);
    for (int i = 0; i < 2; ++i)
        pthread_join(tid[i], NULL);
    return 0;
}
