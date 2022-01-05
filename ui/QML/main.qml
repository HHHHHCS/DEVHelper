import QtQuick 2.7


QtObject
{
    id: root

    property QtObject $loading_view: LoadingInterface{}

    property var loader: Loader
    {
        asynchronous: true
        source: "qrc:/QML/InterfacesList.qml"
        active: false
        onLoaded:
        {
            $loading_view.close()
        }
    }

    Component.onCompleted:
    {
        loader.active = true;
    }
}