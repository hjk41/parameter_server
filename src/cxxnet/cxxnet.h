#pragma once
#include "system/app.h"
#include "mshadow/tensor.h"
#include "global.h"
#include "cxxnet_main.h"

namespace PS {
namespace CXXNET {

typedef cxxnet::real_t real_t;

class Cxxnet : public App {
 public:
  static AppPtr create(const Config& conf);

  static Call get(const MessageCPtr& msg) {
    CHECK_EQ(msg->task.type(), Task::CALL_CUSTOMER);
    CHECK(msg->task.has_cxxnet());
    return msg->task.cxxnet();
  }
  static Call* set(Task *task) {
    task->set_type(Task::CALL_CUSTOMER);
    return task->mutable_cxxnet();
  }
  static Task newTask(Call::Command cmd) {
    Task task; set(&task)->set_cmd(cmd);
    return task;
  }

  virtual void init() {
    conf_ = app_cf_.cxxnet();
  }
  virtual void run() { }
  virtual void process(const MessagePtr& msg) { }
 protected:
  Config conf_;
};

} // namespace CXXNET
} // namespace PS
