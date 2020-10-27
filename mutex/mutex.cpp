
#include <stdio.h>
#include <stdlib.h>
#include <mutex>
#include <math.h>
#include <map>
#include <algorithm>
#include <sys/time.h>
using namespace std;


class ClassA {
 public:
    ClassA() {
        
    }
    ~ClassA() {

    }
    void test() {
        printf("test\n");
    }
};

double CurrentTimeMsec(void) {
    timeval m_tv;
    gettimeofday(&m_tv, NULL);
    return (double)m_tv.tv_sec * 1000.0 + (double)m_tv.tv_usec / 1000.0;
}


void MutexTest(void) {
    double start_time, end_time;
    start_time = CurrentTimeMsec();
    int iloop = 0;
    while (1) {
        iloop++;
        if (iloop > 1000000) {
            break;
        }
    }
    end_time = CurrentTimeMsec();
    printf("Run: %.6f ms\n", end_time - start_time);

    std::mutex test_lock;
    start_time = CurrentTimeMsec();
    iloop = 0;
    while (1) {
        std::lock_guard<std::mutex> l(test_lock);
        iloop++;
        if (iloop > 1000000) {
            break;
        }
    }
    end_time = CurrentTimeMsec();
    printf("Run with mutex: %.6f ms\n", end_time - start_time);
}

uint32_t g_num = 0;
std::mutex g_mutex;
typedef void *(*StartRoutine) (void *);

void *threadEntry(void *arg)
{
    for (uint32_t i = 0; i < 100000; i++)
    {
        g_num++;
    }
    return NULL;
}

void *threadEntryLockGuard(void *arg)
{
    for (uint32_t i = 0; i < 100000; i++)
    {
        std::lock_guard<std::mutex> l(g_mutex);
        g_num++;
    }
    return NULL;
}

void *threadEntryUniqueLock(void *arg)
{
    for (uint32_t i = 0; i < 100000; i++)
    {
        std::unique_lock<std::mutex> l(g_mutex);
        g_num++;
    }
    return NULL;
}

void *threadEntryLockGuardAndUniqueLock(void *arg)
{
    uint64_t i = (uint64_t)arg;
    bool lock_guard = ((i % 2) == 0);

    for (uint32_t i = 0; i < 100000; i++)
    {
        if (lock_guard)
        {
            std::lock_guard<std::mutex> l(g_mutex);
            g_num++;
        }
        else
        {
            std::unique_lock<std::mutex> l(g_mutex);
            g_num++;
        }
    }
    return NULL;
}

void ThreadMutexTest(uint32_t option) {
#define THREAD_NUM 16
    StartRoutine test_entry;
    switch (option)
    {
    case 0:
        test_entry = threadEntry;
        break;
    case 1:
        test_entry = threadEntryLockGuard;
        break;
    case 2:
        test_entry = threadEntryUniqueLock;
        break;
    case 3:
        test_entry = threadEntryLockGuardAndUniqueLock;
        break;
    default:
        break;
    }

    g_num = 0;
    pthread_t pid[THREAD_NUM];
    double start_time, end_time;
    start_time = CurrentTimeMsec();
    for (uint64_t i = 0; i < THREAD_NUM; i++)
    {
        if (-1 == pthread_create(&pid[i], NULL, test_entry, (void *)i))
        {
            printf("pthread_create error!\n");
            return;
        }
    }
    for (uint32_t i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(pid[i], NULL))                 
        {
            printf("pthread_join error!\n");
            return;
        }
    }
    end_time = CurrentTimeMsec();
    printf("Run thread %u: %.6f ms, %u\n", option, end_time - start_time, g_num);

}

int main(int argc, char *argv[]) {
    MutexTest();

    ThreadMutexTest(0);
    ThreadMutexTest(1);
    ThreadMutexTest(2);
    ThreadMutexTest(3);
    return 0;
}