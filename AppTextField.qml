import QtQuick
import QtQuick.Controls.Basic as Controls
import Car_Dealership_Management

// * Styled single-line input with a floating label above it, matching the dark theme.
// ! Root is a plain Item (not Column) specifically so implicitWidth is assignable —
// ! Column computes implicitWidth itself (read-only), and since the input Rectangle
// ! below binds its own width back to this item's width, Column would otherwise
// ! size the whole field off just the label text's width alone, which differs
// ! between e.g. "First Name" and "Last Name" and throws off even 50/50 splits
// ! in a RowLayout of same-row fields with Layout.fillWidth.
Item {
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

    // Overridable so a page (e.g. a bigger login form) can size up without
    // affecting every other page's default fields.
    property int fieldHeight: 46
    property int labelSize: Theme.fontSmall
    property int inputSize: Theme.fontBody
    property int iconSize: Theme.fontBody

    implicitWidth: 120
    implicitHeight: (labelText.visible ? labelText.implicitHeight + Theme.spacingTiny : 0) + fieldHeight

    Text {
        id: labelText
        text: root.label
        color: Theme.textMuted
        font.pixelSize: root.labelSize
        visible: root.label.length > 0
        anchors.top: parent.top
        anchors.left: parent.left
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: labelText.visible ? labelText.bottom : parent.top
        anchors.topMargin: labelText.visible ? Theme.spacingTiny : 0
        height: root.fieldHeight
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
            font.pixelSize: root.iconSize
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
            font.pixelSize: root.inputSize
            background: Item {}
        }
    }
}
