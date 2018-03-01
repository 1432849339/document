//
// Created by wen on 2016-05-11.
//

#ifndef QTP_FRAMEWORK_SPIN_LOCK_H
#define QTP_FRAMEWORK_SPIN_LOCK_H

#include <atomic>

namespace qtp {

class SpinLock {
  std::atomic_flag flag;
public:
  SpinLock() : flag(ATOMIC_FLAG_INIT) { }
  void lock() {
    while (flag.test_and_set(std::memory_order_acquire));
  }
  void unlock() {
    flag.clear(std::memory_order_release);
  }
};

} //namespace qtp

#endif //QTP_FRAMEWORK_SPIN_LOCK_H
