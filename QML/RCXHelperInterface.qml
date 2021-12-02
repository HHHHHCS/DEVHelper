import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2


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

    Row
    {
        id: interface_view

        visible: false

        anchors.fill: parent

        Column
        {
            id: information_column

            width: parent.width * 0.87; height: parent.height
            
            Item
            {
                id: software_info_item

                x: parent.width * 0.01; y: parent.height * 0.01
                width: parent.width * 0.99; height: parent.height * 0.49

                Label
                {
                    id: software_info_label

                    padding: 5

                    text: qsTr("软件信息")
                    font.pixelSize: 15
                }

                Rectangle 
                {
                    x: parent.x; y: software_info_label.y + software_info_label.height + 5
                    width: parent.width * 0.98; height: parent.height - (software_info_label.y + software_info_label.height) - 30

                    border.color: "lightGray"
                    border.width: 1

                    ScrollView
                    {
                        id: software_info_scrollview

                        anchors.fill: parent

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

                                Row
                                {
                                    spacing: 10
                                    
                                    Text
                                    {
                                        padding: 5

                                        text: qsTr(name)
                                        elide: Text.ElideLeft
                                        font.pixelSize: 15
                                    }

                                    TextInput
                                    {
                                        width: 100

                                        focus: true
                                        autoScroll: false

                                        font.pointSize: 15

                                        onTextEdited:
                                        {
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

                                        width: 50; height: 15

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

                                        width: 50; height: 15

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

            Item
            {
                id: hardware_info_item

                x: parent.width * 0.01; y: parent.height - (software_info_item.y + software_info_item.height) + 5
                width: parent.width * 0.99; height: parent.height * 0.49

                Label
                {
                    id: hardware_info_label

                    padding: 5

                    text: qsTr("硬件信息")
                    font.pixelSize: 15
                }

                Rectangle 
                {
                    x: parent.x; y: hardware_info_label.y + hardware_info_label.height + 5
                    width: parent.width * 0.98; height: parent.height - (hardware_info_label.y + hardware_info_label.height) - 30
                    color: "transparent"

                    border.color: "lightGray"
                    border.width: 1

                    ScrollView
                    {
                        id: hardware_info_scrollview

                        width: parent.width; height: parent.height

                        clip: true

                        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

                        ListView
                        {
                            anchors.top: hardware_info_scrollview.top
                            anchors.topMargin: 5
                            anchors.left: hardware_info_scrollview.left
                            anchors.leftMargin: 5

                            spacing: 5

                            delegate: hardware_scroll_list_viem_component
                            model: ListModel
                            {
                                ListElement { name: "硬件版本：" }
                                ListElement { name: "    型号：" }
                                ListElement { name: "  序列号：" }
                            }

                            Component
                            {
                                id: hardware_scroll_list_viem_component

                                Row
                                {
                                    Text
                                    {
                                        padding: 5

                                        text: qsTr(name)
                                        elide: Text.ElideLeft
                                        font.pixelSize: 15
                                    }

                                    TextInput
                                    {
                                        width: 100

                                        focus: true
                                        autoScroll: false

                                        font.pointSize: 15

                                        onTextEdited:
                                        {
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

                                        width: 50; height: 15

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

                                        anchors.right: hardware_scroll_list_viem_component.right
                                        anchors.rightMargin: 10

                                        width: 50; height: 15

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
        }

        ButtonGroup
        {
            buttons: buttons_column.children
        }

        Column
        {
            id: buttons_column

            y: parent.y + 30
            height: parent.height

            spacing: 5

            Button
            {
                id: upload_button

                text: qsTr("上传")
            }

            Button
            {
                id: update_button

                text: qsTr("更新")
            }

            Button
            {
                id: cloud_url_button

                text: qsTr("云URL")
            }

            Button
            {
                id: quit_button

                text: qsTr("返回")

                onClicked: page_view_stack.pop()
            }
        }
    }    
}