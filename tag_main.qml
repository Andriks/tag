import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2


ApplicationWindow {
    title: qsTr("Tag")
    width: 500
    height: 500
    visible: true


    ////////////////////////////////////////

    function game_complited(view) {
        for (var i = 0; i < 15; i++) {
            var cell_number = parseInt(view.model.get(i).text) - 1;  // parse text to get number of cell

            if (cell_number !== i)
                return false;
        }

        return true;
    }



    ///////////////////////////////////////////////////////////////////////////////////////////////

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")

            MenuItem {
                text: qsTr("&Randomize")
                onTriggered: controller.randomize()
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
            objectName: "dataModel"

            Component.onCompleted: {
                var value;

                for (var i = 0; i < 16; i++) {
                    value = {
                        color: "lightgreen",
                        opacity: i === 15 ? 0: 1,
                        text: i === 15 ? "": "%1".arg(i + 1)
                    };

                    append(value);
                }
            }
        }

        GridView {
            id: view
            objectName: "view"

            property real freeCell: 15

            function moveItem(inx1, inx2) {
                view.model.move(inx1, inx2, 1)
            }

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
                            controller.move_cell();
                        }
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
