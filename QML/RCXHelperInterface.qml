import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0


Item
{
    id: rcx_helper_interface

    clip: true
    
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

    FileDialog
    {
        id: firmwareChoosenDialog
        visible: false
        modality: Qt.WindowModal
        title: qsTr("选择更新固件")
        sidebarVisible: true
        selectExisting: true
        selectMultiple: false
        selectFolder: false
        nameFilters: ["Firmware files (*.bin *.binary *.hex *.srec *.elf *.out *.axf *.tsv)"]
        selectedNameFilter: "Firmware files (*.bin *.binary *.hex *.srec *.elf *.out *.axf *.tsv)"
        onAccepted:
        {
            console.log("Accepted: " + fileUrl)
            // TODO(huangchsh): 将固件本地地址fileUrl传出
        }
        onRejected:
        {
            console.log("Rejected firmwareChoosenDialog")
        }
    }

    MessageDialog
    {
        id: recover_factory
        visible: false
        modality: Qt.WindowModal
        title: qsTr("提示")
        text: qsTr("是否恢复出厂设置？")
        icon: StandardIcon.Question
        // informativeText: customizeInformativeText.checked ? informativeTextField.text : ""
        // detailedText: customizeDetailedText.checked ? detailedTextField.text : ""

        standardButtons: StandardButton.Yes | StandardButton.No
        onYes:
        {
            // TODO(huangchsh): 将恢复出厂信号传出
            console.log("Recovery factory firmware and configure which about it.")
        }
    }

    Page
    {
        id: interface_view

        anchors.fill: parent

        visible: false

        header: ToolBar
        {
            background: Rectangle
            {
                anchors.fill: parent
                color: "transparent"
            }

            ColumnLayout
            {
                anchors.fill: parent

                Layout.minimumWidth: width
                Layout.minimumHeight: height

                RowLayout
                {
                    ToolButton
                    {
                        text: qsTr("<")
                        onClicked:
                        {
                            page_view_stack.pop();
                        }
                    }

                    Label
                    {
                        Layout.fillWidth: true

                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter

                        text: qsTr("RCX 设备助手")
                        font.pixelSize: 20
                    }

                    ToolButton
                    {
                        text: qsTr("···")
                        onClicked:
                        {
                            menu.open()
                        }
                    }
                }

                ProgressBar
                {
                    id: control

                    width: parent.width * 0.95; height: 5

                    from: 0.0
                    to: 1.0

                    background: Rectangle
                    {
                        implicitWidth: control.width; implicitHeight: control.height
                    }

                    contentItem: Rectangle
                    {
                        width: control.visualPosition * control.width; height: control.height
                        radius: 10
                        LinearGradient
                        {
                            anchors.fill: parent
                            end: Qt.point(width, 0)      // 横向渐变
                            gradient: Gradient
                            {
                                GradientStop {  position: 0.0;    color: "lightGreen" }
                                GradientStop {  position: 1.0;    color: "Orange" }
                            }
                        }
                    }

                    Timer
                    {
                        interval: 1000
                        repeat: true
                        running: true
                        onTriggered:
                        {
                            if(parent.value < 1.0)
                            {
                                parent.value += 0.2
                            }
                            else
                            {
                                stop()
                                parent.value = 0.0
                            }
                        }
                    }
                }
            }

        }

        // TODO(huangchsh): 增加刷写固件或信息进度条
        // footer:

        RowLayout
        {
            anchors.fill: parent

            spacing: 5

            ColumnLayout
            {
                id: information_column

                Layout.leftMargin: 10.0
                Layout.maximumWidth: parent.width * 0.85; Layout.maximumHeight: parent.height
                Layout.minimumWidth: parent.width * 0.85; Layout.minimumHeight: parent.height * 0.5
                spacing: 5

                Item
                {
                    id: software_info_item

                    Layout.preferredWidth: parent.width; Layout.preferredHeight: parent.height
                    Layout.alignment: Qt.AlignHCenter

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
                        width: parent.width; height: parent.height - (software_info_label.y + software_info_label.height)

                        background: Rectangle
                        {
                            border.color: "lightGray"
                            border.width: 0.5
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

                                        // placeholderTextColor: "Black"

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

            ColumnLayout
            {
                id: buttons_column

                Layout.maximumWidth: parent.width - information_column.width; Layout.maximumHeight: parent.height
                Layout.minimumWidth: Layout.maximumWidth; Layout.minimumHeight: Layout.maximumHeight

                spacing: 5

                Column
                {
                    id: buttons_item

                    Layout.maximumWidth: parent.width; Layout.maximumHeight: parent.height
                    Layout.alignment: Qt.AlignTop
                    Layout.topMargin: 30.0

                    spacing: 5

                    Button
                    {
                        id: upload_button

                        text: qsTr("上传")

                        onClicked:
                        {
                            firmwareChoosenDialog.open()
                        }
                    }

                    Button
                    {
                        id: update_button

                        text: qsTr("更新")
                    }

                    Button
                    {
                        id: recover_factory_button

                        text: qsTr("恢复出厂")

                        onClicked:
                        {
                            recover_factory.open()
                        }
                    }

                    Button
                    {
                        id: first_factory_button

                        text: qsTr("初次出厂")
                    }

                    Button
                    {
                        id: cloud_url_button

                        text: qsTr("云URL")
                    }
                }
            }
        }
    }
}
