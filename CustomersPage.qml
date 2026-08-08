import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic as Controls
import Car_Dealership_Management

// * Customers visible to the current employee — everyone for a Manager, only
// * their own (by sale ownership) for a SalesPerson/CSR. Enforced server-side;
// * this page just displays whatever AppController.customers() returns.
Item {
    id: root

    property var customersData: []

    function refresh() {
        customersData = AppController.customers()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingXLarge
        spacing: Theme.spacingLarge

        RowLayout {
            Layout.fillWidth: true

            Text {
                text: root.customersData.length + " customer(s) visible to you"
                color: Theme.textMuted
                font.pixelSize: Theme.fontBody
                Layout.fillWidth: true
            }

            PrimaryButton {
                text: "+ Add Customer"
                onClicked: {
                    addDialog.errorText = ""
                    addDialog.open()
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: Theme.radiusLarge
            color: Theme.card
            border.width: 1
            border.color: Theme.border
            clip: true

            Text {
                anchors.centerIn: parent
                text: "No customers yet."
                color: Theme.textMuted
                font.pixelSize: Theme.fontBody
                visible: root.customersData.length === 0
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 0
                visible: root.customersData.length > 0

                RowLayout {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    Layout.margins: Theme.spacingMedium

                    Text { text: "Name"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 200 }
                    Text { text: "Phone"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 150 }
                    Text { text: "Email"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 220 }
                    Text { text: "Address"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.fillWidth: true }
                    Item { Layout.preferredWidth: 80 }
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: Theme.border }

                Controls.ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    ListView {
                        model: root.customersData
                        spacing: 0

                        delegate: Rectangle {
                            width: ListView.view.width
                            height: 56
                            color: rowHover.containsMouse ? Theme.cardAlt : "transparent"
                            Behavior on color { ColorAnimation { duration: Theme.animFast } }

                            MouseArea {
                                id: rowHover
                                anchors.fill: parent
                                hoverEnabled: true
                                acceptedButtons: Qt.NoButton
                            }

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: Theme.spacingMedium

                                Text { text: modelData.firstName + " " + modelData.lastName; color: Theme.textPrimary; font.pixelSize: Theme.fontBody; font.weight: Font.DemiBold; Layout.preferredWidth: 200 }
                                Text { text: modelData.phone; color: Theme.textPrimary; font.pixelSize: Theme.fontBody; Layout.preferredWidth: 150 }
                                Text { text: modelData.email; color: Theme.textMuted; font.pixelSize: Theme.fontBody; Layout.preferredWidth: 220; elide: Text.ElideRight }
                                Text { text: modelData.address; color: Theme.textMuted; font.pixelSize: Theme.fontBody; Layout.fillWidth: true; elide: Text.ElideRight }

                                GhostButton {
                                    text: "Delete"
                                    tint: Theme.danger
                                    onClicked: {
                                        deleteConfirm.targetId = modelData.id
                                        deleteConfirm.targetLabel = modelData.firstName + " " + modelData.lastName
                                        deleteConfirm.open()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    AppDialog {
        id: addDialog
        title: "Add Customer"
        confirmText: "Add Customer"

        confirmEnabled: firstNameField.text.trim().length > 0
                         && lastNameField.text.trim().length > 0
                         && phoneField.text.trim().length > 0

        onConfirmed: {
            busy = true
            var ok = AppController.addCustomer({
                firstName: firstNameField.text,
                lastName: lastNameField.text,
                phone: phoneField.text,
                email: emailField.text,
                address: addressField.text
            })
            busy = false

            if (ok) {
                close()
                firstNameField.text = ""; lastNameField.text = ""
                phoneField.text = ""; emailField.text = ""; addressField.text = ""
                root.refresh()
            } else {
                errorText = AppController.lastCustomerError() || "Could not add this customer."
            }
        }

        RowLayout {
            width: parent.width
            spacing: Theme.spacingMedium
            AppTextField { id: firstNameField; Layout.fillWidth: true; label: "First Name"; placeholder: "Jane" }
            AppTextField { id: lastNameField; Layout.fillWidth: true; label: "Last Name"; placeholder: "Doe" }
        }
        AppTextField { id: phoneField; width: parent.width; label: "Phone"; placeholder: "555-0100" }
        AppTextField { id: emailField; width: parent.width; label: "Email"; placeholder: "jane@example.com" }
        AppTextField { id: addressField; width: parent.width; label: "Address"; placeholder: "123 Main St" }
    }

    AppDialog {
        id: deleteConfirm
        title: "Remove Customer"
        confirmText: "Delete"
        property int targetId: -1
        property string targetLabel: ""

        onConfirmed: {
            busy = true
            var ok = AppController.removeCustomer(targetId)
            busy = false
            if (ok) {
                close()
                root.refresh()
            } else {
                errorText = AppController.lastCustomerError() || "Could not delete this customer."
            }
        }

        Text {
            width: parent.width
            wrapMode: Text.WordWrap
            color: Theme.textPrimary
            font.pixelSize: Theme.fontBody
            text: "Remove " + deleteConfirm.targetLabel + "? This cannot be undone."
        }
    }

    Component.onCompleted: refresh()
}
