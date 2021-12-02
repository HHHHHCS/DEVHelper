import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2


Item
{
    id: rcx_helper_interface

    width: parent.width; height: parent.height

    Row 
    {
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

                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.topMargin: 5
                    anchors.leftMargin: 5

                    text: qsTr("软件信息")
                    font.pixelSize: 15
                }

                Rectangle 
                {
                    x: parent.x; y: software_info_label.y + software_info_label.height + 5
                    width: parent.width * 0.98; height: parent.height - (software_info_label.y + software_info_label.height) - 30
                    color: "transparent"

                    border.color: "lightGray"
                    border.width: 1

                    ScrollView
                    {
                        id: software_info_scrollview

                        anchors.fill: parent

                        clip: true

                        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

                        //TODO(huangchsh): 增加文本输入框
                        ListView
                        {
                            anchors.top: parent.top
                            anchors.topMargin: 5
                            anchors.left: parent.left
                            anchors.leftMargin: 5
                            anchors.centerIn: parent

                            spacing: 5

                            delegate: scroll_list_viem_component
                            model: ListModel
                            {
                                ListElement { name: "固件版本" }
                                ListElement { name: "git 分支" }
                                ListElement { name: "git 标签" }
                            }

                            Component
                            {
                                id: scroll_list_viem_component
                                
                                Rectangle
                                {
                                    width: software_info_scrollview.width; height: 15

                                    Row
                                    {
                                        anchors.fill: parent

                                        Text
                                        {
                                            text: qsTr(name)
                                            font.pixelSize: 13
                                        }

                                        Rectangle
                                        {
                                            border.color: "Black"
                                            border.width: 1

                                            TextEdit
                                            {

                                            }
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

                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.topMargin: 5
                    anchors.leftMargin: 5

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

                        //TODO(huangchsh): 增加文本输入框
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

            x: information_column.x + information_column.width;
            width: rcx_helper_interface.width - information_column.width; height: parent.height

            spacing: 3

            Button
            {
                id: upload_button

                text: "Upload"
            }

            Button
            {
                id: update_button

                text: "Update"
            }

            Button
            {
                id: cloud_url_button

                text: "Cloud_URL"
            }

            Button
            {
                id: quit_button

                text: "Quit"
            }
        }
    }    
}