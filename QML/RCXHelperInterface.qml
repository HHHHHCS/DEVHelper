import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Controls 2.2


ApplicationWindow
{
    id:root

    visible: true
    // width: parent.width; height: parent.height
    anchors.fill: parent

    menuBar: MenuBar
    {
        Menu
        {
            title: qsTr("&File")
            MenuItem
            {
                text: qsTr("&Open")
            }
            MenuItem
            {
                text: qsTr("&Save")
            }
            MenuItem
            {
                text: qsTr("Save &As...")
            }
            MenuSeparator { }
            MenuItem
            {
                text: qsTr("&Quit")
            }
            // Action { text: qsTr("&Open") }
            // Action { text: qsTr("&Save") }
            // Action { text: qsTr("Save &As...") }
            // MenuSeparator { }
            // Action { text: qsTr("&Quit") }
        }

        // Menu
        // {
        //     title: qsTr("&Help")
        //     Action { text: qsTr("&About") }
        // }
    }
}