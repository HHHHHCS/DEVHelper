import QtQuick 2.7
import QtQuick.Controls 2.2


Item
{
    id: device_select_interface

    width: parent.width; height: parent.height * 0.6
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.topMargin: parent.height / 8
    anchors.bottomMargin: anchors.topMargin    

    PathView 
    {
        id: view
        width: parent.width * 0.6; height: parent.height * 0.6
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: parent.height / 8
        anchors.bottomMargin: anchors.topMargin
        anchors.leftMargin: parent.width / 8
        anchors.rightMargin: anchors.leftMargin
        clip: true

        delegate: flipCardDelegate
        model: ListModel {
            ListElement { name: "RCX"; image: "qrc:/Images/RC3.png"; }
            ListElement { name: "UAV"; image: "qrc:/Images/UAV.png"; }
            // ListElement { name: "BA"; image: ""; }
        }

        pathItemCount: 3
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        highlightRangeMode: PathView.StrictlyEnforceRange
        interactive: false

        path: Path 
        {
            // 左选项
            startX: 0; startY: view.height * 0.5
            PathAttribute { name: "itemAngle"; value: -30.0; }
            PathAttribute { name: "itemScale"; value: 0.5; }
            PathAttribute { name: "itemZ"; value: 0 }
            // 中选项
            PathLine { x: view.width * 0.4; y: view.height * 0.5; }
            PathPercent { value: 0.48; }
            PathLine { x: view.width * 0.5; y: view.height * 0.5; }
            PathAttribute { name: "itemAngle"; value: 0.0; }
            PathAttribute { name: "itemScale"; value: 1.0; }
            PathAttribute { name: "itemZ"; value: 100 }
            // 右选项
            PathLine { x: view.width * 0.6; y: view.height * 0.5; }
            PathPercent { value: 0.52; }
            PathLine { x: view.width; y: view.height * 0.5; }
            PathAttribute { name: "itemAngle"; value: 30.0; }
            PathAttribute { name: "itemScale"; value: 0.5; }
            PathAttribute { name: "itemZ"; value: 0 }
        }

        Component 
        {
            id: flipCardDelegate

            Item 
            {
                width: parent.width / 2; height: parent.height

                property variant is_on_path: PathView.onPath
                property variant is_current_item: PathView.isCurrentItem
                property variant rotY: PathView.itemAngle

                visible: is_on_path
                opacity: is_current_item ? 1 : 0.45
                scale: PathView.itemScale
                z: PathView.itemZ

                transform: Rotation 
                {
                    origin 
                    { 
                        x: parent.width / 2
                        y: parent.height / 2
                    }
                    axis 
                    {
                        x: 0 
                        y: 1 
                        z: 0 
                    } 
                    angle: rotY
                }

                Rectangle 
                {
                    id: path_view_item
                    anchors.fill: parent

                    Text
                    {
                        anchors.centerIn: parent
                        text: name
                        font.pixelSize: 30
                    }

                    Image
                    {
                        anchors.fill: parent
                        source: image
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered:
                        {                            
                            if(is_current_item)
                            {
                                // TODO(huangchsh): 增加图片渲染
                            }
                        }

                        onExited:
                        {
                            if(is_current_item)
                            {
                                // TODO(huangchsh): 增加图片渲染
                            }
                        }

                        onClicked: 
                        {
                            if(is_current_item)
                            {
                                if(name == "RCX") page_view_stack.push(rcx_helper_interface)
                                else if(name == "UAV") page_view_stack.push(uav_helper_interface)
                                else if(name == "BA") page_view_stack.push(ba_helper_interface)
                            }
                        }
                    }
                }
            }
        }
    }
    // 左切
    Button 
    {
        id: left_button

        width: parent.width * 0.2; height: parent.height
        anchors.left: parent.left

        background: Image
        {
            source: "qrc:/Images/left_button.jpg"
        }

        onClicked: 
        {
            view.decrementCurrentIndex();            
        }
    }
    // 右切
    Button 
    {
        id: right_button

        width: parent.width * 0.2; height: parent.height
        anchors.right: parent.right

        background: Image
        {
            source: "qrc:/Images/right_button.png"
        }

        onClicked: 
        {
            view.incrementCurrentIndex();
        }
    }
}