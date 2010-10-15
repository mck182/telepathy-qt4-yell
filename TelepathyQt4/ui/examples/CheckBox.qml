import Qt 4.7

Rectangle {
    id: checkbox
    border.width: 1
    border.color: "gray"

    property bool checked: false

    signal clicked()

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: {
            checkbox.checked = !checkbox.checked
            checkbox.clicked()
        }
    }

    Image {
        anchors.fill: parent
        anchors.margins: 2
        source: "cross.png"

        visible: checkbox.checked
    }
}
