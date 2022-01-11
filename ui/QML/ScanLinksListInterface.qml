import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0


Item
{
    id: scan_link_list_interface

    anchors.fill: parent

    Component.onCompleted:
    {
        // NOTE(huangchsh): 无连接UI调试代码
        // page_tab_view.addTab("COM23:rc3 vcom", Qt.createComponent("DevHelperInterface.qml"))
        // page_tab_view.addTab("COM26:rc3 vcom", Qt.createComponent("DevHelperInterface.qml"))
        page_tab_view.sigCloseTab.connect(slotLinkRemove)

        // 连接选项删除槽
        function slotLinkRemove(tab_index, tab_name)
        {
            var link_name = tab_name.slice(0, tab_name.indexOf(":"))
            link_manager_obj.removeChoiceLink(link_name)

            page_tab_view.removeTab(tab_index)
        }
    }

    Connections
    {
        target: link_manager_obj

        // 可选连接列表信号槽
        onLinkInfoMap:
        {
            waiting_view_label.visible = false
            link_listview.visible = true

            // 刷新可用连接列表
            if(link_listview_model.count)
            {
                link_listview_model.clear()
            }

            for(var elem in link_info_list)
            {
                link_listview_model.append({"name" : elem, "description" : link_info_list[elem]});
            }
        }

        // 选择连接添加信号槽
        onLinkAdded:
        {
            // 增加连接即增加窗口选项卡
            var tab_name = link_listview_model.get(link_listview.currentIndex).name + ":" + link_listview_model.get(link_listview.currentIndex).description
            page_tab_view.addTab(tab_name, Qt.createComponent("DevHelperInterface.qml"))
            // TODO(huangchsh): 需要增加输入参数
        }

        // TODO(huangchsh): 提供状态改变机制
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
        id: link_listview

        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.7; height: parent.height * 0.7

        visible: false
        clip: true
        focus: true

        spacing: 5

        // TODO(huangchsh): 高亮动画可优化
        highlightFollowsCurrentItem: true
        highlight: Component
        {
            Rectangle
            {
                y: link_listview.currentItem.y
                width: 180; height: 20
                radius: 5
                color: "lightsteelblue"

                Behavior on y
                {
                    SpringAnimation
                    {
                        spring: 3
                        damping: 0.2
                    }
                }
            }
        }

        model: ListModel
        {
            id: link_listview_model
        }
        delegate: Component
        {
            id: link_listview_delegate

            Rectangle
            {
                id: link_listview_delegate_item

                width: parent.width; height: 20

                visible: true

                color: "transparent"

                Text
                {
                    text: name + ":" + description
                    font.pixelSize: 15
                }

                // TODO(huangchsh): 可考虑增加设备背景图片
                // Image
                // {
                //     anchors.fill: parent
                    // source: image
                // }

                MouseArea
                {
                    anchors.fill: parent
                    hoverEnabled: true

                    // 悬停光标于选项时高亮
                    onEntered:
                    {
                        link_listview.currentIndex = index
                    }

                    // 单击选定选项并展开选项详细信息
                    onClicked:
                    {
                        link_listview.currentIndex = index
                        // TODO(huangchsh): 展开连接选项，显示连接详细信息
                    }

                    // 双击选定选项并创建
                    onDoubleClicked:
                    {
                        link_listview.currentIndex = index
                        link_manager_obj.createChoiceLink(name)
                    }
                }
            }
        }
    }
}