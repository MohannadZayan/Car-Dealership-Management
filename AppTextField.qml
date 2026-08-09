import QtQuick
import QtQuick.Controls.Basic as Controls
import Car_Dealership_Management

// * Styled single-line input with a floating label above it, matching the dark theme.
Column {
    id: root

    property alias text: input.text
    property alias validator: input.validator
    property alias echoMode: input.echoMode
    property alias inputField: input
    property string label: ""
    property string placeholder: ""
    property string icon: ""
    property bool isPassword: false
    property bool error: false

    spacing: Theme.spacingTiny

    Text {
        text: root.label
        color: Theme.textMuted
        font.pixelSize: Theme.fontSmall
        visible: root.label.length > 0
    }

    Rectangle {
        width: parent.width
        height: 46
        radius: Theme.radiusSmall
        color: Theme.surface
        border.width: 1
        border.color: root.error ? Theme.danger : (input.activeFocus ? Theme.accent : Theme.border)

        Behavior on border.color { ColorAnimation { duration: Theme.animFast } }

        Text {
            id: iconText
            visible: root.icon.length > 0
            text: root.icon
            color: input.activeFocus ? Theme.accent : Theme.textMuted
            font.pixelSize: Theme.fontBody
            anchors.left: parent.left
            anchors.leftMargin: 14
            anchors.verticalCenter: parent.verticalCenter
            Behavior on color { ColorAnimation { duration: Theme.animFast } }
        }

        Controls.TextField {
            id: input
            anchors.fill: parent
            anchors.leftMargin: root.icon.length > 0 ? iconText.width + 24 : 0
            leftPadding: root.icon.length > 0 ? 0 : 14
            rightPadding: 14
            verticalAlignment: TextInput.AlignVCenter
            color: Theme.textPrimary
            placeholderText: root.placeholder
            placeholderTextColor: Theme.textMuted
            echoMode: root.isPassword ? TextInput.Password : TextInput.Normal
            selectByMouse: true
            font.pixelSize: Theme.fontBody
            background: Item {}
        }
    }
}
