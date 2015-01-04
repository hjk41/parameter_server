#pragma once
#include "cxxnet/cxxnet.h"
#include "cxxnet/cxxnet_kvvector.h"
namespace PS {
namespace CXXNET {

class CxxnetServer : public Cxxnet {
 public:
  virtual void init() {
    Cxxnet::init();
    // TODO i should parse the config file get the layer names, but i hard code
    // the layers for MNIST.conf
    layers_.resize(2);
    layers_[0] = SharedParameterPtr<int>(new CxxnetKVVector<int, real_t>());
    layers_[1] = SharedParameterPtr<int>(new CxxnetKVVector<int, real_t>());
    REGISTER_CUSTOMER("layer_1", layers_[0]);
    REGISTER_CUSTOMER("layer_1", layers_[1]);
    this->addChild("layer_1");
    this->addChild("layer_2");
  }

 private:
  std::vector<SharedParameterPtr<int>> layers_;
};

} // namespace CXXNET
} // namespace PS
