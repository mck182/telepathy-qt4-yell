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
            height: details.height
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
            Item {
                id: details
                anchors.left: enabledCheckBox.right
                anchors.margins: 10
                height: childrenRect.height

                Text {
                    id: displayName
                    anchors.left: parent.left
                    anchors.margins: 10
                    text: model.displayName
                }
                EditableText {
                    id: status
                    anchors.left: displayName.right
                    anchors.margins: 10
                    text: "(" + value + ")"
                    value: model.status
                    onChanged: accountsModel.setAccountStatus(index, value)
                }
                EditableText {
                    id: statusMessage
                    anchors.left: parent.left
                    anchors.top: displayName.bottom
                    anchors.margins: 10
                    text: "message: " + value
                    value: model.statusMessage
                    onChanged: accountsModel.setAccountStatusMessage(index, value)
                }
                EditableText {
                    id: nickname
                    anchors.left: parent.left
                    anchors.top: statusMessage.bottom
                    anchors.margins: 10
                    value: model.nickname
                    onChanged: accountsModel.setAccountNickname(index, value)
                }
            }
        }
    }

    ListView {
        id: accounts
        anchors.top: parent.top
        width: parent.width
        height: 100

        model: accountsModel
        delegate: accountsDelegate
    }

    ContactsView {
        anchors.top: accounts.bottom

    }

}
