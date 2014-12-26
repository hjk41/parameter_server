#pragma once
#include "mshadow/tensor.h"
#include "parameter/kv_vector.h"
namespace PS {
namespace CXXNET {

// put parameter server related data structure here, because it requires c++11
// features, which is not supported by low version cuda
struct SyncData {
  KVVectorPtr<int, mshadow::real_t> sync_;
  SArray<int> keys_;
  SArray<real_t> buf_;
};

} // namespace CXXNET
} // namespace PS
