#include "process_lock.h"
#include <sys/file.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include <thread>
#include <sys/mman.h>

ProcessLock::ProcessLock() {

}

bool ProcessLock::Init(const char* filename) {
  fd_ = open(filename, O_CREAT | O_RDWR, 0777);
  if (fd_ < 0) {
    printf("Error: open filed.\n");
    return false;
  }
  mutex_ = (pthread_mutex_t *)mmap(0, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
  if (mutex_ == nullptr) {
    printf("Error: mmap filed.");
    return false;
  }
  pthread_mutexattr_init(&mutex_attr_);
  pthread_mutexattr_setpshared(&mutex_attr_, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(mutex_, &mutex_attr_);
  return true;
}

ProcessLock::~ProcessLock() {
  pthread_mutexattr_destroy(&mutex_attr_);
  if (mutex_) {
    pthread_mutex_destroy(mutex_);
    munmap(mutex_, sizeof(pthread_mutex_t));
  }
  if (fd_) {
    close(fd_);
  }
}

void ProcessLock::Lock() {
  pthread_mutex_lock(mutex_);
}

void ProcessLock::Unlock() {
  pthread_mutex_unlock(mutex_);
}

