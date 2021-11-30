import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4


Item
{
    id:root

    visible: true

    width: parent.width; height: parent.height

    MenuBar
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

        Menu
        {
            title: qsTr("&Help")
            MenuItem
            {
                text: qsTr("&About")
            }
        }
    }
}