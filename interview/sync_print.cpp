#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>


void threadA(int n) {
    for (int i = 0; i < n; ++i) {
        printf("A");
    }
}

void threadB(int n) {
    for (int i = 0; i < n; ++i) {
        printf("B");
    }
}

void test() {
    std::thread *th[2];
    th[0] = new std::thread(threadA, 100);
    th[1] = new std::thread(threadB, 100);

    for (int i = 0; i < 2; i++) {
        th[i]->join();
    }
}

std::mutex mtx_a;
std::mutex mtx_b;

void threadASync(int n) {
    for (int i = 0; i < n; ++i) {
        mtx_a.lock();
        printf("A");
        mtx_b.unlock();
    }
}

void threadBSync(int n) {
    for (int i = 0; i < n; ++i) {
        mtx_b.lock();
        printf("B");
        mtx_a.unlock();
    }
}

void testSync() {
    mtx_b.lock();
    std::thread *th[2];
    th[0] = new std::thread(threadASync, 100);
    th[1] = new std::thread(threadBSync, 100);

    for (int i = 0; i < 2; i++) {
        th[i]->join();
    }
}




int main()
{
    printf("\n");
    test();
    printf("\n\n");
    testSync();
    printf("\n\n");
    return 0;
}
