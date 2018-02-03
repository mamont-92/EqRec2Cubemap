import QtQuick 2.9

Rectangle {
    id: root

    property string imageProvider: ""
    property string imageName: ""

    border.width: 1
    border.color: "gray"

    onImageNameChanged: {
        resetImageTransform();
        reloadImage();
    }

    onImageProviderChanged: {
        resetImageTransform();
        reloadImage();
    }

    function reloadImage(){
        image.source = "image://" + imageProvider+ "/" + imageName + Math.random();
    }

    function resetImageTransform(){
        image.x = 0;
        image.y = 0;

        if(clipRectangle.width > 0)
            image.width = clipRectangle.width
        if(clipRectangle.height > 0)
            image.height = clipRectangle.height
        scaleTransform.xScale = 1.0;
        scaleTransform.yScale = 1.0;
    }

    Rectangle{
        id: clipRectangle
        clip: true
        anchors.fill: parent
        anchors.margins: 2

        Image{
            id: image
            x:0
            y:0
            fillMode: Image.PreserveAspectFit

            transform: Scale {
                id: scaleTransform
            }

            MouseArea{
                id: mouseArea
                property real zoomFactor: 1.2
                anchors.fill: parent
                drag.target: image
                onReleased: image.Drag.drop()

                onWheel: {

                    if(wheel.angleDelta.y > 0)
                        var zoomRatio = zoomFactor
                    else
                        zoomRatio = 1.0 / zoomFactor
                    var realX = wheel.x * scaleTransform.xScale
                    var realY = wheel.y * scaleTransform.yScale
                    image.x += (1-zoomRatio)*realX
                    image.y += (1-zoomRatio)*realY
                    scaleTransform.xScale *=zoomRatio
                    scaleTransform.yScale *=zoomRatio
                }
            }

        }

        onWidthChanged: {
            resetImageTransform();
        }
        onHeightChanged: {
            resetImageTransform();
        }

    }
}
