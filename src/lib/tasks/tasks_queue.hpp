#pragma once

#include <iostream>
#include <list>

#include "task.hpp"


namespace tasks
{
    static constexpr uint32_t MAX_TASK_COUNTS = std::numeric_limits<uint8_t>::max() + 1;
    static constexpr uint8_t MAX_TASKS_QUEUE_COUNTS = 32;
    static constexpr uint8_t MAX_RUNNING_TASKS_MEANWHILE_CONNTS = 1;

    // 任务队列状态枚举
    enum class TasksQueueStateType : int8_t
    {
        INVALID = -1,
        IDLE,
        BUSY,
        SUSPEND,
        INTERRUPT,
    };

    // TODO(huangchsh): 不支持同名队列

    class TasksQueue final
    {
        public:
            explicit TasksQueue(std::string q_name  = "tasks_q_1" , const uint32_t q_max_size = MAX_TASK_COUNTS)
                : _q_name(q_name)
                , _q_max_size(q_max_size)
                , _q_state(TasksQueueStateType::IDLE)
                , _q_sch_thread_startup_flag(false)
                , _q_running_tasks_meanwhile_counts(0)
                , _q_sch_thread(nullptr)
            {
                startupScheduler();
            }

            TasksQueue(const TasksQueue& cpy) = delete;
            TasksQueue& operator=(const TasksQueue&) = delete;
            TasksQueue(const TasksQueue&&) = delete;
            TasksQueue& operator=(const TasksQueue&&) = delete;

            ~TasksQueue()
            {
                removeAllTasks();

                _q_sch_thread_startup_flag = false;
                if(_q_sch_thread->joinable())
                {
                    _q_sch_thread->join();
                }

                _q_sch_thread = nullptr;
            }

            SharedPtrTask operator[](const uint8_t prio)
            {
                return getTask(prio);
            }

            SharedPtrTask operator[](const std::string task_name)
            {
                return getTask(task_name);
            }

            SharedPtrTask getTask(const uint8_t prio) const
            {
                for(auto &iter : _q_list)
                {
                    if(iter->getPriority() == prio)
                    {
                        return iter;
                    }
                }

                return SharedPtrTask(nullptr);
            }

            SharedPtrTask getTask(const std::string task_name) const
            {
                for(auto &iter : _q_list)
                {
                    if(iter->getName() == task_name)
                    {
                        return iter;
                    }
                }

                return SharedPtrTask(nullptr);
            }

            void setState(const TasksQueueStateType state) { _q_state = state; }

            std::string getName() const { return _q_name; }
            uint8_t getMaxSize() const { return _q_max_size; }
            size_t getTasksCount() const { return _q_list.size(); }
            TasksQueueStateType getState() const { return _q_state; }

            bool addTask(const Task& task)
            {
                if(_q_list.size() == MAX_TASK_COUNTS)
                {
                    // TODO(huangchsh)增加log
                    return false;
                }

                lock_guard lock(_q_mutex);
                SharedPtrTask p_task = std::make_shared<Task>(task);
                _q_list.push_back(p_task);

                p_task->setPriority(_q_list.size() - 1);

                return true;
            }

            bool addFastTask(const Task& task)
            {
                if(_q_list.size() == MAX_TASK_COUNTS)
                {
                    return false;
                }

                // TODO(huangchsh): 若当前未存在正在进行中任务，但是队列中已存在若干任务，则需要根据任务数进行判断，限制入队
                if(_q_running_tasks_meanwhile_counts == MAX_RUNNING_TASKS_MEANWHILE_CONNTS)
                {
                    return false;
                }
#if 0 // 暂时关闭插队逻辑
                uint8_t running_tasks = 0;
                for(auto i = 0; i < MAX_RUNNING_TASKS_MEANWHILE_CONNTS; ++i)
                {
                    SharedPtrTask p_task = getTask(i);
                    if(!p_task)
                    {
                        continue;
                    }

                    if(p_task->getPriority() <= MAX_RUNNING_TASKS_MEANWHILE_CONNTS)
                    {
                        if(p_task->getState() == TaskStateType::RUNNING)
                        {
                            ++running_tasks;
                        }
                    }
                }

                if(running_tasks == MAX_RUNNING_TASKS_MEANWHILE_CONNTS)
                {
                    return false;
                }
#endif
                lock_guard lock(_q_mutex);
                SharedPtrTask p_task = std::make_shared<Task>(task);
                _q_list.push_front(p_task);

                p_task->setPriority(0);

                return true;
            }

            void removeTask(const std::string task_name)
            {
                removeTaskListElem(getTask(task_name));
            }

            void removeTask(const uint8_t prio)
            {
                removeTaskListElem(getTask(prio));
            }

            void removeAllTasks()
            {
                for(auto &item : _q_list)
                {
                    removeTaskListElem(item);
                }
            }

            void modifyTaskAttribute(const std::string name, const TaskAttributeType attr_type, void *arg)
            {
                switch(attr_type)
                {
                    case TaskAttributeType::STATE:
                    {
                        lock_guard lock(_q_mutex);
                        getTask(name)->setState(*reinterpret_cast<TaskStateType*>(arg));
                        break;
                    }
                    case TaskAttributeType::PRIORITY:
                    {
                        lock_guard lock(_q_mutex);
                        getTask(name)->setPriority(*reinterpret_cast<uint8_t*>(arg));
                        break;
                    }
                    case TaskAttributeType::EXP_PARAM:
                    {
                        lock_guard lock(_q_mutex);
                        // TODO(huangchsh): 期望参数修改
                        break;
                    }
                    case TaskAttributeType::INVALID:
                    default: break;
                }
            }

            void startupScheduler()
            {
                _q_sch_thread_startup_flag = true;

                _q_sch_thread = new std::thread([this] ()
                {
                    while(_q_sch_thread_startup_flag)
                    {
                        {
                            try
                            {
                                switch(_q_state)
                                {
                                    case TasksQueueStateType::BUSY:
                                    case TasksQueueStateType::IDLE:
                                    {
                                        lock_guard lock(_q_mutex);
                                        if(_q_list.empty())
                                        {
                                            std::this_thread::sleep_for(util::time::seconds(3));
                                            continue;
                                        }

                                        for(auto &item : _q_list)
                                        {
                                            if(item->getState() == TaskStateType::READY
                                            && item->getPriority() <= MAX_RUNNING_TASKS_MEANWHILE_CONNTS)
                                            {
                                                item->start();
                                            }

                                            if(item->getState() == TaskStateType::FINISH
                                            || item->getState() == TaskStateType::TERMINATE)
                                            {
                                                removeTaskListElem(item);
                                            }
                                        }

                                        // TODO(huangchsh): 能够推算各任务期望速度及期望完成时间

                                        if(_q_list.size() == MAX_TASK_COUNTS
                                        && _q_state != TasksQueueStateType::BUSY)
                                        {
                                            _q_state = TasksQueueStateType::BUSY;
                                        }
                                        break;
                                    }
                                    case TasksQueueStateType::SUSPEND:
                                    {
                                        // 队列挂起时，暂停所有任务
                                        stopAllTasks();
                                        break;
                                    }
                                    case TasksQueueStateType::INTERRUPT:
                                    {
                                        // 1. 中断时，暂停所有任务
                                        stopAllTasks();
                                        // TODO(huangchsh): 2. 保存队列及任务状态记录，中断原因
                                        break;
                                    }
                                    case TasksQueueStateType::INVALID:
                                    default: break;
                                }
                            }
                            catch(const std::exception& e)
                            {
                                // TODO(huangchsh): 捕获错误，进入挂起态
                                _q_state = TasksQueueStateType::SUSPEND;
                            }
                        }

                        std::this_thread::sleep_for(util::time::milliseconds(1));
                    }
                });
            }

        private:
            using TaskPtrList = std::list<SharedPtrTask>;
            using TaskPtrListConstIterator = TaskPtrList::const_iterator;
            using TaskPtrListIterator = TaskPtrList::iterator;

        private:
            std::string _q_name;
            const uint32_t _q_max_size;

            TaskPtrList _q_list;

            TasksQueueStateType _q_state;

            std::mutex _q_mutex;
            std::atomic_bool _q_sch_thread_startup_flag;
            std::atomic_uint8_t _q_running_tasks_meanwhile_counts;
            std::thread* _q_sch_thread;

            void removeTaskListElem(const SharedPtrTask &elem)
            {
                if(elem->getState() == TaskStateType::RUNNING)
                {
                    elem->terminate();
                }

                lock_guard lock(_q_mutex);
                // 查询指针引用计数
                WeakPtrTask wp_task = elem;
                if(!wp_task.expired())
                {
                    // TODO(huangchsh): 需要验证此方法可行性
                    for(auto i = 0; i < wp_task.use_count(); ++i)
                    {
                        _q_list.remove(elem);
                    }
                }
                else
                {
                    _q_list.remove(elem);
                }
            }

            void stopAllTasks()
            {
                lock_guard lock(_q_mutex);
                for(auto &item : _q_list)
                {
                    item->stop();
                }
            }

            // TODO(huangchsh): 异常信号接收触发挂起或中断
    };
    using SharedPtrTasksQueue = std::shared_ptr<TasksQueue>;
    using WeakPtrTasksQueue = std::weak_ptr<TasksQueue>;
}   // namespace tasks