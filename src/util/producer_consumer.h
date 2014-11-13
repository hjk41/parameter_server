#pragma once
#include "util/threadsafe_limited_queue.h"
#include "util/common.h"
namespace PS {

template<class V>
class ProducerConsumer {
 public:
  ProducerConsumer() { setCapacity(1000); }
  ProducerConsumer(int capacity_in_mb) { setCapacity(capacity_in_mb); }
  void setCapacity(int mb) { queue_.setMaxCapacity(mb*1000000); }
  void setFinished() { done_ = true; }

  // return false if finished, true otherwise
  void startProducer(const std::function<bool(V*, size_t*)>& func) {
    producer_thr_ = std::thread([this, func](){
        V entry;
        while (!done_) {
          size_t size = 0;
          if (!func(&entry, &size)) done_ = true;
          if (size > 0) queue_.push(entry, size);
        }
      });
    producer_thr_.detach();
  }

  void startConsumer(const std::function<void(const V&)>& func) {
    consumer_thr_ = std::thread([this, func](){
        V entry;
        while (pop(&entry)) {
          func(entry);
        }
      });
    // consumer_thr_.detach();
  }
  void waitConsumer() { consumer_thr_.join(); }
  bool pop(V* data) {
    if (done_ && queue_.empty()) return false;
    queue_.pop(*data);
    return true;
  }
  void push(const V& entry, size_t size = 1) {
    queue_.push(entry, size);
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(ProducerConsumer);
  ThreadsafeLimitedQueue<V> queue_;
  bool done_ = false;
  std::thread producer_thr_;
  std::thread consumer_thr_;
};
} // namespace PS