import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0

import SceneGraphRendering 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    property var graphicFileExtensions: ["Graphic files (*.png *.jpg *.bmp)"]

    Item{
        id: buttons

        width: 200
        height: 50

        anchors.bottom: parent.bottom
        anchors.right: parent.right

        Button{
            id: openButton
            text: qsTr("Open")

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 1

            width: parent.width / 2 - anchors.margins*2

            onClicked: {
                openDialog.open()
            }
        }

        Button{
            id: saveAsButton
            text: qsTr("Save as...")

            anchors.left: openButton.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 1

            onClicked: {
                saveDialog.open()
            }
        }
    }

    NumParameter{
        id: yRotationParameter

        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: buttons.left
        anchors.margins: 1

        minValue: 0.0
        maxValue: 360.0

        height: buttons.height

        onValueModified: {
            cubemapRender.yRotation = value;
        }
    }


    RowLayout{
        id: images

        Repeater{
            id: imageRepeater
            model: ListModel{
                id: imageList

                ListElement{
                    scheme: Renderer.VerticalCross
                    text: "vertical cross"
                    path: "media/CubemapCrossVert.png"
                }

                ListElement{
                    scheme: Renderer.HorizontalCross
                    text: "horizontal cross"
                    path: "media/CubemapCrossHort.png"
                }

                ListElement{
                    scheme: Renderer.VerticalLine
                    text: "vertical line"
                    path: "media/CubemapLinearVert.png"
                }

                ListElement{
                    scheme: Renderer.HorizontalLine
                    text: "horizontal line"
                    path: "media/CubemapLinearHor.png"
                }
            }

            delegate:
                Rectangle{
                border.color: (cubemapRender.scheme == model.scheme) ? "blue" : "gray"
                border.width: (cubemapRender.scheme == model.scheme) ? 2 : 1
                height: 64
                width: 64

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 1

                Image {
                    anchors.fill: parent
                    anchors.margins: 2
                    source: model.path
                    fillMode: Image.PreserveAspectFit
                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.ClosedHandCursor
                        onClicked: {
                            cubemapRender.scheme = model.scheme
                        }
                    }
                }
            }


        }

        height: 110
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: buttons.top
        anchors.bottomMargin: 10
    }

    function extractPathFromURL(url){
        var path = url.toString();
        path = path.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"");
        return path;
    }

    FileDialog {
        id: openDialog
        fileMode: FileDialog.OpenFile
        nameFilters: graphicFileExtensions
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: {
            cubemapRender.loadFromFileEquRectMap( extractPathFromURL(file) )
        }
    }

    FileDialog {
        id: saveDialog
        fileMode: FileDialog.SaveFile
        nameFilters: graphicFileExtensions
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted:  {
            cubemapRender.saveToFileCubemap( extractPathFromURL(file) )
        }
    }

    Item{
        id: mapViews

        anchors.top : parent.top
        anchors.bottom: images.top
        anchors.left: parent.left
        anchors.right: parent.right

        TransformViewer{
            id: sourceView
            anchors.top : parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height / 2

            Image{
                property string imageProvider: "maps"
                property string imageName: "equirect"
                anchors.fill:parent
                id: cubeImage
                fillMode: Image.PreserveAspectFit
                source : "image://" + imageProvider+ "/" + imageName + Math.random();

                function reloadImage(){
                    source = "image://" + imageProvider+ "/" + imageName + Math.random();
                }
            }

        }

        TransformViewer{
            id: cubemapView
            anchors.top : sourceView.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            Renderer{
                id: cubemapRender
                anchors.fill: parent
                textureFollowsItemSize: false

                onImageLoaded: {
                    cubeImage.reloadImage();
                }
            }

        }
    }
}
