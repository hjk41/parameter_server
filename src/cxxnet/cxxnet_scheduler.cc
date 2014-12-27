#include "cxxnet/cxxnet_scheduler.h"
#include "system/postmaster.h"
namespace PS {
namespace CXXNET {

void CxxnetScheduler::run() {
  Postmaster sch(static_cast<Customer*>(this));

  // divide model
  auto nodes = sch.partitionKey(sch.nodes(), Range<Key>::all());

  // divide data TODO
  // auto data = sch.partitionData(conf_.input_graph(), FLAGS_num_workers);
  std::vector<AppConfig> apps(nodes.size(), app_cf_);
  // for (int i = 0; i < nodes.size(); ++i) {
  //   auto gp = apps[i].mutable_cxxnet();
  //   gp->clear_input_graph();
  //   if (nodes[i].role() == Node::WORKER) {
  //     *gp->mutable_input_graph() = data.back();
  //     data.pop_back();
  //   }
  // }

  // start the system
  sch.createApp(nodes, apps);


  // run
  Task run = newTask(Call::RUN);
  taskpool(kActiveGroup)->submitAndWait(run);
}

} // namespace CXXNET
} // namespace PS
