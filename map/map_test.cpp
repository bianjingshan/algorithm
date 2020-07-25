#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <map>
#include "hash_map.h"

#define ITEM_NUM 100
uint64_t keys[ITEM_NUM];
uint64_t values[ITEM_NUM];
std::map<uint64_t, uint64_t> g_map;
HashMap hash_map;

void setStdMap(uint64_t key, uint64_t value) {
    g_map[key] = value;
}

void threadEntryStdMap(int n) {
    uint64_t temp_value;
    for (int i = 0; i < n; ++i) {
        temp_value = g_map.at(keys[n % ITEM_NUM]);
    }
}

void threadEntryHashMap(int n) {
    uint64_t temp_value;
    for (int i = 0; i < n; ++i) {
        hash_map.get(keys[n % ITEM_NUM], &temp_value);
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
    printf("%s time cost: %lu us\n", test_name, end_time - start_time);
}


int main()
{
    hash_map.init(ITEM_NUM);
    for (int i = 0; i < ITEM_NUM; i++) {
        keys[i] = rand();
        values[i] = rand();
        setStdMap(keys[i], values[i]);
        hash_map.set(keys[i], values[i]);
    }
    test(threadEntryStdMap, "std::map");
    test(threadEntryHashMap, "hash_map");
    printf("\n\n");
    return 0;
}
