//
// Created by wen on 2016-04-26.
//

#ifndef QTP_FRAMEWORK_LOCK_QUEUE_H
#define QTP_FRAMEWORK_LOCK_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

namespace qtp {

template<typename T>
class LockQueue {
private:
  mutable std::mutex mut;
  std::queue<T> data_queue;
  std::condition_variable data_cond;
public:
  LockQueue() {
  }

  void Push(const T& new_value) {
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(new_value);
    data_cond.notify_one();
  }

  T WaitAndPop() {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
    T res = data_queue.front();
    data_queue.pop();
    return res;
  }

  T TryPop() {
    std::lock_guard<std::mutex> lk(mut);
    if (data_queue.empty())
      return nullptr;
    T res = data_queue.front();
    data_queue.pop();
    return res;
  }

  bool IsEmpty() const {
    std::lock_guard<std::mutex> lk(mut);
    return data_queue.empty();
  }
};

} //namespace qtp

#endif //QTP_FRAMEWORK_LOCK_QUEUE_H
