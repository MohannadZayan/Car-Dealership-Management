import QtQuick
import Car_Dealership_Management

// * Full-screen login form. On success emits `loggedIn()` for Main.qml to swap to Shell.
Rectangle {
    id: root

    signal loggedIn()

    color: Theme.background

    Behavior on color { ColorAnimation { duration: Theme.animMedium } }

    // Faint decorative glow, purely cosmetic.
    Rectangle {
        width: 520
        height: 520
        radius: width / 2
        color: Theme.accent
        opacity: 0.10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -80
    }

    Rectangle {
        id: card
        width: 400
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        height: column.implicitHeight + Theme.spacingXLarge * 2
        radius: Theme.radiusLarge
        color: Theme.card
        border.width: 1
        border.color: Theme.border

        opacity: 0
        scale: 0.96
        Component.onCompleted: { opacity = 1; scale = 1 }
        Behavior on opacity { NumberAnimation { duration: Theme.animSlow; easing.type: Easing.OutCubic } }
        Behavior on scale { NumberAnimation { duration: Theme.animSlow; easing.type: Easing.OutBack } }

        Column {
            id: column
            width: parent.width - Theme.spacingXLarge * 2
            anchors.centerIn: parent
            spacing: Theme.spacingMedium

            Text {
                text: "🚗"
                font.pixelSize: 34
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "Welcome back"
                color: Theme.textPrimary
                font.pixelSize: Theme.fontLarge
                font.weight: Font.Bold
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "Sign in to manage the dealership"
                color: Theme.textMuted
                font.pixelSize: Theme.fontBody
                anchors.horizontalCenter: parent.horizontalCenter
                bottomPadding: Theme.spacingSmall
            }

            AppTextField {
                id: emailField
                width: parent.width
                label: "Email"
                placeholder: "you@dealership.com"
                error: root.errorText.length > 0
                inputField.onAccepted: passwordField.inputField.forceActiveFocus()
            }

            AppTextField {
                id: passwordField
                width: parent.width
                label: "Password"
                placeholder: "••••••••"
                isPassword: true
                error: root.errorText.length > 0
                inputField.onAccepted: root.attemptLogin()
            }

            Text {
                text: root.errorText
                color: Theme.danger
                font.pixelSize: Theme.fontSmall
                visible: root.errorText.length > 0
                width: parent.width
                wrapMode: Text.WordWrap
            }

            PrimaryButton {
                width: parent.width
                text: "Log In"
                busy: root.busy
                onClicked: root.attemptLogin()
            }
        }
    }

    property string errorText: ""
    property bool busy: false

    function attemptLogin() {
        if (busy) return

        if (emailField.text.trim().length === 0 || passwordField.text.length === 0) {
            errorText = "Enter your email and password."
            return
        }

        busy = true
        errorText = ""

        var success = AppController.login(emailField.text.trim(), passwordField.text)

        busy = false

        if (success) {
            passwordField.text = ""
            root.loggedIn()
        } else {
            errorText = "Invalid email or password."
            shake.start()
        }
    }

    // Nudges anchors.horizontalCenterOffset rather than x directly — card is
    // anchor-centered, and animating x on an anchored item would break that anchor.
    SequentialAnimation {
        id: shake
        loops: 1
        NumberAnimation { target: card; property: "anchors.horizontalCenterOffset"; to: -10; duration: 45 }
        NumberAnimation { target: card; property: "anchors.horizontalCenterOffset"; to: 10; duration: 90 }
        NumberAnimation { target: card; property: "anchors.horizontalCenterOffset"; to: 0; duration: 45 }
    }
}
