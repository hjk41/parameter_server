#pragma once
#include "cxxnet/cxxnet.h"
namespace PS {
namespace CXXNET {

class CxxnetScheduler : public Cxxnet {
 public:
  virtual void init() { Cxxnet::init(); }
  virtual void run();
  virtual void process(const MessagePtr& msg) { }
 private:
};

} // namespace CXXNET
} // namespace PS
