#pragma once
#include "mshadow/tensor.h"
namespace PS {
namespace CXXNET {

// a hacker
static int layer_id = 1;

struct SyncData;

typedef mshadow::real_t real_t;
class SyncWorker {
 public:
  void Init(size_t w_size, size_t b_size);
  void Push(real_t const* gwmat, real_t const* gbias);
  void Pull(real_t* gwmat, real_t* gbias);
 private:
  size_t w_size_, b_size_;
  SyncData* ps_;
  int last_pull_time_;
};

} // namespace CXXNET
} // namespace PS
