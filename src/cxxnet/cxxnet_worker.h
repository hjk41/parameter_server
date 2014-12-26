#pragma once
#include "cxxnet/cxxnet.h"
#include "sync/sync_ps-inl.h"
namespace PS {
namespace CXXNET {

class CxxnetWorker : public Cxxnet {
 public:
  virtual void init() { Cxxnet::init(); }
  virtual void process(const MessagePtr& msg) {
    auto cmd = get(msg).cmd();
    if (cmd == Call::RUN) {
      char* args[1];
      int n = conf_.args().size();
      args[0] = new char[n];
      memcpy(args[0], conf_.args().data(), n*sizeof(char));
      cxxnet::CXXNetLearnTask tsk;
      tsk.Run(1, args);
    }
  }
};

} // namespace CXXNET
} // namespace PS
