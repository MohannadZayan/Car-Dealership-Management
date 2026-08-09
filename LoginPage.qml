import QtQuick
import QtQuick.Layouts
import Car_Dealership_Management

// * Full-screen login: a gradient branding panel on the left (hides on narrow
// * windows), the actual form on the right. On success emits loggedIn().
Rectangle {
    id: root

    signal loggedIn()

    color: Theme.background
    Behavior on color { ColorAnimation { duration: Theme.animMedium } }

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

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // ============================= Branding panel =============================
        Rectangle {
            id: brandingPanel
            Layout.preferredWidth: 460
            Layout.fillHeight: true
            visible: root.width > 860
            clip: true

            gradient: Gradient {
                orientation: Gradient.Vertical
                GradientStop { position: 0.0; color: Qt.darker(Theme.accent, 1.35) }
                GradientStop { position: 1.0; color: Qt.darker(Theme.accent, 2.1) }
            }

            // Slowly drifting decorative circles — purely cosmetic.
            Repeater {
                model: [
                    { size: 260, x: -80, y: 60, opacity: 0.10 },
                    { size: 160, x: 300, y: 420, opacity: 0.14 },
                    { size: 340, x: 120, y: 560, opacity: 0.08 }
                ]

                delegate: Rectangle {
                    width: modelData.size
                    height: modelData.size
                    radius: width / 2
                    color: "#FFFFFF"
                    opacity: modelData.opacity
                    x: modelData.x
                    y: modelData.y

                    SequentialAnimation on y {
                        loops: Animation.Infinite
                        NumberAnimation { to: modelData.y - 22; duration: 3200 + index * 700; easing.type: Easing.InOutSine }
                        NumberAnimation { to: modelData.y + 22; duration: 3200 + index * 700; easing.type: Easing.InOutSine }
                    }
                }
            }

            ColumnLayout {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: Theme.spacingXLarge
                spacing: Theme.spacingLarge

                Text { text: "🚗"; font.pixelSize: 46 }

                Text {
                    text: "Dealership\nManagement"
                    color: "#FFFFFF"
                    font.pixelSize: 30
                    font.weight: Font.Bold
                    lineHeight: 1.15
                }

                Text {
                    text: "Everything your dealership runs on — inventory, customers, sales, and invoices — in one place."
                    color: Qt.rgba(1, 1, 1, 0.82)
                    font.pixelSize: Theme.fontBody
                    wrapMode: Text.WordWrap
                    Layout.preferredWidth: 340
                    Layout.topMargin: 4
                }

                ColumnLayout {
                    spacing: Theme.spacingSmall
                    Layout.topMargin: Theme.spacingMedium

                    Repeater {
                        model: [
                            { icon: "🚘", label: "Track every vehicle in stock" },
                            { icon: "🤝", label: "Record sales in seconds" },
                            { icon: "🧾", label: "Generate and print invoices" }
                        ]

                        delegate: RowLayout {
                            spacing: 10
                            Text { text: modelData.icon; font.pixelSize: 16 }
                            Text { text: modelData.label; color: Qt.rgba(1, 1, 1, 0.9); font.pixelSize: Theme.fontSmall }
                        }
                    }
                }
            }

            Text {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.margins: Theme.spacingXLarge
                text: "© " + new Date().getFullYear() + " Dealership Management"
                color: Qt.rgba(1, 1, 1, 0.55)
                font.pixelSize: Theme.fontTiny
            }
        }

        // =============================== Form panel ================================
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Column {
                id: formColumn
                width: Math.min(360, parent.width - Theme.spacingXLarge * 2)
                anchors.centerIn: parent
                spacing: Theme.spacingMedium

                opacity: 0
                scale: 0.97
                Component.onCompleted: { opacity = 1; scale = 1 }
                Behavior on opacity { NumberAnimation { duration: Theme.animSlow; easing.type: Easing.OutCubic } }
                Behavior on scale { NumberAnimation { duration: Theme.animSlow; easing.type: Easing.OutBack } }

                Text {
                    text: "🚗"
                    font.pixelSize: 28
                    visible: !brandingPanel.visible
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    text: "Welcome back"
                    color: Theme.textPrimary
                    font.pixelSize: 26
                    font.weight: Font.Bold
                }

                Text {
                    text: "Sign in to your dealership account"
                    color: Theme.textMuted
                    font.pixelSize: Theme.fontBody
                    bottomPadding: Theme.spacingMedium
                }

                Item { id: shakeAnchor; width: parent.width; height: shakeColumn.implicitHeight

                    Column {
                        id: shakeColumn
                        width: parent.width
                        spacing: Theme.spacingMedium
                        x: 0

                        // Nudges x directly — this Item isn't anchor-positioned by its
                        // parent Column (Column lays children out via y only), so x is safe to animate.
                        SequentialAnimation {
                            id: shake
                            loops: 1
                            NumberAnimation { target: shakeColumn; property: "x"; to: -10; duration: 45 }
                            NumberAnimation { target: shakeColumn; property: "x"; to: 10; duration: 90 }
                            NumberAnimation { target: shakeColumn; property: "x"; to: 0; duration: 45 }
                        }

                        AppTextField {
                            id: emailField
                            width: parent.width
                            label: "Email"
                            icon: "✉"
                            placeholder: "you@dealership.com"
                            error: root.errorText.length > 0
                            inputField.onAccepted: passwordField.inputField.forceActiveFocus()
                        }

                        AppTextField {
                            id: passwordField
                            width: parent.width
                            label: "Password"
                            icon: "🔒"
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

                Text {
                    text: "Contact your manager if you don't have an account yet."
                    color: Theme.textMuted
                    font.pixelSize: Theme.fontTiny
                    topPadding: Theme.spacingSmall
                }
            }
        }
    }
}
