import Qt 4.7

Item {
    id: editable
    property string value
    property string text: value
    signal changed(string value)
    height: display.height
    width: display.width

    Text {
        id: display
        text: editable.text
        visible: !editor.visible

        MouseArea {
            anchors.fill: parent
            onClicked: {
                editor.show();
            }
        }
    }

    TextEdit {
        id: editor
        anchors.fill: display
        visible: false
        z: editable.z + 1

        function show() {
            editor.text = editable.value;
            editor.cursorPosition = 0;
            editor.moveCursorSelection(editor.text.length);
            editor.focus = true;
            editor.visible = true;
        }

        Keys.onReturnPressed: {
            editor.visible = false;
            changed(editor.text);
        }
    }
}

