#include "cxxnet/sync_worker.h"
#include "cxxnet/sync_data.h"
namespace PS {
namespace CXXNET {

void Sync::init(size_t size, std::string layer_name, std::string app_name) {
  size_ = size;
  auto ptr = new KVVector<SyncData::key_t, real_t>();
  sync_ = new SyncData(ptr, size);
  // continuous keys for dense model
  for (int i = 0; i < size; ++i) sync_->key[i] = i;

  // register the shared model to the system
  if (layer_name.empty()) layer_name = "layer_" + std::to_string(layer_id++);
  if (app_name.empty()) app_name = "cxxnet";
  REGISTER_CUSTOMER_WITH_PARENT(layer_name, sync_->data, app_name);
}

void Sync::push(real_t* data, bool no_copy = false) {
  // push the gradient to servers
  MessagePtr push(new Message(kServerGroup, time_ + 1));
  push->setKey(sync_->key);
  if (no_copy) {
    SArray<real_t> buf; buf.reset(data, size_);
    push->addValue(buf);
  } else {
    sync_->buf.copyFrom(data, size_);
    push->addValue(sync_->buf);
  }
  // to avoid send the keys multiple times
  push->addFilter(FilterConfig::KEY_CACHING);
  // ??
  sync_->data->value(0).setZero();
  sync_->data->push(push);

  // send the pull request earlier
  int max_delay = 0;  // TODO configable
  int wait_time = std::max(-1, time_ - max_delay * 3 + 2);
  // a server will reply this pull request only if *wait_time* is marked as
  // finished, which is used to control the data consistencycs
  MessagePtr pull(new Message(kServerGroup, time_+3, wait_time));
  pull->setKey(sync_->key);
  // to avoid send the keys multiple times
  pull->addFilter(FilterConfig::KEY_CACHING);
  time_ = sync_->data->pull(pull);
}

real_t* Sync::pull() {
  sync_->data->waitOutMsg(kServerGroup, time_);
  return sync_->data->value(0).data();
}

} // namespace CXXNET
} // namespace PS
