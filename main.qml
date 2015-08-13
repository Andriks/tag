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
        function randomize() {
            // some code
            return;
        }

        Menu {


            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Randomize")
                onTriggered: randomize();
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    function able_to_move(from, to) {


        return true
    }


    Rectangle {
        width: parent.height; height: parent.width
        anchors.fill: parent
        anchors.centerIn: parent

        border.width: 10

        ListModel {
            id: dataModel

            Component.onCompleted: {
                var value
                for (var i=0; i<4; i++) {
                    for (var j=0; j<4; j++) {
                        var num = i*4+j

                        value = {
                            color: num === 15 ? "white": "lightgreen",
                            x: i,
                            y: j,
                            text: "%1".arg(num+1),
                        }
                        append(value)
                    }
                }


            }

        }

        GridView {
            id: view

            property real freeCell: 8

            anchors.margins: 10
            anchors.fill: parent
            anchors.centerIn: parent
            cellHeight: parent.height/5
            cellWidth: parent.width/5
            model: dataModel
            clip: true

            move: Transition {
                NumberAnimation { properties: "x,y"; duration: 800; easing.type: Easing.OutBack }
            }


            delegate: Item {
                property var view: GridView.view


                height: view.cellHeight
                width: view.cellWidth

                Rectangle {
                    anchors.margins: 5
                    anchors.fill: parent
                    color: model.color

                    Text {
                        anchors.centerIn: parent
                        renderType: Text.NativeRendering
                        text: "%1 || %2 (%3,%4)".arg(model.text).arg(model.index).arg(model.x).arg(model.y)
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                        {
                            if (able_to_move(view.model.index, view.freeCell)) {
                                var old_index = model.index
                                var old_x = model.x
                                var old_y = model.y

                                view.model.move(model.index, model.index+1, 1)

                                view.freeCell = old_index
                                model.x = old_x
                                model.y = old_y
                            }
                        }
                    }
                }
            }
        }
    }


}
