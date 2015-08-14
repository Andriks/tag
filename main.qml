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

    function getRandomInt(min, max) {
        return Math.floor(Math.random() * (max - min + 1)) + min;
    }

    function randomize(view) {
        for (var i = 0; i < 500; i++) {
            var x = getRandomInt(0, 15);
            var y = getRandomInt(0, 15);

            view.model.move(x, y, 1);
        }

        // puting right value in freeCell
        for (i = 0; i < 15; i++) {
            // all cells except empty have theirs number as text
            if (!view.model.get(i).text)
                view.freeCell = i;
        }
    }

    ////////////////////////////////////////

    function game_complited(view) {
        for (var i = 0; i < 15; i++) {
            var cell_number = parseInt(view.model.get(i).text) - 1;  // parse text to get number of cell

            if (cell_number !== i)
                return false;
        }

        return true;
    }

    ////////////////////////////////////////
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

    function move_cell(view) {
        if (able_to_move(view.currentIndex, view.freeCell)) {
            var old_index = view.currentIndex;
            view.model.move(old_index, view.freeCell, 1);

            if (old_index < view.freeCell)
                view.model.move(view.freeCell-1, old_index, 1);
            else
                view.model.move(view.freeCell+1, old_index, 1);

            view.freeCell = old_index;

            if (game_complited(view)) {
                messageDialog.show("Congratulation, you win!!")
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")

            MenuItem {
                text: qsTr("&Randomize")
                onTriggered: randomize(view)
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

        ListModel {
            id: dataModel

            Component.onCompleted: {
                var value;

                for (var i = 0; i < 4; i++) {
                    for (var j = 0; j < 4; j++) {
                        var num = i * 4 + j;

                        value = {
                            id: "c%1".arg(num),
                            color: "lightgreen",
                            opacity: num === 15 ? 0: 1,
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

            anchors.fill: parent
            anchors.centerIn: parent
            cellHeight: parent.height/4
            cellWidth: parent.width/4
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
                    radius: 15
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
                            view.currentIndex = model.index;
                            move_cell(view);
                        }
                    }
                }
            }
        }
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("tag")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }

}
