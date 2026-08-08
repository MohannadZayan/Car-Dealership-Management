import QtQuick
import QtQuick.Layouts
import Car_Dealership_Management

// * Persistent header: page title on the left, identity + theme toggle on the right.
Rectangle {
    id: root

    property string title: ""

    color: Theme.background

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: Theme.border
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: Theme.spacingXLarge
        anchors.rightMargin: Theme.spacingXLarge
        spacing: Theme.spacingMedium

        Text {
            text: root.title
            color: Theme.textPrimary
            font.pixelSize: Theme.fontLarge
            font.weight: Font.Bold
            Layout.fillWidth: true
        }

        Column {
            spacing: 1
            Layout.alignment: Qt.AlignVCenter

            Text {
                text: AppController.currentEmployeeName
                color: Theme.textPrimary
                font.pixelSize: Theme.fontSmall
                font.weight: Font.DemiBold
                horizontalAlignment: Text.AlignRight
                anchors.right: parent.right
            }
            Text {
                text: AppController.currentEmployeeRole
                color: Theme.textMuted
                font.pixelSize: Theme.fontTiny
                horizontalAlignment: Text.AlignRight
                anchors.right: parent.right
            }
        }

        Rectangle {
            width: 38
            height: 38
            radius: 19
            Layout.alignment: Qt.AlignVCenter
            color: themeToggleArea.containsMouse ? Theme.cardAlt : "transparent"
            border.width: 1
            border.color: Theme.border
            Behavior on color { ColorAnimation { duration: Theme.animFast } }

            rotation: Theme.dark ? 0 : 180
            Behavior on rotation { NumberAnimation { duration: Theme.animMedium; easing.type: Easing.OutBack } }

            Text {
                anchors.centerIn: parent
                text: Theme.dark ? "☾" : "☀"
                font.pixelSize: 16
                color: Theme.textPrimary
                rotation: parent.rotation // keep glyph upright while parent spins
            }

            MouseArea {
                id: themeToggleArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: Theme.dark = !Theme.dark
            }
        }
    }
}
