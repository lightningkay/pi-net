#include <iostream>
#include <pthread.h>
#include <pi/base/Mutex.h>

int count = 0;
pi::MutexLock lock;

void * start(void * m)
{
    int j = *(int*)m;
    for (int i = 0; i < 10000; ++i)
    {
        pi::MutexLockGuard lockguard(lock);
        std::cout << "thread " << j << std::endl;
        count++;
        std::cout << "cout = " << count << std::endl;
    }
}

using std::cout;
using std::endl;
int main()
{
    cout << "Hello C++ !" << endl;
    pthread_t tid[2];
    int a[2] = {1, 2};
    for (int i = 0; i < 2; ++i)
        pthread_create(&tid[i], NULL, start, a + i);
    for (int i = 0; i < 2; ++i)
        pthread_join(tid[i], NULL);
    return 0;
}
