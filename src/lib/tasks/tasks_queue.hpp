#pragma once

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
        STOP,
    };

    // TODO(huangchsh): 不支持同名队列

    class TasksQueue final
    {
        public:
            explicit TasksQueue(std::string q_name = "tasks_q_1", const uint32_t q_max_size = MAX_TASK_COUNTS)
                : _q_name(q_name)
                , _q_max_size(q_max_size)
                , _q_state(TasksQueueStateType::IDLE)
                , _q_sch_thread_startup_flag(false)
                , _q_running_tasks_meanwhile_counts(0)
                , _q_sch_thread(nullptr) {  }

            // TasksQueue(const TasksQueue& cpy)
            // {
            //     _q_name = cpy.getName();
            //     _q_max_size = cpy.getMaxSize();
            //     _q_state = cpy.getState();
            // }

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

            static TasksQueue* tasksQueue()
            {
                static TasksQueue tq;
                return &tq;
            }

            SharedPtrTask operator[](const uint8_t prio)
            {
                return task(prio);
            }

            SharedPtrTask operator[](const std::string task_name)
            {
                return task(task_name);
            }

            SharedPtrTask task(const uint8_t prio)
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

            SharedPtrTask task(const std::string task_name)
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

                uint8_t running_tasks = 0;
                for(auto i = 0; i < MAX_RUNNING_TASKS_MEANWHILE_CONNTS; ++i)
                {
                    // TODO(huangchsh): 判断指针非空
                    // TOOD(huangchsh): 判断是否小于正在执行任务数
                    // if(this[i]->getPriority() <= MAX_RUNNING_TASKS_MEANWHILE_CONNTS)
                    // {
                    //     ++running_tasks;
                    // }
                }

                // for(auto &item : _q_list)
                // {
                //     if(item->getPriority() <= MAX_RUNNING_TASKS_MEANWHILE_CONNTS)
                //     // && item->)
                //     {
                //         ++running_tasks;
                //     }
                //     else
                //     {
                //         break;
                //     }
                // }

                lock_guard lock(_q_mutex);
                SharedPtrTask p_task = std::make_shared<Task>(task);
                _q_list.push_front(p_task);

                p_task->setPriority(0);

                return true;
            }

            void removeTask(const std::string task_name)
            {
                for(auto &item : _q_list)
                {
                    if(item->getName() == task_name)
                    {
                        removeTaskListElem(item);
                    }
                }
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
                        task(name)->setState(*reinterpret_cast<TaskStateType*>(arg));
                        break;
                    }
                    case TaskAttributeType::PRIORITY:
                    {
                        lock_guard lock(_q_mutex);
                        task(name)->setPriority(*reinterpret_cast<uint8_t*>(arg));
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
                                        case TasksQueueStateType::STOP:
                                        {
                                            break;
                                        }
                                        case TasksQueueStateType::INTERRUPT:
                                        {
                                            // TODO(huangchsh): 支持历史记录、异常/中断记录
                                            break;
                                        }
                                        case TasksQueueStateType::SUSPEND:
                                        {
                                            break;
                                        }
                                        case TasksQueueStateType::INVALID:
                                        default: break;
                                    }
                                }
                            }
                            catch(const std::exception& e)
                            {
                                // TODO(huangchsh): 捕获错误，进入挂起态
                                // std::cerr << e.what() << '\n';
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

            std::list<SharedPtrTask> _q_list;

            TasksQueueStateType _q_state;

            std::mutex _q_mutex;
            std::atomic_bool _q_sch_thread_startup_flag;
            std::atomic_uint8_t _q_running_tasks_meanwhile_counts;
            std::thread* _q_sch_thread;

            void removeTaskListElem(SharedPtrTask &elem)
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
    };
    using SharedPtrTasksQueue = std::shared_ptr<TasksQueue>;
    using WeakPtrTasksQueue = std::weak_ptr<TasksQueue>;
}   // namespace tasks