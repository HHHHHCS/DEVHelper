#pragma once

#include <QObject>
#include <QVariant>
#include <QString>

#include "manager_collection.h"


namespace managers
{
    class UpgradeManager final : public Manager
    {
        Q_OBJECT

        signals:
            /**
             * @brief 出厂操作执行完成
             */
            void sigDoFactoryFinished();

        public:
            /**
             * @brief 上传函数
             * @note 用于上传本地固件或参数
             * @param[in] file_name: 刷写文件名
             */
            Q_INVOKABLE void upload(const QVariant& file_name);

            /**
             * @brief 下载函数
             * @note 用于下载云端或远端仓库固件或参数
             * @param[in] file_name: 刷写文件名
             */
            Q_INVOKABLE void download(const QVariant& file_name);

            /**
             * @brief 刷写函数
             * @note 用于参数刷写及固件刷写
             * @param[in] file_name: 刷写文件名
             */
            Q_INVOKABLE void flash(const QVariant& file_name);

            /**
             * @brief 出厂配置函数
             * @note 用于实现固件及参数恢复或初次出厂
             */
            Q_INVOKABLE void factory(const QString link_name);

        public:
            explicit UpgradeManager(QApplication *app, ManagerCollection* man_collect);
            UpgradeManager(const UpgradeManager&) = delete;
            UpgradeManager(const UpgradeManager&&) = delete;
            UpgradeManager& operator=(const UpgradeManager&) = delete;
            UpgradeManager& operator=(const UpgradeManager&&) = delete;
            ~UpgradeManager() final;

        private:
            /**
             * @brief 更新函数
             * @note 用于获取远端更新
             */
            void update();

            /**
             * @brief 获取URL
             *
             */
            void getUrl();
    };
}   // namespace managers