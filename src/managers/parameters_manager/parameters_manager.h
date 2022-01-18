#pragma once

#include <functional>

#include <QObject>
#include <QVariant>
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
             * @note 对应槽 slotsCreateParametersMap
             * @param[in] link_name: 连接名
             */
            void sigRequestParametersMap(const QString link_name);

            /**
             * @brief 获取参数列表信号
             * @note 对应槽
             * @param[in] link_name: 连接名
             * @param[in] link_params_map: 若干参数（包括参数名及参数值）
             */
            void sigFetchParametersMap(const QString link_name, const QVariantMap link_params_map);

            /**
             * @brief 修改参数成功信号
             * @param[in] link_name: 连接名
             * @param[in] params_name_list: 参数名列表
             */
            void sigModifyParametersSuccess(const QString link_name, const QVariantList params_name_list);

        private slots:
            /**
             * @brief 获取及创建参数列表
             * @note 对应信号 sigRequestParametersMap
             * @param[in] link_name: 请求参数列表信号中的连接名
             */
            void slotsCreateParametersMap(const QString link_name);

        public:
            using ParametersMap = QVariantMap;
            using ParametersMapIterator = ParametersMap::Iterator;
            using ParametersMapConstIterator = ParametersMap::ConstIterator;
            using LinkParametersMap = QMap<QString, ParametersMap>;
            using LinkParametersMapIterator = LinkParametersMap::Iterator;
            using LinkParametersMapConstIterator = LinkParametersMap::ConstIterator;

        public:
            /**
             * @brief 增加参数
             * @note 供QML调用
             * @param[in] link_name: 连接名
             * @param[in] link_params_map: 若干参数（包括参数名及参数值）
             */
            Q_INVOKABLE void addParameters(const QString link_name, const ParametersMap link_params_map);

            /**
             * @brief 删除参数
             * @note 供QML调用
             * @param[in] link_name: 连接名
             * @param[in] param_name: 参数名
             */
            Q_INVOKABLE void removeParameters(const QString link_name, const QString param_name);

            /**
             * @brief 修改参数
             * @note 供QML调用
             * @param[in] link_name: 连接名
             * @param[in] link_params_map: 若干参数（包括参数名及参数值）
             */
            Q_INVOKABLE void modifyParameters(const QString link_name, const ParametersMap link_params_map);

            /**
             * @brief 查找单个参数
             * @note 供QML调用，修改成功后发送修改参数成功信号，UI同步
             * @param[in] link_name: 连接名
             * @param[in] param_name: 参数名
             * @return 参数名对应的参数值
             *          -1: 未查找到连接对应参数列表
             *          -2: 未查找到此参数
             */
            Q_INVOKABLE QVariant findParameters(const QString link_name, const QString param_name);

        public:
            ParametersManager(QApplication *app, ManagerCollection* man_collect);
            ~ParametersManager();

        private:
            // TODO(huangchsh): 同一连接名存在连接多个设备的情况，需要考虑优化
            LinkParametersMap m_dev_params_map;   // 与连接名相对应的参数列表

            void upload(const QString dev_name);

            void download(const QString dev_name);

            void fetch(const QString dev_name);
            // TODO(huangchsh): 实现参数列表解析
    };
}   // namespace managers