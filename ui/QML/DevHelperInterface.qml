import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0


Item
{
    id: dev_helper_interface

    width: parent.width; height: parent.height

    clip: true

    property string link_name
    property string link_description

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
        id: factory
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

        // NOTE(huangchsh): 此处不用 anchors.fill: parent，原因为parent隐式宽高未确定
        implicitWidth: parent.width
        implicitHeight: parent.height
        contentWidth: parent.width
        contentHeight: parent.height

        visible: true

        header: ToolBar
        {
            background: Rectangle
            {
                anchors.fill: parent
                color: "transparent"
            }

            // TODO(huangchsh): 需要根据动作进度进行变化
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

        // TODO(huangchsh): 增加刷写固件或信息进度条
        // footer:

        // 界面行布局
        RowLayout
        {
            anchors.fill: parent

            spacing: 5

            // 参数信息列布局
            Column
            {
                id: parameters_scroll_list_column

                Layout.alignment: Qt.AlignTop
                Layout.leftMargin: 10.0
                Layout.preferredWidth: parent.width * 0.85
                Layout.fillHeight: true

                // TODO(huangchsh): 此处可改进为ListView

                // 软件信息
                ParametersScrollList
                {
                    id: software_parameters_list
                    info_type: "sw"
                    label_name: qsTr("软件参数")
                    belong_link_name: link_name
                }

                // 硬件信息
                ParametersScrollList
                {
                    id: hardware_parameters_list
                    info_type: "hw"
                    label_name: qsTr("硬件参数")
                    belong_link_name: link_name
                }
            }

            // 功能按钮列布局
            Column
            {
                id: buttons_column

                Layout.alignment: Qt.AlignTop
                Layout.rightMargin: 10.0
                Layout.fillHeight: true

                spacing: 5

                Button
                {
                    id: upload_button

                    text: qsTr("刷写")

                    onClicked:
                    {
                        firmwareChoosenDialog.open()
                    }
                }

                Button
                {
                    id: update_button

                    text: qsTr("上传")
                }

                Button
                {
                    id: factory_button

                    text: qsTr("出厂")

                    onClicked:
                    {
                        factory.open()
                    }
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
