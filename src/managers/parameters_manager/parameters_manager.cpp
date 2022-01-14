#include "link_manager.h"
#include "parameters_manager.h"


using namespace managers;

ParametersManager::ParametersManager(QApplication *app, ManagerCollection* man_collect)
    : Manager(app, man_collect)
{
    connect(this, &requestParametersMap, this, &createParametersMap);
}

ParametersManager::~ParametersManager()
{

}

/**
 * @brief 获取及创建参数列表
 * @note 对应信号 requestParametersMap
 * @param[in] link_name: 请求参数列表信号中的连接名
 */
void ParametersManager::createParametersMap(const QString link_name)
{
    // TODO(huangchsh): 1. 建立任务
    man_collect()->link_man();
    // 发送获取请求

    // TODO(huangchsh): 2. 插入任务队列

    // TODO(huangchsh): 3. 等待任务完成信号，修改参数列表

    // 4. 发出参数列表信号
    // emit fetchParametersMap(m_dev_params_map);
}