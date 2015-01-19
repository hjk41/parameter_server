#pragma once

#include <memory>
#include "minerva.pb.h"
#include "parameter/shared_parameter.h"
#include "system/app.h"
#include "system/message.h"

namespace PS
{
    namespace Minerva
    {
        typedef std::shared_ptr<App> AppPtr;
        class MinervaScheduler;
        class MinervaWorker;
        class MinervaServer;

        template <typename K, typename V>
        class MinervaKVVector : public KVVector<K, V> {
            void setValue(const MessagePtr& msg) {
                int chl = msg->task.key_channel();
                int time = msg->task.time();
                if (time != last_time) {
                    this->val_[chl].setZero();
                }

                KVVector<K, V>::setValue(msg);

                // we need to -1 here because the current task will be finished after
                // exitting this function.
                // TODO relax this aggregation to ignore the stragglers
                if (this->taskpool(kWorkerGroup)->countIncomingTask(time) ==
                    FLAGS_num_workers - 1) {
                    // TODO in current implementation, each worker pull the aggregated
                    // gradient back. in the future,  call the update function here, so that
                    // worker pull the updated weights.

                    // the gradients have been aggregated, allow workers to pull now
                    this->taskpool(kWorkerGroup)->finishIncomingTask(time + 1);
                }
            }
        private:
            int last_time = -1;
        };


        class MinervaApp : public App
        {
        public:
            static AppPtr create(const Config& conf)
            {
                auto my_role = Postoffice::instance().myNode().role();
                if (my_role == Node::SCHEDULER) {
                    return AppPtr(new MinervaScheduler());
                }
                else if (my_role == Node::WORKER) {
                    return AppPtr(new MinervaWorker());
                }
                else if (my_role == Node::SERVER) {
                    return AppPtr(new MinervaServer());
                }
                CHECK(false) << "unknow node" << conf.DebugString();
                return AppPtr(nullptr);
            }

            static Call get(const MessageCPtr& msg) {
                CHECK_EQ(msg->task.type(), Task::CALL_CUSTOMER);
                CHECK(msg->task.has_minerva());
                return msg->task.minerva();
            }
            static Call* set(Task *task) {
                task->set_type(Task::CALL_CUSTOMER);
                return task->mutable_minerva();
            }
            static Task newTask(Call::Command cmd) {
                Task task; set(&task)->set_cmd(cmd);
                return task;
            }

            virtual void init() {
                conf_ = app_cf_.minerva();
                LL << conf_.DebugString();
            }
            virtual void run() { }
            virtual void process(const MessagePtr& msg) {}
        protected:
            Config conf_;
        };


        class MinervaScheduler : public MinervaApp
        {
        public:
            virtual void init() { MinervaApp::init(); }
            virtual void run();
            virtual void process(const MessagePtr & msg) {}
        };

        class MinervaServer : public MinervaApp
        {
        public:
            virtual void init();
        private:
            std::vector<SharedParameterPtr<int>> layers_;
        };

        class MinervaWorker : public MinervaApp
        {
        public:
            virtual void init() { MinervaApp::init();  }
            virtual void process(const MessagePtr & msg);
        };
    };
};
