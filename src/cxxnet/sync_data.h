#pragma once
#include "mshadow/tensor.h"
#include "parameter/kv_vector.h"
namespace PS {
namespace CXXNET {

// put parameter server related data structure here to separate the compilation
// dependency
struct SyncData {
  // the key type can be only uint32 or uint64
  typedef uint32 key_t;
  typedef mshadow::real_t real_t;
  SyncData(KVVector<key_t, real_t>* ptr, size_t n)
      : data(ptr), keys(n), buf(n) { }
  KVVectorPtr<key_t, real_t> data;
  SArray<key_t> key;
  SArray<real_t> buf;
};

} // namespace CXXNET
} // namespace PS
