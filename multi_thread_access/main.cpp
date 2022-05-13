#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <thread>
#include <mutex>

#define LOOP_NUM 10000000000

uint64_t g_num1 = 0;
uint64_t g_num2 = 0;


void ThreadWrite1() {
    for (uint64_t i = 0; i < LOOP_NUM; ++i) {
        g_num1++;
    }
}

void ThreadWrite2() {
    for (uint64_t i = 0; i < LOOP_NUM; ++i) {
        g_num2++;
    }
}

void ThreadRead() {
    int64_t result;
    while (1) {
        result = g_num1 - g_num2;
        printf("%-16lu%-16lu%-16ld\n", g_num1, g_num2, result);
        if ((g_num1 >= LOOP_NUM) && (g_num2 >= LOOP_NUM)) {
            break;
        }
    }
}

int main()
{
    std::thread *th[3];
    th[0] = new std::thread(ThreadWrite1);
    th[1] = new std::thread(ThreadWrite2);
    th[2] = new std::thread(ThreadRead);
    for (int i = 0; i < 3; i++) {
        th[i]->join();
    }
    printf("\n\n");
    return 0;
}
