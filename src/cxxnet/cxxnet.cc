#include "cxxnet/cxxnet.h"
#include "cxxnet/cxxnet_scheduler.h"
#include "cxxnet/cxxnet_worker.h"
#include "cxxnet/cxxnet_server.h"

namespace PS {
namespace GP {

AppPtr Cxxnet::create(const Config& conf) {
  auto my_role = Postoffice::instance().myNode().role();
  if (conf.has_cxxnet()) {
    if (my_role == Node::SCHEDULER) {
      return AppPtr(new CxxnetScheduler());
    } else if (my_role == Node::WORKER) {
      return AppPtr(new CxxnetWorker());
    } else if (my_role == Node::SERVER) {
      return AppPtr(new CxxnetServer());
    }
  }
  CHECK(false) << "unknow config" << conf.DebugString();
  return AppPtr(nullptr);
}

} // namespace GP
} // namespace PS
