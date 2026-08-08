import QtQuick
import QtQuick.Layouts
import Car_Dealership_Management

// * Account info, appearance toggle, and a self-service password change.
Item {
    id: root

    function refresh() {
        successText = ""
        errorText = ""
    }

    property string errorText: ""
    property string successText: ""
    property bool busy: false

    function submitPasswordChange() {
        if (busy) return

        errorText = ""
        successText = ""

        if (newPasswordField.text.length < 8) {
            errorText = "New password must be at least 8 characters."
            return
        }
        if (newPasswordField.text !== confirmPasswordField.text) {
            errorText = "New password and confirmation don't match."
            return
        }

        busy = true
        var ok = AppController.changeMyPassword(oldPasswordField.text, newPasswordField.text)
        busy = false

        if (ok) {
            oldPasswordField.text = ""
            newPasswordField.text = ""
            confirmPasswordField.text = ""
            successText = "Password updated."
        } else {
            errorText = "Current password is incorrect, or the new one is too short."
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingXLarge
        spacing: Theme.spacingLarge
        width: 480

        // --- Account card ---
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: accountColumn.implicitHeight + Theme.spacingLarge * 2
            radius: Theme.radiusLarge
            color: Theme.card
            border.width: 1
            border.color: Theme.border

            Column {
                id: accountColumn
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: Theme.spacingLarge
                spacing: Theme.spacingSmall

                Text { text: "Account"; color: Theme.textPrimary; font.pixelSize: Theme.fontMedium; font.weight: Font.Bold }

                Row {
                    spacing: Theme.spacingSmall
                    topPadding: Theme.spacingSmall
                    Text { text: "Name"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; width: 100 }
                    Text { text: AppController.currentEmployeeName; color: Theme.textPrimary; font.pixelSize: Theme.fontBody }
                }
                Row {
                    spacing: Theme.spacingSmall
                    Text { text: "Role"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; width: 100 }
                    Badge { text: AppController.currentEmployeeRole; tone: "accent" }
                }
            }
        }

        // --- Appearance card ---
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 84
            radius: Theme.radiusLarge
            color: Theme.card
            border.width: 1
            border.color: Theme.border

            RowLayout {
                anchors.fill: parent
                anchors.margins: Theme.spacingLarge

                Column {
                    Layout.fillWidth: true
                    spacing: 2
                    Text { text: "Appearance"; color: Theme.textPrimary; font.pixelSize: Theme.fontBody; font.weight: Font.DemiBold }
                    Text { text: Theme.dark ? "Dark mode" : "Light mode"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall }
                }

                Rectangle {
                    id: toggleTrack
                    width: 52
                    height: 28
                    radius: 14
                    color: Theme.dark ? Theme.accent : Theme.border
                    Behavior on color { ColorAnimation { duration: Theme.animMedium } }

                    Rectangle {
                        width: 22
                        height: 22
                        radius: 11
                        color: "#FFFFFF"
                        anchors.verticalCenter: parent.verticalCenter
                        x: Theme.dark ? parent.width - width - 3 : 3
                        Behavior on x { NumberAnimation { duration: Theme.animMedium; easing.type: Easing.OutBack } }
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: Theme.dark = !Theme.dark
                    }
                }
            }
        }

        // --- Change password card ---
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: passwordColumn.implicitHeight + Theme.spacingLarge * 2
            radius: Theme.radiusLarge
            color: Theme.card
            border.width: 1
            border.color: Theme.border

            Column {
                id: passwordColumn
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: Theme.spacingLarge
                spacing: Theme.spacingMedium

                Text { text: "Change Password"; color: Theme.textPrimary; font.pixelSize: Theme.fontMedium; font.weight: Font.Bold }

                AppTextField { id: oldPasswordField; width: parent.width; label: "Current Password"; isPassword: true }
                AppTextField { id: newPasswordField; width: parent.width; label: "New Password"; isPassword: true; placeholder: "At least 8 characters" }
                AppTextField { id: confirmPasswordField; width: parent.width; label: "Confirm New Password"; isPassword: true }

                Text {
                    text: root.errorText
                    color: Theme.danger
                    font.pixelSize: Theme.fontSmall
                    visible: root.errorText.length > 0
                    width: parent.width
                    wrapMode: Text.WordWrap
                }
                Text {
                    text: root.successText
                    color: Theme.success
                    font.pixelSize: Theme.fontSmall
                    visible: root.successText.length > 0
                }

                PrimaryButton {
                    text: "Update Password"
                    busy: root.busy
                    onClicked: root.submitPasswordChange()
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
