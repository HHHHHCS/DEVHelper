import QtQuick 2.3
import QtQuick.Window 2.3
import QtQuick.Controls 2.2

// ApplicationWindow {
Window 
{
    id: root
    visible: true
    width: 800; height: 600
    title: qsTr("DEVHelper")

    Component.onCompleted: root.show()

    // // 动态加载界面
    // Loader 
    // {
    //     id: page_loader
    //     anchors.centerIn: parent

    //     active: true

    //     // sourceComponent: dev_select_interface        
    // }

    // Component.onCompleted: page_loader.sourceComponent = dev_select_interface

    // 静态栈加载界面
    StackView
    {
        id: page_view_stack

        anchors.fill: parent
        width: parent.width; height: parent.height
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