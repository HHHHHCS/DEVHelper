#pragma once

#include <limits>
#include <string>
#include <chrono>
#include <memory>
#include <deque>

#include "util/time.h"


namespace tasks
{
    static constexpr uint8_t MAX_PRIORITY = std::numeric_limits<uint8_t>::max();
    static constexpr uint8_t MIN_PRIORITY = std::numeric_limits<uint8_t>::min();
    static constexpr uint8_t DEFAULT_PRIORITY = (MIN_PRIORITY + MAX_PRIORITY) / 2;

    enum class TaskState : int8_t
    {
        INVALID,
        READY,
        DOING,
        STOP,
        FINISH,
        TERMINATE,
    };

    template<typename dT>
    class Task
    {
        public:
            explicit Task(const std::string name, const uint8_t prio = DEFAULT_PRIORITY, const std::string q_name)
                : _name(name)
                , _q_name(q_name)
                , _state(TaskState::READY)
                , _priority(prio)
            {
            }

            // TODO(huangchsh): 任务控制权转移及拷贝

            ~Task() = default;

            void setTarget(const double target_mb) { _target_mb = target_mb; }

            std::string getName() const { return _name; }
            std::string getQueueName() const { return _q_name; }
            TaskState getState() const { return _state; }
            uint8_t getPriority() const { return _priority; }
            double getTarget() const { return _target_mb; }
            double getSpeed() const { return _speed_mb_s; }
            double getDone() const { return _done_mb; }
            double getDonePercent() const { return _done_percent; }

            void start()
            {
                // TODO(huangchsh): 浮点数处理
                // 未设定目标，禁止开始
                if(_target_mb < 0.f)
                {
                    return;
                }

                // 除了准备状态外，一律禁止开始
                if(_state != TaskState::READY)
                {
                    return;
                }

                _start_time = util::time::now();
                _state = TaskState::DOING;
            }

            void stop()
            {
                // TODO(huangchsh): 对当前任务状态进行判断，如优先级、速度、时间、状态的影响
                _state = TaskState::STOP;
            }

            void teminate()
            {
                // TODO(huangchsh): 处理任务后续状态
                _state = TaskState::TERMINATE;
            }

            void doWork(std::vector<dT>& data)
            {
                if(_state != TaskState::DOING)
                {
                    return;
                }

                
            }

        private:
            const uint8_t _priority;

            std::string _name;
            std::string _q_name;

            TaskState _state;
            double _target_mb;
            double _done_mb;
            double _done_percent;

            double _speed_mb_s;

            // TODO(huangchsh): 需要记录时间，期望完成时间及当前时间，做到加减速
            util::time::SteadyTime _start_time;   // 任务开始时间
            double _spend_time;  // 当前用时
            double _exp_finish_time; // 期望完成时间
            double _exp_speed;   // 期望执行速度

            void updateState()
            {
                if(_state == TaskState::FINISH)
                {
                    return;
                }

                _done_percent = (_done_mb / _target_mb) * 100.f;

                _spend_time = util::time::elapsed<util::time::SteadyTime, util::time::ms>(_start_time);
                _speed_mb_s = (_target_mb - _done_mb) / _spend_time;

                _state = (_done_mb < _target_mb) ? TaskState::DOING : TaskState::FINISH;
                // TODO(huangchsh): 能够推算期望速度及期望完成时间
            }
    };
    using SharedPtrTask = std::shared_ptr<Task>;
    using WeakPtrTask = std::weak_ptr<Task>;
}