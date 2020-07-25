#ifndef SPIN_LOCK_H_
#define SPIN_LOCK_H_

#include <atomic>

class SpinLock {
 public:
    SpinLock();
    ~SpinLock();
    void lock();
    void unlock();

 private:
    std::atomic_flag flag_;
};

#endif  // SPIN_LOCK_H_


