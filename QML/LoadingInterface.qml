import QtQuick 2.7
import QtQuick.Window 2.3
import QtQuick.Controls 2.2


Window
{
    id: loading_view

    visible: true

    color: "transparent"
    title: "Loading Window"

    modality: Qt.WindowModal
    flags: Qt.SplashScreen | Qt.WindowStaysOnTopHint
    x: (Screen.width - loading_image.width) / 2; y: (Screen.height - loading_image.height) / 2
    width: loading_image.width; height: loading_image.height

    Image 
    {
        id: loading_image
                
        source: "qrc:/Images/xunyi.png"
    }

    Text
    {
        id: textCtrl

        width: contentWidth; height: contentHeight

        font.pointSize: 30

        anchors
        {
            left: loading_image.left
            bottom: loading_image.bottom
        }
    }
}