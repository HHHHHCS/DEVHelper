#include <QDebug>

#include "link_manager.h"
#include "upgrade_manager.h"


using namespace managers;

UpgradeManager::UpgradeManager(QApplication *app, ManagerCollection* man_collect)
    : Manager(app, man_collect)
{

}

UpgradeManager::~UpgradeManager()
{

}

/**
 * @brief 上传函数
 * @note 用于上传本地固件或参数
 * @param[in] file_name: 刷写文件名
 */
void UpgradeManager::upload(const QVariant& file_name)
{

}

/**
 * @brief 下载函数
 * @note 用于下载云端或远端仓库固件或参数
 * @param[in] file_name: 刷写文件名
 */
void UpgradeManager::download(const QVariant& file_name)
{

}

/**
 * @brief 刷写函数
 * @note 用于选定固件或参数后更新或刷写
 * @param[in] file_name: 刷写文件名
 */
void UpgradeManager::flash(const QVariant& file_name)
{

}

/**
 * @brief 出厂配置函数
 * @note 用于实现固件及参数恢复或初次出厂
 */
void UpgradeManager::factory(const QString link_name)
{
    // TODO(huangchsh): 发送恢复出厂通知
    tasks::SharedPtrTasksQueue task_queue = man_collect()->link_man()->getFreeSharedPtrTasksQueueByLink(link_name);
    if(!task_queue)
    {
        qDebug() << "Failed to do" << link_name.toStdString().data() << "restore factory" << endl;
        return;
    }

    tasks::Task task(link_name.toStdString() + "_restore_factory", task_queue->getName(), 0, [p_proto = man_collect()->link_man()->getSharedPtrProtoByLinkName(link_name), this]()
    {
        if(p_proto->getProtoType() == communication::Proto::ProtoType::DEVLINK2)
        {
            static uint8_t msg_seq;

            awlink_restore_factory_t msg
            {
                // .time_boot_ms = 0,
                // .seq = msg_seq++,
                // .type = RC,
                // .id = 0,
            };

            uint32_t msg_id(AWLINK_MSG_ID_RESTORE_FACTORY);
            auto bytes(QByteArray(reinterpret_cast<char*>(&msg_id), sizeof(msg_id)));
            bytes.append(reinterpret_cast<char*>(&msg), sizeof(awlink_restore_factory_t));
            p_proto->sigPack(bytes);
        }
    });

    bool result = task_queue->addTask(task);
    if(result)
    {
        qDebug() << "Append task" << task.getName().data() << "into queue" << task_queue->getName().data() << endl;
    }
    else
    {
        qDebug() << "Failed append task" << task.getName().data() << "into queue" << task_queue->getName().data() << endl;
    }
}

/**
 * @brief 更新函数
 * @note 用于获取远端更新
 */
void UpgradeManager::update()
{

}

/**
 * @brief 获取URL
 *
 */
void UpgradeManager::getUrl()
{

}