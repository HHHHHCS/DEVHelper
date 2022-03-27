#pragma once

#include <functional>
#include <thread>
#include <atomic>

#include <QObject>
#include <QMap>
#include <QString>

#include "tasks_queue.hpp"

#include "devlink2_proto.h"

#include "serial_link.h"

#include "manager_collection.h"


namespace managers
{
    class LinkManager final : public Manager
    {
        Q_OBJECT

        signals:
            /**
             * @brief 获取可用连接信息map信号
             * @note 对应处理于ScanLinksListInterface.qml中
             * @param[in] link_info_list: 可用连接信息map，包括连接名及连接描述
             */
            void sigFetchLinkInfoMap(const QVariantMap link_info_list);

            /**
             * @brief 增加连接信号
             * @note 对应处理于ScanLinksListInterface.qml中
             * @note 确认选择连接时发出
             */
            void sigAddLink();

            /**
             * @brief 更新连接状态信号
             * @note 对应处理于ScanLinksListInterface.qml中
             * @note 连接状态变化时发出
             */
            void sigUpdateLinkStatus(const QVariant status);

        private slots:
            /**
             * @brief 通信错误槽
             * @note 对应信号 communication::Link::sigCommError
             * @param[in] link_name 出现异常的连接名
             * @param[in] link_error 出现的具体异常
             */
            void slotLinkCommError(const QString& link_name, const QString& link_error);

            /**
             * @brief 连接断开槽
             * @note 对应信号 communication::Link::sigDisconnected
             */
            void slotLinkDisconnected();

        public:
            /**
             * @brief 创建用户于UI的所选连接
             * @note 建立连接后，创建任务队列，并在UI增加连接选项卡
             * @param[in] name: 端口名，如COM1
             */
            Q_INVOKABLE void createChoiceLink(const QString name);

            /**
             * @brief 删除用户于UI的所选连接
             * @note UI中删除连接选项卡后，删除连接
             * @param[in] name: UI选项卡对应连接名，端口名，如COM1
             */
            Q_INVOKABLE void removeChoiceLink(const QString name);

            enum class LinkStatusType : uint8_t
            {
                CONNECTED,
                DISCONNECTED,
                LOST,
                RECONNECTED,
            };
            Q_ENUM(LinkStatusType);

            Q_ENUM(communication::Link::LinkType);

        public:
            explicit LinkManager(QApplication *app, ManagerCollection* man_collect);
            LinkManager(const LinkManager&) = delete;
            LinkManager(const LinkManager&&) = delete;
            LinkManager& operator=(const LinkManager&) = delete;
            LinkManager&& operator=(const LinkManager&&) = delete;
            ~LinkManager() final;

            QVariantMap& getLinkInfoMap() { return m_link_info_map; }
            int getLinkNum() const { return m_link_block_map.count(); }
            communication::SharedPtrLink getSharedPtrLinkByName(const QString& name);
            communication::SharedPtrProto getSharedPtrProtoByLinkName(const QString& name);
            tasks::SharedPtrTasksQueue getFreeSharedPtrTasksQueueByLink(const QString& name);

            /**
             * @brief 创建可用连接列表扫描线程
             * @note 扫描周期3s
             */
            void createScanLinksListWork();

            /**
             * @brief 断开连接列表中所有连接
             */
            void disconnectAll();

        private:
            using TasksQueuePtrList = QList<tasks::SharedPtrTasksQueue>;
            using TasksQueuePtrListIterator = TasksQueuePtrList::Iterator;
            using TasksQueuePtrListConstIterator = TasksQueuePtrList::ConstIterator;

            struct LinkBlock
            {
                communication::SharedPtrProto p_proto;

                TasksQueuePtrList tasks_q_list;

                explicit LinkBlock() = default;
                LinkBlock(communication::Proto* proto, TasksQueuePtrList& t_q_list)
                    : p_proto(proto)
                    , tasks_q_list(t_q_list) {}
            };
            using LinkBlockMap = QMap<communication::SharedPtrLink, LinkBlock>;

        private:
            static constexpr char *LINK_DEV_NAME_LIST[] = {"rc", "rn", "rp", "ba", "uav"};   // 可选设备列表

            std::unique_ptr<std::thread> m_p_serial_scan_thread_ptr;
            std::atomic_bool m_serial_scan_thread_stop_flag;

            QVariantMap m_link_info_map;   // 可连接端口名列表

            LinkBlockMap m_link_block_map;   // 选择的连接端口指针对象及其携带的任务队列

            communication::SharedPtrLink searchLinkToDo(std::function<communication::SharedPtrLink(const communication::SharedPtrLink&)> func);
    };
}   // namespace managers