#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

#define THREAD_NUM 10
uint64_t g_num = 0;

void threadEntry(int n) {
    for (int i = 0; i < n; ++i) {
        g_num++;
    }
}

void test() {
    g_num = 0;
    std::thread *th[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) {
        th[i] = new std::thread(threadEntry, 10000);
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        th[i]->join();
    }
    printf("test g_num: %lu\n", g_num);
}

std::mutex g_mtx;
void threadEntryMutex(int n) {
    for (int i = 0; i < n; ++i) {
        g_mtx.lock();
        g_num++;
        g_mtx.unlock();
    }
}

void testMutex() {
    g_num = 0;
    std::thread *th[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) {
        th[i] = new std::thread(threadEntryMutex, 10000);
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        th[i]->join();
    }
    printf("testMutex g_num: %lu\n", g_num);
}

void threadEntryMutexLockGuard(int n) {
    for (int i = 0; i < n; ++i) {
        std::lock_guard<std::mutex> lock(g_mtx);
        g_num++;
    }
}

void testMutexLockGuard() {
    g_num = 0;
    std::thread *th[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) {
        th[i] = new std::thread(threadEntryMutexLockGuard, 10000);
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        th[i]->join();
    }
    printf("testMutexLockGuard g_num: %lu\n", g_num);
}

std::atomic<uint64_t> g_num_atomic(0);

void threadEntryAtomic(int n) {
    for (int i = 0; i < n; ++i) {
        g_num_atomic++;
    }
}

void testAtomic() {
    g_num_atomic = 0;
    std::thread *th[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) {
        th[i] = new std::thread(threadEntryAtomic, 10000);
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        th[i]->join();
    }
    uint64_t temp = g_num_atomic;
    printf("testAtomic g_num_atomic: %lu\n", temp);
}





int main()
{
    printf("\n");
    test();
    testMutex();
    testMutexLockGuard();
    testAtomic();
    printf("\n\n");
    return 0;
}
