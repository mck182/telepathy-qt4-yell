import Qt 4.7

Rectangle {
    border.width: 1
    border.color: "black"

    Component {
        id: conversationDelegate

        Rectangle {
            height: childrenRect.height + 8
            width: parent.width

            Image {
                id: avatar
                width: 24
                height: 24
                anchors.left: parent.left
                anchors.top: parent.top

                source: model.contactAvatar
            }
            Text {
                id: message
                anchors.left: avatar.right
                anchors.leftMargin: 4
                anchors.top: parent.top
                font.italic: model.type == "event"
                text: {
                    var time = Qt.formatTime(model.time, "hh:mm:ss");
                    var message;
                    if (model.type == "incoming_message" || model.type == "outgoing_message") {
                        message = model.contact + ": " + model.text;
                    }
                    else {
                        message = model.text;
                    }
                    "[" + time + "] " + message;
                }
            }
        }
    }

    ListView {
        id: conversation
        anchors.fill: parent
        anchors.margins: 5

        model: conversationModel
        delegate: conversationDelegate
    }
}
