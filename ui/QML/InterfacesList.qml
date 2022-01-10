import QtQuick 2.7
import QtQuick.Window 2.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
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

    // 使用选项卡方式加载切换界面
    TabView
    {
        id: page_tab_view

        anchors.fill: parent

        Tab
        {
            active: true
            asynchronous: true

            title: qsTr("可选连接列表")
        }

        style: TabViewStyle
        {
            tabsMovable: true

            frameOverlap: 1
            tab: Rectangle
            {
                color: "transparent"
                border.color:  "black"
                implicitWidth: Math.max(text.width + 4, 80)
                implicitHeight: 20
                radius: 5
                Text
                {
                    id: text
                    anchors.centerIn: parent
                    text: styleData.title
                    color: "black"
                    font.bold: styleData.selected
                }
            }
            frame: ScanLinksListInterface {}
        }
    }
}