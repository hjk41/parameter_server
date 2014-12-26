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
// #ifdef CXXNET_PS
//       char* args[1];
//       args[0] = conf_.args().data();
//       tsk.Run(1, args);
// #else
//       CHECK(false);
// #endif // CXXNET_PS
    }
  }
};

} // namespace CXXNET
} // namespace PS
