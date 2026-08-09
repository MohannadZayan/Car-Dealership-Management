import QtQuick
import QtQuick.Layouts
import Car_Dealership_Management

// * Full-screen login: a showroom photo fills the whole page (one layer below
// * everything else), with a dark overlay — heavier on the form side — so text
// * stays readable regardless of the photo's own brightness. Falls back to the
// * plain gradient/solid background if Assets/login-background.jpg isn't present.
// * On success emits loggedIn().
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
            errorText = qsTr("Enter your email and password.")
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
            errorText = qsTr("Invalid email or password.")
            shake.start()
        }
    }

    // ============================ Background photo ============================
    Image {
        id: backgroundImage
        anchors.fill: parent
        source: "Assets/login-background.jpg"
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
        visible: status === Image.Ready
        cache: true
    }

    // Dark overlay: heavier toward the right (form side) than the left (branding
    // side), so the photo stays visible as texture but every bit of text —
    // regardless of how bright the photo is underneath it — stays readable.
    Rectangle {
        anchors.fill: parent
        visible: backgroundImage.visible
        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop { position: 0.0; color: Qt.rgba(0.02, 0.02, 0.05, 0.55) }
            GradientStop { position: 0.55; color: Qt.rgba(0.02, 0.02, 0.05, 0.78) }
            GradientStop { position: 1.0; color: Qt.rgba(0.02, 0.02, 0.05, 0.92) }
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // ============================= Branding panel =============================
        Rectangle {
            id: brandingPanel
            Layout.preferredWidth: 560
            Layout.fillHeight: true
            visible: root.width > 980
            clip: true
            color: "transparent"

            // Solid gradient fallback — only drawn when there's no photo behind it.
            Rectangle {
                anchors.fill: parent
                visible: !backgroundImage.visible
                gradient: Gradient {
                    orientation: Gradient.Vertical
                    GradientStop { position: 0.0; color: Qt.darker(Theme.accent, 1.35) }
                    GradientStop { position: 1.0; color: Qt.darker(Theme.accent, 2.1) }
                }
            }

            // Slowly drifting decorative circles — purely cosmetic, toned down
            // further when the photo is doing most of the visual work.
            Repeater {
                model: [
                    { size: 300, x: -90, y: 60, opacity: 0.09 },
                    { size: 190, x: 360, y: 460, opacity: 0.12 },
                    { size: 400, x: 140, y: 620, opacity: 0.07 }
                ]

                delegate: Rectangle {
                    width: modelData.size
                    height: modelData.size
                    radius: width / 2
                    color: "#FFFFFF"
                    opacity: backgroundImage.visible ? modelData.opacity * 0.6 : modelData.opacity
                    x: modelData.x
                    y: modelData.y

                    SequentialAnimation on y {
                        loops: Animation.Infinite
                        NumberAnimation { to: modelData.y - 24; duration: 3200 + index * 700; easing.type: Easing.InOutSine }
                        NumberAnimation { to: modelData.y + 24; duration: 3200 + index * 700; easing.type: Easing.InOutSine }
                    }
                }
            }

            ColumnLayout {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: Theme.spacingXLarge * 1.3
                spacing: Theme.spacingLarge

                Text { text: "🚗"; font.pixelSize: 58 }

                Text {
                    text: qsTr("Dealership\nManagement")
                    color: "#FFFFFF"
                    font.pixelSize: 42
                    font.weight: Font.Bold
                    lineHeight: 1.15
                }

                Text {
                    text: qsTr("Everything your dealership runs on — inventory, customers, sales, and invoices — in one place.")
                    color: Qt.rgba(1, 1, 1, 0.85)
                    font.pixelSize: 17
                    wrapMode: Text.WordWrap
                    Layout.preferredWidth: 400
                    Layout.topMargin: 4
                }

                ColumnLayout {
                    spacing: Theme.spacingMedium
                    Layout.topMargin: Theme.spacingMedium

                    Repeater {
                        model: [
                            { icon: "🚘", label: qsTr("Track every vehicle in stock") },
                            { icon: "🤝", label: qsTr("Record sales in seconds") },
                            { icon: "🧾", label: qsTr("Generate and print invoices") }
                        ]

                        delegate: RowLayout {
                            spacing: 12
                            Text { text: modelData.icon; font.pixelSize: 20 }
                            Text { text: modelData.label; color: Qt.rgba(1, 1, 1, 0.92); font.pixelSize: 15 }
                        }
                    }
                }
            }

            Text {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.margins: Theme.spacingXLarge
                text: qsTr("© %1 Dealership Management").arg(new Date().getFullYear())
                color: Qt.rgba(1, 1, 1, 0.6)
                font.pixelSize: Theme.fontSmall
            }
        }

        // =============================== Form panel ================================
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Column {
                id: formColumn
                width: Math.min(440, parent.width - Theme.spacingXLarge * 2)
                anchors.centerIn: parent
                spacing: Theme.spacingLarge

                opacity: 0
                scale: 0.97
                Component.onCompleted: { opacity = 1; scale = 1 }
                Behavior on opacity { NumberAnimation { duration: Theme.animSlow; easing.type: Easing.OutCubic } }
                Behavior on scale { NumberAnimation { duration: Theme.animSlow; easing.type: Easing.OutBack } }

                Text {
                    text: "🚗"
                    font.pixelSize: 36
                    visible: !brandingPanel.visible
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    text: qsTr("Welcome back")
                    color: backgroundImage.visible ? "#FFFFFF" : Theme.textPrimary
                    font.pixelSize: 34
                    font.weight: Font.Bold
                }

                Text {
                    text: qsTr("Sign in to your dealership account")
                    color: backgroundImage.visible ? Qt.rgba(1, 1, 1, 0.75) : Theme.textMuted
                    font.pixelSize: 17
                    bottomPadding: Theme.spacingMedium
                }

                Item {
                    id: shakeAnchor
                    width: parent.width
                    height: shakeColumn.implicitHeight

                    Column {
                        id: shakeColumn
                        width: parent.width
                        spacing: Theme.spacingLarge
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
                            label: qsTr("Email")
                            icon: "✉"
                            placeholder: qsTr("you@dealership.com")
                            error: root.errorText.length > 0
                            fieldHeight: 56
                            labelSize: 14
                            inputSize: 16
                            iconSize: 16
                            inputField.onAccepted: passwordField.inputField.forceActiveFocus()
                        }

                        AppTextField {
                            id: passwordField
                            width: parent.width
                            label: qsTr("Password")
                            icon: "🔒"
                            placeholder: "••••••••"
                            isPassword: true
                            error: root.errorText.length > 0
                            fieldHeight: 56
                            labelSize: 14
                            inputSize: 16
                            iconSize: 16
                            inputField.onAccepted: root.attemptLogin()
                        }

                        Text {
                            text: root.errorText
                            color: Theme.danger
                            font.pixelSize: Theme.fontBody
                            visible: root.errorText.length > 0
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }

                        PrimaryButton {
                            width: parent.width
                            height: 56
                            text: qsTr("Log In")
                            busy: root.busy
                            fontSize: 17
                            onClicked: root.attemptLogin()
                        }
                    }
                }

                Text {
                    text: qsTr("Contact your manager if you don't have an account yet.")
                    color: backgroundImage.visible ? Qt.rgba(1, 1, 1, 0.65) : Theme.textMuted
                    font.pixelSize: 13
                    topPadding: Theme.spacingSmall
                }
            }
        }
    }
}
