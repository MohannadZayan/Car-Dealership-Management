import QtQuick
import Car_Dealership_Management

// * A single sidebar entry — icon + label, highlighted when active, animated on hover/press.
Rectangle {
    id: root

    property string icon: ""
    property string label: ""
    property bool active: false

    signal clicked()

    implicitHeight: 42
    radius: Theme.radiusSmall
    color: root.active ? Theme.accent : (hoverArea.containsMouse ? Theme.cardAlt : "transparent")
    Behavior on color { ColorAnimation { duration: Theme.animFast } }

    scale: hoverArea.pressed ? 0.97 : 1.0
    Behavior on scale { NumberAnimation { duration: 90; easing.type: Easing.OutQuad } }

    Row {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 14
        spacing: 12

        Text {
            text: root.icon
            font.pixelSize: 15
            color: root.active ? "#FFFFFF" : Theme.textMuted
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            text: root.label
            font.pixelSize: Theme.fontBody
            font.weight: root.active ? Font.DemiBold : Font.Normal
            color: root.active ? "#FFFFFF" : Theme.textPrimary
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    MouseArea {
        id: hoverArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
    }
}
