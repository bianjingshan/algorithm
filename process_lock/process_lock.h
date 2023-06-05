#ifndef PROCESS_LOCK_H_
#define PROCESS_LOCK_H_

#include <pthread.h>



class ProcessLock {
 public:
    ProcessLock();
    ~ProcessLock();
    bool Init(const char* filename);
    void Lock();
    void Unlock();

 private:
    int fd_;
    pthread_mutex_t *mutex_ = nullptr;
    pthread_mutexattr_t mutex_attr_;
};


#endif  // PROCESS_LOCK_H_


