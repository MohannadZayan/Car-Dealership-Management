import QtQuick
import Car_Dealership_Management

// * Dashboard stat tile — big number, small trend/subtitle. Fades/scales in on load
// * and lifts slightly on hover, so the dashboard doesn't feel static.
Rectangle {
    id: root

    property string title: ""
    property string value: ""
    property string subtitle: ""
    property string trend: ""
    property bool trendPositive: true

    radius: Theme.radiusLarge
    color: Theme.card
    border.width: 1
    border.color: hoverArea.containsMouse ? Theme.accent : Theme.border
    implicitHeight: column.implicitHeight + Theme.spacingLarge * 2

    Behavior on border.color { ColorAnimation { duration: Theme.animFast } }

    // Single declarative binding per property (opacity/scale) — a mount flag drives
    // the initial fade/scale-in, and the same binding keeps handling hover afterward.
    property bool mounted: false

    opacity: mounted ? 1 : 0
    scale: !mounted ? 0.96 : (hoverArea.containsMouse ? 1.015 : 1.0)

    Behavior on opacity { NumberAnimation { duration: Theme.animSlow; easing.type: Easing.OutCubic } }
    Behavior on scale { NumberAnimation { duration: Theme.animSlow; easing.type: Easing.OutBack } }

    Component.onCompleted: mounted = true

    MouseArea {
        id: hoverArea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.NoButton
    }

    Column {
        id: column
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: Theme.spacingLarge
        spacing: Theme.spacingSmall

        Text {
            text: root.title
            color: Theme.textMuted
            font.pixelSize: Theme.fontSmall
        }

        Text {
            text: root.value
            color: Theme.textPrimary
            font.pixelSize: Theme.fontXLarge
            font.weight: Font.Bold
        }

        Row {
            spacing: 6
            visible: root.trend.length > 0

            Text {
                text: (root.trendPositive ? "▲ " : "▼ ") + root.trend
                color: root.trendPositive ? Theme.success : Theme.danger
                font.pixelSize: Theme.fontTiny
                font.weight: Font.DemiBold
            }
        }

        Text {
            text: root.subtitle
            color: Theme.textMuted
            font.pixelSize: Theme.fontTiny
            visible: root.subtitle.length > 0
            wrapMode: Text.WordWrap
            width: column.width
        }
    }
}
