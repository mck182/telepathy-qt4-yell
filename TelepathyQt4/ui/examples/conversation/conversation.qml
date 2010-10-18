import Qt 4.7

Rectangle {
    border.width: 1
    border.color: "black"

    Component {
        id: conversationDelegate

        Text {
            text: model.text
        }
    }

    ListModel {
        id: dummyModel

        ListElement { text: "1" }
        ListElement { text: "2" }
        ListElement { text: "3" }
        ListElement { text: "4" }
    }

    ListView {
        id: conversation
        anchors.fill: parent

        model: conversationModel
        delegate: conversationDelegate
    }
}
