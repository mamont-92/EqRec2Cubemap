import QtQuick 2.9

Rectangle {
    id: root

    property string imageProvider: ""
    property string imageName: ""
    default property alias contents: placeholder.children

    border.width: 1
    border.color: "gray"

    function resetItemTransform(){
        placeholder.x = 0;
        placeholder.y = 0;

        if(clipRectangle.width > 0)
            placeholder.width = clipRectangle.width
        if(clipRectangle.height > 0)
            placeholder.height = clipRectangle.height
        scaleTransform.xScale = 1.0;
        scaleTransform.yScale = 1.0;
    }

    Rectangle{
        id: clipRectangle
        clip: true
        anchors.fill: parent
        anchors.margins: 2

        Item{
            id: placeholder
            x:0
            y:0

            transform: Scale {
                id: scaleTransform
            }

            MouseArea{
                id: mouseArea
                property real zoomFactor: 1.2
                anchors.fill: parent
                drag.target: placeholder
                onReleased: placeholder.Drag.drop()
                cursorShape: Qt.SizeAllCursor
                onWheel: {

                    if(wheel.angleDelta.y > 0)
                        var zoomRatio = zoomFactor
                    else
                        zoomRatio = 1.0 / zoomFactor
                    var realX = wheel.x * scaleTransform.xScale
                    var realY = wheel.y * scaleTransform.yScale
                    placeholder.x += (1-zoomRatio)*realX
                    placeholder.y += (1-zoomRatio)*realY
                    scaleTransform.xScale *=zoomRatio
                    scaleTransform.yScale *=zoomRatio
                }
            }
        }

        onWidthChanged: {
            resetItemTransform();
        }
        onHeightChanged: {
            resetItemTransform();
        }

    }
}
