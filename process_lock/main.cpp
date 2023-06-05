#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include "process_lock.h"

ProcessLock process_lock;

void ThreadEntry(int n) {
  process_lock.Lock();
  printf("thread-%u start\n", n);
  sleep(10);
  printf("thread-%u stop\n", n);
  process_lock.Unlock();
}


int main() {
  auto start_time = time(0);
  //process_lock.Init("./tmp_lock");
  process_lock.Init("/dev/zero");
  #define THREAD_NUM 1
  std::thread *th[THREAD_NUM];
  for (int i = 0; i < THREAD_NUM; i++) {
    th[i] = new std::thread(ThreadEntry, i);
  }

  for (int i = 0; i < THREAD_NUM; i++) {
    th[i]->join();
  }
  auto end_time = time(0);
  printf("Cost time : %lu\n", end_time - start_time);
  return 0;
}
