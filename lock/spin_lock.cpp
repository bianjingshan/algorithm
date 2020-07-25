#include "spin_lock.h"


SpinLock::SpinLock()
{
    flag_.clear();
}

SpinLock::~SpinLock()
{
}

void SpinLock::lock()
{
    while(flag_.test_and_set()) {}
}

void SpinLock::unlock()
{
    flag_.clear();
}
