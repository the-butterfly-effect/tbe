import QtQuick 2.0

Rectangle {
    id:top
    anchors.fill: parent
    color: "transparent"

    property alias accessImage: virtual.source

    Image{
        id:virtual;
        anchors.fill: parent
        source:""
        // performance optimization: do not ask for redraw of images while resizing/rotating/dragging
        sourceSize.width:  (selectedItem != undefined && selectedItem.theDecorated===viewItem) ? selectedItem.oldWidth : width
        sourceSize.height: (selectedItem != undefined && selectedItem.theDecorated===viewItem) ? selectedItem.oldHeight: height
    }
}
