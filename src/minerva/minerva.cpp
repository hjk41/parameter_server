#include "minerva.h"
#include "system/postmaster.h"

namespace PS
{
namespace Minerva
{

    void MinervaScheduler::run()
    {
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

    void MinervaServer::init()
    {
        // TODO i should parse the config file get the layer names, but i hard code
        // the layers for MNIST.conf
        layers_.resize(2);
        layers_[0] = SharedParameterPtr<int>(new float[1]);
        layers_[1] = SharedParameterPtr<int>(new float[1]);
        REGISTER_CUSTOMER("layer_1", layers_[0]);
        REGISTER_CUSTOMER("layer_1", layers_[1]);
        this->addChild("layer_1");
        this->addChild("layer_2");
    }

    void MinervaWorker::process(const MessagePtr & msg)
    {
        auto cmd = get(msg).cmd();
        if (cmd == Call::RUN) {
            char* args[2];
            int n = conf_.args().size();
            args[1] = new char[n];
            memcpy(args[1], conf_.args().data(), n*sizeof(char));
            // run minerva
        }
    }

}
}