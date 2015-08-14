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


    function getPoint(p) {
        var x_ = Math.floor(p / 4);
        var y_ = p - (x_ * 4);

        return {x: x_,
                y: y_};
    }

    function eq(p1, p2) {
        if (p1.x === p2.x && p1.y === p2.y)
            return true;

        return false;
    }


    function able_to_move(from, to) {
        var p_from = getPoint(from);
        var p_to = getPoint(to);

        if (eq({x: p_from.x, y: p_from.y - 1}, p_to))
            return true;

        if (eq({x: p_from.x, y: p_from.y + 1}, p_to))
            return true;

        if (eq({x: p_from.x - 1, y: p_from.y}, p_to))
            return true;

        if (eq({x: p_from.x + 1, y: p_from.y}, p_to))
            return true;


        return false;
    }


    Rectangle {
        width: parent.height; height: parent.width
        anchors.fill: parent
        anchors.centerIn: parent

        border.width: 10

        ListModel {
            id: dataModel

            Component.onCompleted: {
                var value;

                for (var i = 0; i < 4; i++) {
                    for (var j = 0; j < 4; j++) {
                        var num = i * 4 + j;

                        value = {
                            color: "lightgreen",
                            opacity: num === 15 ? 0.3: 1,
                            text: num === 15 ? "": "%1".arg(num + 1)
                        };

                        append(value);
                    }
                }
            }
        }

        GridView {
            id: view

            property real freeCell: 15

            anchors.margins: 10
            anchors.fill: parent
            anchors.centerIn: parent
            cellHeight: parent.height/5
            cellWidth: parent.width/5
            model: dataModel
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
                    anchors.fill: parent
                    color: model.color
                    opacity: model.opacity

                    Text {
                        anchors.centerIn: parent
                        renderType: Text.NativeRendering
                        text: model.text
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                        {
                            if (able_to_move(model.index, view.freeCell)) {
                                var old_index = model.index;
                                view.model.move(old_index, view.freeCell, 1);

                                if (old_index < view.freeCell)
                                    view.model.move(view.freeCell-1, old_index, 1);
                                else
                                    view.model.move(view.freeCell+1, old_index, 1);

                                view.freeCell = old_index;
                            }
                        }
                    }
                }
            }
        }
    }


}
