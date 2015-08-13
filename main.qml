import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("Tag")
    width: 500
    height: 500
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&TODO")
//                onTriggered: messageDialog.show(qsTr("Open action triggered"));
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

        border.width: 10

        ListModel {
            id: dataModel

            ListElement { text: "1"; location: 1 }
            ListElement { text: "2"; location: 2 }
            ListElement { text: "3"; location: 3 }
            ListElement { text: "4"; location: 4 }
            ListElement { text: "5"; location: 5 }
            ListElement { text: "6"; location: 6 }
            ListElement { text: "7"; location: 7 }
            ListElement { text: "8"; location: 8 }
            ListElement { text: "9"; location: 9 }

        }


        GridView {
            id: view

            property real freeCell: 9
            currentIndex: 8

            anchors.margins: 10
            anchors.fill: parent
            anchors.centerIn: parent
            cellHeight: parent.height/4
            cellWidth: parent.width/4
            model: dataModel
            clip: true


            delegate: Item {
                property var view: GridView.view
                property var isCurrent: GridView.isCurrentItem


                height: view.cellHeight
                width: view.cellWidth

                Rectangle {
                    anchors.margins: 5
                    anchors.fill: parent
                    color: isCurrent ? "white" : "lightgreen"

                    Text {
                        anchors.centerIn: parent
                        renderType: Text.NativeRendering
                        text: isCurrent ? "" : "%1".arg(model.text)
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                        {

                            view.currentIndex = model.index
                        }

                    }
                }
            }
        }
    }


}
