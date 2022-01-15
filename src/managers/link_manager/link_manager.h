#pragma once

#include <functional>
#include <thread>
#include <atomic>

#include <QObject>
#include <QMap>
#include <QString>

#include "tasks_queue.hpp"

#include "manager_collection.h"
#include "serial_link.h"


namespace managers
{
    class LinkManager : public Manager
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

        private slots:
            /**
             * @brief 连接断开槽
             * @note 对应信号 communication::Link::disconnected
             */
            void slotsLinkDisconnected();

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

            Q_ENUM(communication::Link::LinkType);

        public:
            LinkManager(QApplication *app, ManagerCollection* man_collect);
            ~LinkManager();

            QVariantMap getLinkInfoMap() const { return m_link_info_map; }
            int getLinkNum() const { return m_links_map.count(); }
            communication::SharedLinkPtr getSharedLinkPtrByLinkName(const QString name);

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
            using LinkTasksQueueMap = QMap<communication::SharedLinkPtr, QList<tasks::SharedPtrTasksQueue>>;
            using LinkTasksQueueMapConstIter = QMap<communication::SharedLinkPtr, QList<tasks::SharedPtrTasksQueue>>::const_iterator;
            using LinkTasksQueueMapIter = QMap<communication::SharedLinkPtr, QList<tasks::SharedPtrTasksQueue>>::iterator;

            using LinkOptFunc = std::function<communication::SharedLinkPtr(communication::SharedLinkPtr)>;

        private:
            static constexpr char *LINK_DEV_NAME_LIST[] = {"rc", "rn", "rp", "ba", "uav"};   // 可选设备列表

            std::thread *m_p_serial_scan_thread;
            std::atomic_bool m_serial_scan_thread_stop_flag;

            QVariantMap m_link_info_map;   // 可连接端口名列表

            LinkTasksQueueMap m_links_map;   // 选择的连接端口指针对象及其携带的任务队列

            communication::SharedLinkPtr searchLinkToDo(LinkOptFunc func);
    };
}   // namespace managers