import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("Tag")
    width: 500
    height: 500
    visible: true

    ///////////////////////////////////////////////////////////////////////////////////////////////

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")

            MenuItem {
                text: qsTr("&Randomize")
                onTriggered: dataModel.randomize()
            }

            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    Rectangle {
        width: parent.height; height: parent.width
        anchors.fill: parent
        anchors.centerIn: parent

        GridView {
            id: view
            objectName: "view"
            model: dataModel

            anchors.fill: parent
            anchors.centerIn: parent
            cellHeight: parent.height / 4
            cellWidth: parent.width / 4
            clip: true

            move: Transition {
                NumberAnimation { properties: "x,y"; duration: 800; easing.type: Easing.OutBounce }
            }

            delegate: Item {
                property var view: GridView.view

                height: view.cellHeight
                width: view.cellWidth

                Rectangle {
                    anchors.margins: 5
                    radius: 15
                    anchors.fill: parent
                    color: model.color
                    opacity: model.opacity

                    Text {
                        anchors.centerIn: parent
                        renderType: Text.NativeRendering
                        text: model.name
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: dataModel.moveCell(model.index)
                    }
                }
            }

        }
    }

    MessageDialog {
        id: messageDialog
        objectName: "messageDialog"
        title: qsTr("tag")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }

}

