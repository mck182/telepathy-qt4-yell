import Qt 4.7

Rectangle {
    border.width: 1
    border.color: "black"

    Component {
        id: conversationDelegate

        Text {
            text: {
                var time = Qt.formatTime(model.time, "hh:mm:ss");
                "[" + time + "] " + model.sender + ": " + model.text;
            }
        }
    }

    ListView {
        id: conversation
        anchors.fill: parent

        model: conversationModel
        delegate: conversationDelegate
    }
}
