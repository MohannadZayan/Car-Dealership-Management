import QtQuick
import Car_Dealership_Management

// * Solid accent-filled button — the main "do the thing" action on a page/dialog.
Rectangle {
    id: root

    property string text: ""
    property bool enabled: true
    property bool busy: false
    property color baseColor: Theme.accent
    property color hoverColor: Theme.accentHover
    property color pressedColor: Theme.accentPressed

    signal clicked()

    implicitHeight: 42
    implicitWidth: label.implicitWidth + 36
    radius: Theme.radiusSmall
    color: !root.enabled ? Theme.border
           : mouseArea.pressed ? root.pressedColor
           : mouseArea.containsMouse ? root.hoverColor
           : root.baseColor
    opacity: root.busy ? 0.75 : 1.0

    Behavior on color { ColorAnimation { duration: Theme.animFast } }
    Behavior on opacity { NumberAnimation { duration: Theme.animFast } }

    scale: mouseArea.pressed ? 0.96 : 1.0
    Behavior on scale { NumberAnimation { duration: 90; easing.type: Easing.OutQuad } }

    Row {
        anchors.centerIn: parent
        spacing: 8

        Text {
            id: label
            text: root.busy ? "Please wait…" : root.text
            color: root.enabled ? "#FFFFFF" : Theme.textMuted
            font.pixelSize: Theme.fontBody
            font.weight: Font.DemiBold
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: (root.enabled && !root.busy) ? Qt.PointingHandCursor : Qt.ArrowCursor
        enabled: root.enabled && !root.busy
        onClicked: root.clicked()
    }
}
