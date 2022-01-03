import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0

import Managers.LinkManager 1.0


Item
{
    id: scan_link_list_interface

    anchors.fill: parent

    LinkManager
    {
        id: link_manager
    }

    Label
    {
        id: waiting_view_label

        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        text: qsTr("等待设备接入...")

        font.pixelSize: 30
    }

    ListView
    {
        id: link_list_view

        clip: true

        visible: false
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.7; height: parent.height * 0.7

        model: link_list_view_model
        delegate: list_elem_delegate

        ListModel
        {
            id: link_list_view_model
        }

        Component
        {
            id: list_elem_delegate

            Rectangle
            {
                id: list_item
                visible: true
                width: parent.width; height: 20

                Text
                {
                    text: name + ":" + description
                    font.pixelSize: parent.height - 5
                }

                Image
                {
                    anchors.fill: parent
                    // source: image
                    // TODO(huangchsh): 可考虑增加设备背景图片
                }

                MouseArea
                {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered:
                    {
                        if(is_current_item)
                        {
                            // TODO(huangchsh): 选项高亮
                        }
                    }

                    onExited:
                    {
                        if(is_current_item)
                        {
                            // TODO(huangchsh): 取消选项高亮
                        }
                    }

                    onClicked:
                    {
                        if(is_current_item)
                        {
                            // TODO(huangchsh): 发送选中信号
                        }
                    }
                }
            }
        }
    }

    Button     // NOTE(huangchsh): 测试代码，暂存
    {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        background: Rectangle
        {
            color: "transparent"
        }

        onClicked:
        {
            visible = false
            waiting_view_label.visible = false
            link_list_view.visible = true

            // page_view_stack.push(dev_helper_interface)
        }
    }
}