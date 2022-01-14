#pragma once

#include <QObject>
#include <QString>
#include <QMap>

#include "manager_collection.h"


namespace managers
{
    class ParametersManager : public Manager
    {
        Q_OBJECT

        signals:
            /**
             * @brief 请求参数列表信号
             * @note 对应槽
             * @param[in] link_name: 连接名
             */
            void requestParametersMap(const QString link_name);

            /**
             * @brief 获取参数列表信号
             * @note 对应槽
             * @param[in] link_params_mapp: 包括连接名、若干参数（包括参数名及参数值）
             */
            void fetchParametersMap(const QVariantMap link_params_map);

        public slots:
            /**
             * @brief 获取及创建参数列表
             * @note 对应信号 requestParametersMap
             * @param[in] link_name: 请求参数列表信号中的连接名
             */
            void createParametersMap(const QString link_name);

        public:
            /**
             * @brief 增加参数
             * @note 供QML调用
             * @param[in] link_params_mapp: 包括连接名、若干参数（包括参数名及参数值）
             */
            Q_INVOKABLE void addParameters(const QVariantMap link_params_map);

            /**
             * @brief 删除参数
             * @note 供QML调用
             * @param[in] link_param_map: 包括连接名及参数名
             */
            Q_INVOKABLE void removeParameters(const QVariantMap link_param_map);

            /**
             * @brief 修改参数
             * @note 供QML调用
             * @param[in] link_params_map: 包括连接名、若干参数（包括参数名及参数值）
             */
            Q_INVOKABLE void modifyParameters(const QVariantMap link_params_map);

            /**
             * @brief 查找参数
             * @note 供QML调用
             * @param[in] link_params_map: 包括连接名及若干参数名
             */
            Q_INVOKABLE void findParameters(const QVariantMap link_params_map);

        public:
            ParametersManager(QApplication *app, ManagerCollection* man_collect);
            ~ParametersManager();

        private:
            // TODO(huangchsh): 同一连接名存在连接多个设备的情况，需要考虑优化
            QVariantMap m_dev_params_map;   // 与连接名相对应的参数列表

            void upload(const QString dev_name);

            void download(const QString dev_name);

            void fetch(const QString dev_name);
            // TODO(huangchsh): 实现参数列表解析

    };
}   // namespace managers