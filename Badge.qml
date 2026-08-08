import QtQuick
import Car_Dealership_Management

// * Small rounded status pill (e.g. car status, role). `tone` picks the color family.
Rectangle {
    id: root

    property string text: ""
    property string tone: "neutral" // "success" | "warning" | "danger" | "accent" | "neutral"

    readonly property color toneColor: tone === "success" ? Theme.success
                                        : tone === "warning" ? Theme.warning
                                        : tone === "danger" ? Theme.danger
                                        : tone === "accent" ? Theme.accent
                                        : Theme.textMuted

    implicitWidth: label.implicitWidth + 20
    implicitHeight: 24
    radius: Theme.radiusPill
    color: Qt.rgba(toneColor.r, toneColor.g, toneColor.b, 0.15)
    border.width: 1
    border.color: Qt.rgba(toneColor.r, toneColor.g, toneColor.b, 0.35)

    Text {
        id: label
        anchors.centerIn: parent
        text: root.text
        color: root.toneColor
        font.pixelSize: Theme.fontTiny
        font.weight: Font.DemiBold
    }
}
