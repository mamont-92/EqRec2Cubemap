import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

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
}
