//
// Created by wen on 2016-04-26.
//

#ifndef QTP_FRAMEWORK_SINGLETON_H
#define QTP_FRAMEWORK_SINGLETON_H

#include "spin_lock.h"

namespace qtp {

template<typename T>
class Singleton : public SpinLock {
public:
  static T& Instance() { //it's thread-safe in c++11
    static T t;
    return t;
  }
protected:
  Singleton(void) {}
  ~Singleton(void) {}

private:
  Singleton(const Singleton& rhs);
  Singleton& operator=(const Singleton& rhs);
};

}

#endif //QTP_FRAMEWORK_SINGLETON_H
