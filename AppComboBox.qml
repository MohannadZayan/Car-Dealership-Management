import QtQuick
import QtQuick.Controls.Basic as Controls
import Car_Dealership_Management

// * Styled dropdown. `options` is an array of {text, value}; `currentValue` tracks
// * the selected entry's value (not its display text) so enum/id pickers stay simple.
Column {
    id: root

    property string label: ""
    property var options: []
    property var currentValue: undefined

    spacing: Theme.spacingTiny

    function indexForValue(value) {
        for (var i = 0; i < options.length; i++) {
            if (options[i].value === value) return i
        }
        return -1
    }

    Text {
        text: root.label
        color: Theme.textMuted
        font.pixelSize: Theme.fontSmall
        visible: root.label.length > 0
    }

    Controls.ComboBox {
        id: box
        width: parent.width
        height: 44
        textRole: "text"
        valueRole: "value"
        model: root.options
        currentIndex: root.indexForValue(root.currentValue)
        onActivated: root.currentValue = box.currentValue

        background: Rectangle {
            radius: Theme.radiusSmall
            color: Theme.surface
            border.width: 1
            border.color: box.activeFocus || box.popup.visible ? Theme.accent : Theme.border
            Behavior on border.color { ColorAnimation { duration: Theme.animFast } }
        }

        contentItem: Text {
            leftPadding: 14
            rightPadding: 30
            verticalAlignment: Text.AlignVCenter
            text: box.displayText
            color: Theme.textPrimary
            font.pixelSize: Theme.fontBody
            elide: Text.ElideRight
        }

        indicator: Text {
            x: box.width - width - 14
            y: (box.height - height) / 2
            text: "▾"
            color: Theme.textMuted
        }

        popup: Controls.Popup {
            y: box.height + 4
            width: box.width
            implicitHeight: Math.min(contentItem.implicitHeight, 220)
            padding: 4

            background: Rectangle {
                color: Theme.card
                border.width: 1
                border.color: Theme.border
                radius: Theme.radiusSmall
            }

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: box.popup.visible ? box.delegateModel : null
                currentIndex: box.highlightedIndex
                Controls.ScrollIndicator.vertical: Controls.ScrollIndicator {}
            }
        }

        delegate: Controls.ItemDelegate {
            width: box.width
            height: 38

            contentItem: Text {
                text: modelData.text
                color: Theme.textPrimary
                font.pixelSize: Theme.fontBody
                verticalAlignment: Text.AlignVCenter
                leftPadding: 12
            }

            background: Rectangle {
                color: hovered ? Theme.cardAlt : "transparent"
                radius: Theme.radiusSmall
            }
        }
    }
}
