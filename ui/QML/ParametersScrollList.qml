import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2


Item
{
    id: information_column

    width: parent.width; height: parent.height * 0.46

    property alias label_name: label.text
    property string belong_link_name

    // TODO(huangchsh): 提供对外信号与槽
    Component.onCompleted:
    {
        // TODO(huangchsh): 提供无参数时调试代码
        // NOTE(huangchsh): 无参数时调试代码
    }

    Connections
    {
        target: param_manager_obj
        onSigModifyParametersSuccess:
        {
            if(belong_link_name == link_name)
            {
                for(var name in params_name_list)
                {
                    if(name == scrollview_listview_model.get(scrollview_listview.currentIndex).name)
                    {
                        scrollview_listview_model.get(scrollview_listview.currentIndex).param_value_text_field.color = "lightgray"
                    }
                }
            }
        }

        // TODO(huangchsh): 获取参数列表，显示默认字符串
        onSigFetchParametersMap: //(const QString link_name, const QVariantMap link_params_map);
        {

        }
    }

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

                        // 参数名
                        Text
                        {
                            id: param_name

                            text: qsTr(name)
                            elide: Text.ElideLeft
                            font.pixelSize: 15
                        }

                        // 参数值
                        TextField
                        {
                            id: param_value

                            width: 200

                            clip: true
                            focus: true
                            autoScroll: false
                            hoverEnabled: true
                            selectByMouse: true
                            maximumLength: 25

                            property var init_value: parameter
                            property var value: init_value

                            font.pointSize: 10

                            text: qsTr(value)
                            // placeholderTextColor: "Black"

                            background: Rectangle
                            {
                                id: param_value_text_field

                                implicitWidth: 200
                                implicitHeight: 15
                                color: "lightgray"
                                border.color: "Black"
                            }

                            onTextEdited:
                            {
                                // TODO(huangchsh): 增加文本修改判断
                                param_value_text_field.color = "transparent"

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
                                // 触发保存事件
                                param_manager_obj.modifyParameters(information_column.link_name, {name: param_value.value})
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
                                // 触发取消事件，将参数复原
                                param_value_text_field.color = "lightgray"
                                param_value_text_field.text = qsTr(param_value_text_field.last_value)
                            }
                        }
                    }
                }
            }
        }
    }
}