import QtQuick
import Car_Dealership_Management

// * Outline/transparent button — secondary actions (Cancel, row-level Edit/Delete).
Rectangle {
    id: root

    property string text: ""
    property bool enabled: true
    property color tint: Theme.textPrimary // set to Theme.danger for destructive actions

    signal clicked()

    implicitHeight: 42
    implicitWidth: label.implicitWidth + 32
    radius: Theme.radiusSmall
    color: mouseArea.containsMouse ? Theme.cardAlt : "transparent"
    border.width: 1
    border.color: mouseArea.containsMouse ? root.tint : Theme.border

    Behavior on color { ColorAnimation { duration: Theme.animFast } }
    Behavior on border.color { ColorAnimation { duration: Theme.animFast } }

    scale: mouseArea.pressed ? 0.96 : 1.0
    Behavior on scale { NumberAnimation { duration: 90; easing.type: Easing.OutQuad } }

    Text {
        id: label
        anchors.centerIn: parent
        text: root.text
        color: root.enabled ? root.tint : Theme.textMuted
        font.pixelSize: Theme.fontBody
        font.weight: Font.Medium
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: root.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
        enabled: root.enabled
        onClicked: root.clicked()
    }
}
