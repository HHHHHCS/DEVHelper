#pragma once

#include <atomic>
#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "../util/time_lib/time_lib.hpp"


namespace tasks
{
    static constexpr uint8_t MAX_PRIORITY = std::numeric_limits<uint8_t>::max();
    static constexpr uint8_t MIN_PRIORITY = std::numeric_limits<uint8_t>::min();
    static constexpr uint8_t DEFAULT_PRIORITY = (MIN_PRIORITY + MAX_PRIORITY) / 2;

    enum class TaskAttributeType: int8_t
    {
        INVALID = -1,
        PRIORITY,
        STATE,
        EXP_PARAM,
        // TODO(huangchsh): 后续属性在此添加
    };

    enum class TaskStateType : int8_t
    {
        INVALID = -1,
        READY,
        RUNNING,
        STOP,
        FINISH,
        TERMINATE,
    };

    using lock_guard = std::lock_guard<std::mutex>;
    using TaskFunc = std::function<void()>;

    class Task
    {
        friend class TasksQueue;

        public:
            explicit Task(const std::string name, const std::string q_name, const double target_mb, TaskFunc func)
                : _name(name)
                , _q_name(q_name)
                , _priority(DEFAULT_PRIORITY)
                , _state(TaskStateType::READY)
                , _target_mb(target_mb)
                , _work_thread_startup_flag(false)
                , _task_func(func) { }

            // TODO(huangchsh): 任务控制权转移及拷贝
            Task(const Task& cpy)
                : _target_mb(cpy.getTargetMb())
            {
                _name = cpy.getName();
                _q_name = cpy.getQueueName();
                _priority = cpy.getPriority();
                _state = cpy.getState();
                _task_func = cpy.getTaskFunc();
            }

            Task(const Task&& cpy)
                : _target_mb(cpy.getTargetMb())
            {
                Task(std::move(cpy));
            }

            Task& operator=(const Task&& cpy) = delete;
            Task& operator=(const Task& cpy) const
            {
                return Task(cpy);
            }

            virtual ~Task()
            {
                _work_thread_startup_flag = false;
                while(_work_thread_startup_flag);

                if(_work_thread)
                {
                    if(_work_thread->joinable())
                    {
                        _work_thread->join();
                    }
                    while(_work_thread->joinable());
                }

                _task_func = nullptr;
            }

            std::string getName() const { return _name; }
            std::string getQueueName() const { return _q_name; }
            TaskStateType getState() const { return _state; }
            uint8_t getPriority() const { return _priority; }
            double getTargetSpendTime() const { return _target_spend_time; }
            double getTargetMb() const { return _target_mb; }
            double getSpeed() const { return _speed_mb_s; }
            double getDoneMb() const { return _done_mb; }
            double getDonePercent() const { return _done_percent; }

        protected:
            TaskFunc getTaskFunc() const { return _task_func; }

        private:
            std::string _name;
            std::string _q_name;

            uint8_t _priority;

            TaskStateType _state;
            const double _target_mb;
            double _done_mb;
            double _done_percent;
            double _speed_mb_s;  // 当前速度
            double _spend_time;  // 当前用时

            // TODO(huangchsh): 需要记录时间，期望完成时间及当前时间，做到加减速
            util::time::SteadyTime _start_time;   // 任务开始时间
            double _target_spend_time;  // 预计用时

            double _exp_spend_time; // 期望用时

            double _single_run_duration;    // 单周期执行用时

            std::mutex _state_mutex;

            std::atomic_bool _work_thread_startup_flag;
            TaskFunc _task_func;
            std::unique_ptr<std::thread> _work_thread;

            void setPriority(const uint8_t prio) { _priority = prio; }
            void setState(const TaskStateType state) { _state = state; }
            void setTargetSpendTime(const double time) { _target_spend_time = time; }
            void setExpSpendTime(const double time) { _exp_spend_time = time; }

            virtual void start()
            {
                // TODO(huangchsh): 浮点数处理
                // 未设定目标，禁止开始
                if(_target_mb < 0.f)
                {
                    return;
                }

                // 除了准备状态外，一律禁止开始
                if(_state != TaskStateType::READY)
                {
                    return;
                }

                _start_time = util::time::SteadyClock::now();
                _state = TaskStateType::RUNNING;

                doWork();
            }

            virtual void stop()
            {
                lock_guard lock(_state_mutex);
                // TODO(huangchsh): 对当前任务状态进行判断，如优先级、速度、时间、状态的影响
                _state = TaskStateType::STOP;
            }

            virtual void terminate()
            {
                lock_guard lock(_state_mutex);
                // TODO(huangchsh): 处理任务后续状态
                _state = TaskStateType::TERMINATE;
            }

            virtual void doWork()
            {
                if(_state != TaskStateType::RUNNING)
                {
                    return;
                }

                if(!_task_func)
                {
                    _state = TaskStateType::TERMINATE;
                    return;
                }

                if(_work_thread_startup_flag)
                {
                    return;
                }

                _work_thread = std::make_unique<std::thread>([this]()
                {
                    _work_thread_startup_flag = true;
                    while(_work_thread_startup_flag
                        && (_state != TaskStateType::FINISH
                        && _state != TaskStateType::TERMINATE))
                    {
                        {
                            lock_guard lock(_state_mutex);
                            // TODO(huangchsh): 单周期执行用时记录

                            if(_state == TaskStateType::STOP)
                            {
                                std::this_thread::sleep_for(util::time::seconds(3));
                                continue;
                            }

                            // TODO(huangchsh): 执行捕获错误，进入终止态
                            if(_state != TaskStateType::TERMINATE)
                            {
                                try
                                {
                                    _task_func();
                                }
                                catch(std::exception &e)
                                {
                                    _state = TaskStateType::TERMINATE;
                                }
                            }

                            updateState();
                        }

                        std::this_thread::sleep_for(util::time::seconds(1));
                    }
                });
            }

            inline void updateState()
            {
                _done_percent = (_done_mb / _target_mb) * 100.f;

                _spend_time = util::time::elapsed<util::time::SteadyClock, util::time::ms>(_start_time);
                _speed_mb_s = (_target_mb - _done_mb) / _spend_time;

                _state = (_done_mb < _target_mb) ? TaskStateType::RUNNING : TaskStateType::FINISH;
            }
    };
    using SharedPtrTask = std::shared_ptr<Task>;
    using WeakPtrTask = std::weak_ptr<Task>;
}   // namespace tasks