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
        sourceSize.width:  width
        sourceSize.height: height
    }
}
