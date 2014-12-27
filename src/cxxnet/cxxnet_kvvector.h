#pragma once
#include "parameter/kv_vector.h"
namespace PS {
namespace CXXNET {

template <typename K, typename V>
class CxxnetKVVector : public KVVector<K, V> {
  void setValue(const MessagePtr& msg) {
    int chl = msg->task.key_channel();
    int time = msg->task.time();
    if (time != last_time) {
      this->val_[chl].setZero();
    }

    KVVector<K,V>::setValue(msg);

    // we need to -1 here because the current task will be finished after
    // exitting this function
    if (this->taskpool(kWorkerGroup)->countIncomingTask(time) ==
        FLAGS_num_workers - 1) {
      // aggregated the gradients from all workers, allow for pulling now
      this->taskpool(kWorkerGroup)->finishIncomingTask(time+1);
    }
  }
 private:
  int last_time = -1;
};
} // namespace CXXNET
} // namespace PS
