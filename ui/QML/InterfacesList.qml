import QtQuick 2.7
import QtQuick.Window 2.3
import QtQuick.Controls 2.2


ApplicationWindow
{
    id: interface_window
    visible: true
    width: 800; height: 600
    maximumWidth: width; maximumHeight: height
    minimumWidth: width; minimumHeight: height
    title: qsTr("设备助手")

    Component.onCompleted: show()

    // 静态栈加载界面
    StackView
    {
        id: page_view_stack

        anchors.fill: parent

        initialItem: DeviceSelectInterface {}
    }

    Component
    {
        id: rcx_helper_interface

        RCXHelperInterface {}
    }

    Component
    {
        id: uav_helper_interface

        UAVHelperInterface {}
    }

    Component
    {
        id: ba_helper_interface

        BAHelperInterface {}
    }
}