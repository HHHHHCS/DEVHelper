#pragma once

#include <string>
#include <memory>

#include <boost/circular_buffer.hpp>


namespace tasks
{
    // 任务队列状态枚举
    enum class TasksQueueState : int8_t
    {
        INVALID = -1,
        IDLE,
        BUSY,
        SUSPEND,
        INTERRUPT,
        STOP,
    };

    //TODO(huangchsh): 完成任务队列模板类
    class TasksQueue
    {
        public:
            explicit TasksQueue(std::string q_name)
                : _q_name(std::move(q_name)) {}

            TasksQueue(const TasksQueue&) = delete;
            TasksQueue& operator=(const TasksQueue&) = delete;
            TasksQueue(const TasksQueue&&) = delete;
            TasksQueue& operator=(const TasksQueue&&) = delete;

            virtual ~TasksQueue() = default;

        private:
            std::string _q_name;
    };
    using SharedPtrTasksQueue = std::shared_ptr<TasksQueue>;
    using WeakPtrTasksQueue = std::weak_ptr<TasksQueue>;
}   // namespace tasks