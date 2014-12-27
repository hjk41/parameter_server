#include "cxxnet/sync_worker.h"
#include "cxxnet/sync_data.h"
namespace PS {
namespace CXXNET {

void SyncWorker::init(size_t w_size, size_t b_size) {
  w_size_ = w_size;
  b_size_ = b_size;

  auto n = w_size + b_size;

  auto p = new KVVector<uint32, real_t>();
  ps_ = new SyncData(p, n);
  string name = "fc" + std::to_string(layer_id++);
  // TODO I should read the name from the configure file
  REGISTER_CUSTOMER_WITH_PARENT(name, ps_->sync_, "mnist_fullc");
  for (int i = 0; i < n; ++i) ps_->keys_[i] = i;
  last_pull_time_ = -1;
}
void SyncWorker::push(real_t const* gwmat, real_t const* gbias) {
  // push the gradient to servers
  MessagePtr push(new Message(kServerGroup, last_pull_time_+1));
  push->setKey(ps_->keys_);

  // copy the data, implement zero-copy in the future
  memcpy(ps_->buf_.data(), gwmat, w_size_ * sizeof(real_t));
  if (gbias) {
    memcpy(ps_->buf_.data() + w_size_, gbias, b_size_ * sizeof(real_t));
  }
  push->addValue(ps_->buf_);
  ps_->sync_->value(0).setZero();
  ps_->sync_->push(push);

  // pull the weight
  MessagePtr pull(new Message(
      kServerGroup, last_pull_time_+3, last_pull_time_+2));
  pull->setKey(ps_->keys_);
  last_pull_time_ = ps_->sync_->pull(pull);
}

void SyncWorker::pull(real_t* gwmat, real_t* gbias) {
  ps_->sync_->waitOutMsg(kServerGroup, last_pull_time_);
  auto global_grad = ps_->sync_->value(0);

  memcpy(gwmat, global_grad.data(), w_size_ * sizeof(real_t));
  if (gbias) {
    memcpy(gbias, global_grad.data() + w_size_, b_size_ * sizeof(real_t));
  }
}

} // namespace CXXNET
} // namespace PS
