import QtQuick
import QtQuick.Controls.Basic as Controls
import Car_Dealership_Management

// * Reusable modal shell for Add/Edit forms: dimmed overlay, title, a scrollable
// * body that any page can drop form fields into, and a Save/Cancel footer.
// * Usage: AppDialog { title: "Add Car"; AppTextField { ... } ... onConfirmed: ... }
Controls.Popup {
    id: root

    default property alias contentData: bodyColumn.data

    property string title: ""
    property string confirmText: qsTr("Save")
    property string cancelText: qsTr("Cancel")
    property bool confirmEnabled: true
    property bool busy: false
    property string errorText: ""
    property bool showFooter: true

    signal confirmed()

    modal: true
    focus: true
    closePolicy: Controls.Popup.CloseOnEscape | Controls.Popup.CloseOnPressOutside
    anchors.centerIn: Controls.Overlay.overlay
    width: 460
    padding: 0

    Controls.Overlay.modal: Rectangle {
        color: Qt.rgba(0, 0, 0, 0.55)
    }

    enter: Transition {
        NumberAnimation { properties: "opacity"; from: 0; to: 1; duration: Theme.animMedium }
        NumberAnimation { properties: "scale"; from: 0.94; to: 1; duration: Theme.animMedium; easing.type: Easing.OutBack }
    }
    exit: Transition {
        NumberAnimation { properties: "opacity"; from: 1; to: 0; duration: Theme.animFast }
    }

    background: Rectangle {
        color: Theme.card
        radius: Theme.radiusLarge
        border.width: 1
        border.color: Theme.border
    }

    contentItem: Column {
        spacing: 0
        width: root.width - root.leftPadding - root.rightPadding

        Item {
            width: parent.width
            height: 60

            Text {
                anchors.left: parent.left
                anchors.leftMargin: Theme.spacingLarge
                anchors.verticalCenter: parent.verticalCenter
                text: root.title
                color: Theme.textPrimary
                font.pixelSize: Theme.fontLarge
                font.weight: Font.Bold
            }
        }

        Rectangle { width: parent.width; height: 1; color: Theme.border }

        Controls.ScrollView {
            id: scrollView
            width: parent.width
            height: Math.min(bodyColumn.implicitHeight + Theme.spacingLarge * 2, 420)
            clip: true
            Controls.ScrollBar.horizontal.policy: Controls.ScrollBar.AlwaysOff

            // bodyColumn's *actual* parent at runtime is ScrollView's internal
            // Flickable, not ScrollView itself — "parent.width" there doesn't
            // propagate a usable value (every field ends up width 0, stacked on
            // top of each other). availableWidth is ScrollView's own geometry
            // (width minus padding/scrollbar), computed independent of that
            // reparenting, so it's the robust way to size scrollable content.
            Column {
                id: bodyColumn
                x: Theme.spacingLarge
                width: scrollView.availableWidth - Theme.spacingLarge * 2
                topPadding: Theme.spacingLarge
                bottomPadding: Theme.spacingLarge
                spacing: Theme.spacingMedium
            }
        }

        Text {
            x: Theme.spacingLarge
            width: parent.width - Theme.spacingLarge * 2
            text: root.errorText
            color: Theme.danger
            font.pixelSize: Theme.fontSmall
            wrapMode: Text.WordWrap
            visible: root.errorText.length > 0
            bottomPadding: Theme.spacingSmall
        }

        Rectangle { width: parent.width; height: 1; color: Theme.border; visible: root.showFooter }

        Item {
            width: parent.width
            height: root.showFooter ? 76 : 0
            visible: root.showFooter

            Row {
                anchors.right: parent.right
                anchors.rightMargin: Theme.spacingLarge
                anchors.verticalCenter: parent.verticalCenter
                spacing: Theme.spacingSmall
                layoutDirection: Qt.RightToLeft

                PrimaryButton {
                    text: root.confirmText
                    enabled: root.confirmEnabled
                    busy: root.busy
                    onClicked: root.confirmed()
                }
                GhostButton {
                    text: root.cancelText
                    onClicked: root.close()
                }
            }
        }
    }
}
