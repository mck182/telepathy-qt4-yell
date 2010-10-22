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
                Text {
                    id: status
                    anchors.left: displayName.right
                    anchors.margins: 10
                    text: "(" + model.status + ")"
                    visible: !statusEdit.visible
                    MouseArea {
                        anchors.fill: parent
                        onClicked: statusEdit.show()
                    }
                }
                TextEdit {
                    id: statusEdit
                    anchors.fill: status
                    visible: false
                    z: 1

                    function show() {
                        statusEdit.text = model.status;
                        statusEdit.cursorPosition = 0;
                        statusEdit.moveCursorSelection(statusEdit.text.length);
                        statusEdit.focus = true;
                        statusEdit.visible = true;
                    }

                    Keys.onReturnPressed: {
                        statusEdit.visible = false;
                        accountsModel.setAccountStatus(index, statusEdit.text);
                    }
                }
                Text {
                    id: statusMessage
                    anchors.left: parent.left
                    anchors.top: displayName.bottom
                    anchors.margins: 10
                    text: "message: " + model.statusMessage
                    visible: !statusMessageEdit.visible

                    MouseArea {
                        anchors.fill: parent
                        onClicked: statusMessageEdit.show()
                    }
                }
                TextEdit {
                    id: statusMessageEdit
                    anchors.fill: statusMessage
                    visible: false
                    z: 1

                    function show() {
                        statusMessageEdit.text = model.statusMessage;
                        statusMessageEdit.cursorPosition = 0;
                        statusMessageEdit.moveCursorSelection(statusMessageEdit.text.length);
                        statusMessageEdit.focus = true;
                        statusMessageEdit.visible = true;
                    }

                    Keys.onReturnPressed: {
                        statusMessageEdit.visible = false;
                        accountsModel.setAccountStatusMessage(index, statusMessageEdit.text);
                    }
                }
                Text {
                    id: nickname
                    anchors.left: parent.left
                    anchors.top: statusMessage.bottom
                    anchors.margins: 10
                    visible: !nicknameEdit.visible
                    text: model.nickname

                    MouseArea {
                        anchors.fill: parent
                        onClicked: nicknameEdit.show()
                    }
                }
                TextEdit {
                    id: nicknameEdit
                    anchors.fill: nickname
                    visible: false
                    z: 1

                    function show() {
                        nicknameEdit.text = model.nickname
                        nicknameEdit.cursorPosition = 0;
                        nicknameEdit.moveCursorSelection(nicknameEdit.text.length);
                        nicknameEdit.focus = true;
                        nicknameEdit.visible = true;
                    }

                    Keys.onReturnPressed: {
                        nicknameEdit.visible = false;
                        accountsModel.setAccountNickname(index, nicknameEdit.text);
                    }
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
