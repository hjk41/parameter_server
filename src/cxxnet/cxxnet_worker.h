#pragma once
#include "cxxnet/cxxnet.h"
namespace PS {
namespace CXXNET {

class CxxnetWorker : public Cxxnet {
 public:
  virtual void init() { Cxxnet::init(); }
  virtual void process(const MessagePtr& msg) {
    auto cmd = get(msg).cmd();
    if (cmd == Call::RUN) {
      char* args[2];
      int n = conf_.args().size();
      args[1] = new char[n];
      memcpy(args[1], conf_.args().data(), n*sizeof(char));
      cxxnet::CXXNetLearnTask tsk;
      tsk.Run(2, args);
    }
  }
};

} // namespace CXXNET
} // namespace PS
