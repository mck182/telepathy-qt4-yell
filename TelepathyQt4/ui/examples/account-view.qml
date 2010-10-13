import Qt 4.7

Rectangle {
    id: window
    width: 500
    height: 500

    Component.onCompleted: {
    }

    Component {
        id: accountsDelegate
        Rectangle {
            border.width: 1
            border.color: "black"
            width: parent.width - 4
            anchors.horizontalCenter: parent.horizontalCenter
            height: childrenRect.height
            Text {
                id: text
                anchors.left: parent.left
                anchors.right: parent.right
                // anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 10
                text: model.displayName
            }
            Text {
                id: nickname
                anchors.right: parent.right
                // anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 10
                text: model.protocol
            }
        }
    }

    ListView {
        id: accounts
        anchors.fill: parent

        model: accountsModel
        delegate: accountsDelegate
    }
}
