#pragma once

#include <list>
#include <memory>

#include "task.hpp"


namespace tasks
{
    static constexpr uint8_t MAX_TASK_COUNTS = std::numeric_limits<uint8_t>::max();
    static constexpr uint8_t MAX_TASKS_QUEUE_COUNTS = 32;

    // 任务队列状态枚举
    enum class TasksQueueStateType : int8_t
    {
        INVALID = -1,
        IDLE,
        BUSY,
        SUSPEND,
        INTERRUPT,
        STOP,
    };

    class TasksQueue
    {
        public:
            explicit TasksQueue(std::string q_name = "tasks_q_1", const uint8_t q_max_size = MAX_TASK_COUNTS)
                : _q_name(q_name)
                , _q_max_size(q_max_size) { }

            TasksQueue(const TasksQueue&) = delete;
            TasksQueue& operator=(const TasksQueue&) = delete;
            TasksQueue(const TasksQueue&&) = delete;
            TasksQueue& operator=(const TasksQueue&&) = delete;

            virtual ~TasksQueue() = default;

            static TasksQueue* tasksQueue()
            {
                static TasksQueue tq;
                return &tq;
            }

            SharedPtrTask task(const std::string task_name)
            {
                for(auto &iter : _q)
                {
                    if(iter->getName() == task_name)
                    {
                        return iter;
                    }
                }
            }

            size_t getTasksCount() const { return _q.size(); }
            TasksQueueStateType getTasksQueueState() const { return _q_state; }

            bool addTask(const Task& task)
            {
                return false;
            }

            bool addFastTask(const Task& task);

            void removeTask(const std::string task_name);

            bool modifyTaskAttribute(const TaskAttributeType type);

        private:
            std::string _q_name;
            const uint8_t _q_max_size;

            TasksQueueStateType _q_state;

            std::list<SharedPtrTask> _q;
    };
    using SharedPtrTasksQueue = std::shared_ptr<TasksQueue>;
    using WeakPtrTasksQueue = std::weak_ptr<TasksQueue>;
}   // namespace tasks