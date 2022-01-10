import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2


Item
{
    id: information_column

    width: parent.width; height: parent.height * 0.46

    // TODO(huangchsh): 提供对外属性
    // TODO(huangchsh): 提供对外信号与槽

    ColumnLayout
    {
        anchors.fill: parent

        Label
        {
            id: label

            padding: 5

            text: qsTr("xxxx")
            font.pixelSize: 15
        }

        ScrollView
        {
            id: scrollview

            anchors.top: label.bottom
            anchors.left: parent.left
            Layout.preferredWidth: parent.width; Layout.preferredHeight: parent.height - (label.y + label.height)

            clip: true

            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn

            background: Rectangle
            {
                border.color: "lightGray"
                border.width: 0.5
            }

            ListView
            {
                id: scrollview_listview

                anchors.top: scrollview.top
                anchors.topMargin: 5
                anchors.left: scrollview.left
                anchors.leftMargin: 5

                spacing: 5

                model: ListModel
                {
                    id: scrollview_listview_model
                    // TODO(huangchsh): 动态增删改查参数，此处输出标签
                }
                delegate: Component
                {
                    id: scrollview_listview_delegate

                    Row
                    {
                        spacing: 5

                        topPadding: 5
                        bottomPadding: 5
                        leftPadding: 5
                        rightPadding: 10

                        Text
                        {
                            // TODO(huangchsh): 此处text显示参数名，并且对外公有
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

                            font.pointSize: 10
                            // TODO(huangchsh): 设备连接后，获取设备信息，显示默认字符串
                            text: qsTr(parameter)
                            // placeholderTextColor: "Black"

                            background: Rectangle
                            {
                                id: view_component_text_field

                                implicitWidth: 200
                                implicitHeight: 15
                                color: "transparent"
                                border.color: "Black"
                            }

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