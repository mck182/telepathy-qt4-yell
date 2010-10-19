import Qt 4.7

Item {
    height: 300
    width: 400

    Component {
        id: contactsDelegate
        Rectangle {
            border.width: 1
            border.color: "black"
            width: parent.width - 4
            anchors.horizontalCenter: parent.horizontalCenter
            height: childrenRect.height
            Text {
                id: aliasName
                anchors.left: parent.left
                // anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 10
                text: model.aliasName
            }
            Text {
                id: presenceMessage
                anchors.left: aliasName.right
                // anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 10
                text: " - " + model.presenceMessage
            }
            Text {
                id: status
                anchors.left: presenceMessage.right
                text: " - " + model.presenceStatus
            }

        }
    }

    ListView {
        id: contactsList
        anchors.fill: parent

        model: contactsListModel
        delegate: contactsDelegate
    }
}

