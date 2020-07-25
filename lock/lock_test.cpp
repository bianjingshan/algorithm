#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include "spin_lock.h"

uint64_t g_num = 0;
std::mutex g_mtx;
SpinLock g_spin_lock;
std::atomic_flag flag_;

void threadEntryNolock(int n) {
    for (int i = 0; i < n; ++i) {
        g_num++;
    }
}

void threadEntryMutex(int n) {
    for (int i = 0; i < n; ++i) {
        g_mtx.lock();
        g_num++;
        g_mtx.unlock();
    }
}

void threadEntryMutexLockGuard(int n) {
    for (int i = 0; i < n; ++i) {
        std::lock_guard<std::mutex> lock(g_mtx);
        g_num++;
    }
}

void threadEntrySpinLock(int n) {
    for (int i = 0; i < n; ++i) {
        g_spin_lock.lock();
        g_num++;
        g_spin_lock.unlock();
    }
}

void threadEntryAtomicFlag(int n) {
    for (int i = 0; i < n; ++i) {
        flag_.test_and_set();
        g_num++;
        flag_.clear();
    }
}


uint64_t currentTimeUsec(void) {
    timeval tv;
    gettimeofday(&tv, NULL);
    return ((uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)tv.tv_usec);
}

typedef void (*thread_entry_t)(int n);

void test(thread_entry_t thread_entry, const char *test_name) {
#define THREAD_NUM 10
    g_num = 0;
    uint64_t start_time;
    uint64_t end_time;
    start_time = currentTimeUsec();
    std::thread *th[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) {
        th[i] = new std::thread(thread_entry, 1000000);
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        th[i]->join();
    }
    end_time = currentTimeUsec();
    printf("\n");
    printf("%s g_num: %lu\n", test_name, g_num);
    printf("%s time cost: %lu us\n", test_name, end_time - start_time);
}


int main()
{
    test(threadEntryNolock, "nolock");
    test(threadEntryMutex, "mutex");
    test(threadEntryMutexLockGuard, "mutex_lock_guard");
    test(threadEntrySpinLock, "spin_lock");
    test(threadEntryAtomicFlag, "atomic_flag");
    printf("\n\n");
    return 0;
}
