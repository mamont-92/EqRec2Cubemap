import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

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
                console.log("Open btn clicked")
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
                console.log("Save as btn clicked")
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
}
