#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <thread>
#include "lib_func.h"


pthread_t pid;
std::thread *test_thread;


void *ThreadEntry(void *arg) {
    for (uint32_t i = 0; i < 100000; i++) {
        TestLevel1();
    }
    return 0;
}





//void CreateTestThread() {
//  if (-1 == pthread_create(&pid, 0, ThreadEntry, 0)) {
//      printf("pthread_create error!\n");
//      return;
//  }
//}

void CreateTestThread() {
  test_thread = new std::thread(ThreadEntry, nullptr);
}

void KillTestThread() {
  // pthread_kill(pid, SIGUSR1);
  pthread_kill(test_thread->native_handle(), SIGUSR1);
}
