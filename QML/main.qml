import QtQuick 2.7
import QtQuick.Window 2.3


QtObject 
{
    id: root

    property QtObject $loading_view: LoadingInterface{}

    property var loader: Loader
    {
        asynchronous: true
        source: "qrc:/QML/QML/InterfacesList.qml"
        active: false
        onLoaded: 
        {
            $loading_view.delay();
        }
    }

    Component.onCompleted:
    {
        loader.active = true;
    }
}