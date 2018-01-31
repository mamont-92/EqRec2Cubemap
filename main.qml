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

    RowLayout{
        id: images

        Repeater{
            id: imageRepeater
            model: ListModel{
                id: imageList

                ListElement{
                    number: 0
                    text: "vertical cross"
                    path: "media/CubemapCrossVert.png"
                }

                ListElement{
                    number: 1
                    text: "horizontal cross"
                    path: "media/CubemapCrossHort.png"
                }

                ListElement{
                    number: 2
                    text: "vertical line"
                    path: "media/CubemapLinearHor.png"
                }

                ListElement{
                    number: 3
                    text: "horizontal line"
                    path: "media/CubemapLinearHor.png"
                }
            }

            delegate:
                Rectangle{
                    border.color: "gray"
                    border.width: 1
                    height: 64
                    width: 64

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.margins: 1

                    Image {
                        anchors.fill: parent
                        anchors.margins: 1
                        source: model.path
                        fillMode: Image.PreserveAspectFit
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

    Renderer{
        id: cubemapRender
        anchors.top : parent.top
        anchors.bottom: images.top
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
