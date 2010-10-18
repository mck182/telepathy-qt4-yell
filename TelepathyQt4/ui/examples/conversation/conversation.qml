import Qt 4.7

Rectangle {
    border.width: 1
    border.color: "black"

    Component {
        id: conversationDelegate

        Text {
            text: "[" + model.time + "] " + model.sender + ": " + model.text
        }
    }

    ListView {
        id: conversation
        anchors.fill: parent

        model: conversationModel
        delegate: conversationDelegate
    }
}
