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
void UpgradeManager::factory()
{
    // TODO(huangchsh): 发送恢复出厂通知
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