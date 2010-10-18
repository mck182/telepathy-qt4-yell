import Qt 4.7

Rectangle {
    id: window
    anchors.fill: parent

    Component {
        id: accountsDelegate
        Rectangle {
            border.width: 1
            border.color: "black"
            width: parent.width - 4
            anchors.horizontalCenter: parent.horizontalCenter
            height: 20
            CheckBox {
                id: enabledCheckBox
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 10
                width: 16
                height: 16
                checked: model.enabled
                onClicked: {
                    accountsModel.setAccountEnabled(index, !model.enabled);
                }
            }
            Text {
                id: displayName
                anchors.left: enabledCheckBox.right
                // anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 10
                text: model.displayName
            }
            Text {
                id: nickname
                anchors.left: displayName.right
                // anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 10
                text: model.protocol
            }
            Text {
                id: status
                anchors.left: nickname.right
                text: model.connectionStatus
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
