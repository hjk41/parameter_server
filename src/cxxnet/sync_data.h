#pragma once
#include "mshadow/tensor.h"
#include "parameter/kv_vector.h"
namespace PS {
namespace CXXNET {

// put parameter server related data structure here, because it requires c++11
// features, which is not supported by low version cuda
struct SyncData {
  // the key type can be only uint32 or uint64
  SyncData(KVVector<uint32, mshadow::real_t>* ptr, size_t n)
      : sync_(ptr), keys_(n), buf_(n) { }
  KVVectorPtr<uint32, mshadow::real_t> sync_;
  SArray<uint32> keys_;
  SArray<real_t> buf_;
};

} // namespace CXXNET
} // namespace PS
