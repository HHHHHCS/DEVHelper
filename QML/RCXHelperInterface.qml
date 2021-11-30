import QtQuick 2.3
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
            width: parent.width * 0.8; height: parent.height
        }

        ButtonGroup
        {
            buttons: buttons_column.children
        }

        Column
        {
            id: buttons_column

            x: parent.width * 0.8; y: 0
            width: parent.width * 0.2; height: parent.height

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