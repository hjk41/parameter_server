#pragma once
#include <string>
#include "mshadow/tensor.h"
namespace PS {
namespace CXXNET {

struct SyncData;
typedef mshadow::real_t real_t;

// the synchronization interface for cxxnet workers. only support dense model now
class Sync {
 public:
  Sync() : sync_(NULL), time_(-1) { }

  // size: model size, which is often weight_size + bias_size
  // app_name: the application name, such as imagenet_alex_net.
  // layer_name: the unique name for a layer
  void init(size_t size, std::string layer_name, std::string app_name);

  // push the gradient to servers. if *no_copy* is true, then the system will
  // take the ownership of *data* (nobody else can read and write *data*) and
  // delete *data* when pushing is finished.
  void push(real_t* data, bool no_copy = false);

  // return the pulled data
  real_t* pull();

 private:
  size_t size_;
  SyncData* sync_;
  int time_;
  int last_pull_time_;
};

// a hacker to generate the layer ids
static int layer_id = 1;

} // namespace CXXNET
} // namespace PS
