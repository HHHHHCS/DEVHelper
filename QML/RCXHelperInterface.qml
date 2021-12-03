import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2


Item
{
    id: rcx_helper_interface
    
    Label
    {
        id: waiting_view

        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        text: qsTr("等待设备接入...")
        
        font.pixelSize: 30
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
            waiting_view.visible = false
            interface_view.visible = true
        }
    }

    RowLayout
    {
        id: interface_view

        visible: false

        anchors.fill: parent

        spacing: 5

        ColumnLayout
        {
            id: information_column

            spacing: 5
            
            Item
            {
                id: software_info_item

                x: parent.width * 0.01; y: parent.height * 0.01
                width: parent.width * 0.95; height: information_column.height * 0.5

                Label
                {
                    id: software_info_label

                    padding: 5

                    text: qsTr("软件信息")
                    font.pixelSize: 15
                }

                ScrollView
                {
                    id: software_info_scrollview

                    anchors.top: software_info_label.bottom
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    width: parent.width * 0.98; height: parent.height - (software_info_label.y + software_info_label.height) - 30

                    background: Rectangle
                    {
                        border.color: "lightGray"
                        border.width: 1
                    }

                    clip: true

                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    ScrollBar.vertical.policy: ScrollBar.AlwaysOn

                    ListView
                    {
                        anchors.top: software_info_scrollview.top
                        anchors.topMargin: 5
                        anchors.left: software_info_scrollview.left
                        anchors.leftMargin: 5

                        spacing: 5

                        delegate: software_scroll_list_viem_component
                        model: ListModel
                        {
                            ListElement { name: "固件版本：" }
                            ListElement { name: "git 分支：" }
                            ListElement { name: "git 标签：" }
                        }

                        Component
                        {
                            id: software_scroll_list_viem_component

                            RowLayout
                            {
                                spacing: 5

                                Text
                                {
                                    padding: 5

                                    text: qsTr(name)
                                    elide: Text.ElideLeft
                                    font.pixelSize: 15
                                }

                                TextField
                                {
                                    width: 200;

                                    clip: true
                                    focus: true
                                    autoScroll: false
                                    hoverEnabled: true
                                    selectByMouse: true
                                    maximumLength: 25

                                    placeholderTextColor: "Black"

                                    background: Rectangle
                                    {
                                        id: view_component_text_field

                                        implicitWidth: 200
                                        implicitHeight: 15
                                        color: "transparent"
                                        border.color: "Black"
                                    }

                                    font.pointSize: 10
                                    // TODO(huangchsh): 设备连接后，获取设备信息，显示默认字符串

                                    onTextEdited:
                                    {
                                        // TODO(huangchsh): 增加文本修改判断

                                        if(length > 0)
                                        {
                                            save_button.enabled = true
                                            cancel_button.enabled = true
                                        }
                                        else
                                        {
                                            save_button.enabled = false
                                            cancel_button.enabled = false
                                        }
                                    }
                                }

                                Button
                                {
                                    id: save_button

                                    enabled: false

                                    background: Rectangle
                                    {
                                        implicitWidth: 50
                                        implicitHeight: 15
                                        color: "transparent"
                                        border.color: "Black"
                                    }

                                    text: qsTr("✔")

                                    onClicked:
                                    {
                                        // TODO(huangchsh): 触发保存事件
                                    }
                                }

                                Button
                                {
                                    id: cancel_button

                                    enabled: false

                                    anchors.right: software_scroll_list_viem_component.right
                                    anchors.rightMargin: 10

                                    background: Rectangle
                                    {
                                        implicitWidth: 50
                                        implicitHeight: 15
                                        color: "transparent"
                                        border.color: "Black"
                                    }

                                    text: qsTr("×")

                                    onClicked:
                                    {
                                        // TODO(huangchsh): 触发取消事件，将参数复原
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        ButtonGroup
        {
            buttons: buttons_column.children
        }

        ColumnLayout
        {
            id: buttons_column

            width: parent.width * 0.02

            spacing: 5

            Button
            {
                id: upload_button

                Layout.alignment: Qt.AlignCenter

                text: qsTr("上传")
            }

            Button
            {
                id: update_button

                Layout.alignment: Qt.AlignCenter

                text: qsTr("更新")
            }

            Button
            {
                id: cloud_url_button

                Layout.alignment: Qt.AlignCenter

                text: qsTr("云URL")
            }

            Button
            {
                id: quit_button

                Layout.alignment: Qt.AlignCenter

                text: qsTr("返回")

                onClicked: page_view_stack.pop()
            }
        }
    }    
}