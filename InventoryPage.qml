import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic as Controls
import Car_Dealership_Management

// * Vehicle inventory: shared across every employee, no ownership restriction.
Item {
    id: root

    property var carsData: []

    function refresh() {
        carsData = AppController.cars()
    }

    readonly property var bodyTypeOptions: [
        { text: "Sedan", value: 0 }, { text: "SUV", value: 1 }, { text: "Coupe", value: 2 },
        { text: "Hatchback", value: 3 }, { text: "Convertible", value: 4 }, { text: "Pickup", value: 5 },
        { text: "Wagon", value: 6 }, { text: "Van", value: 7 }
    ]
    readonly property var transmissionOptions: [
        { text: "Manual", value: 0 }, { text: "Automatic", value: 1 }
    ]

    function statusTone(status) {
        return status === 0 ? "success" : status === 1 ? "warning" : status === 2 ? "neutral" : "accent"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingXLarge
        spacing: Theme.spacingLarge

        RowLayout {
            Layout.fillWidth: true

            Text {
                text: root.carsData.length + " vehicle(s) in stock"
                color: Theme.textMuted
                font.pixelSize: Theme.fontBody
                Layout.fillWidth: true
            }

            PrimaryButton {
                text: "+ Add Car"
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

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                RowLayout {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    Layout.margins: Theme.spacingMedium

                    Text { text: "Vehicle"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 220 }
                    Text { text: "Price"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 110 }
                    Text { text: "Mileage"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 100 }
                    Text { text: "Body"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 110 }
                    Text { text: "Status"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 110 }
                    Item { Layout.fillWidth: true }
                    Text { text: ""; Layout.preferredWidth: 80 }
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: Theme.border }

                Controls.ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    ListView {
                        model: root.carsData
                        spacing: 0

                        delegate: Rectangle {
                            width: ListView.view.width
                            height: 58
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

                                Column {
                                    Layout.preferredWidth: 220
                                    Text { text: modelData.make + " " + modelData.model; color: Theme.textPrimary; font.pixelSize: Theme.fontBody; font.weight: Font.DemiBold }
                                    Text { text: modelData.year + " · " + modelData.vin; color: Theme.textMuted; font.pixelSize: Theme.fontTiny }
                                }
                                Text { text: "$" + modelData.price.toLocaleString(); color: Theme.textPrimary; font.pixelSize: Theme.fontBody; Layout.preferredWidth: 110 }
                                Text { text: modelData.mileage.toLocaleString() + " mi"; color: Theme.textPrimary; font.pixelSize: Theme.fontBody; Layout.preferredWidth: 100 }
                                Text { text: modelData.bodyTypeName; color: Theme.textPrimary; font.pixelSize: Theme.fontBody; Layout.preferredWidth: 110 }

                                Item {
                                    Layout.preferredWidth: 110
                                    Layout.fillHeight: true
                                    Badge {
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: modelData.statusName
                                        tone: root.statusTone(modelData.status)
                                    }
                                }

                                Item { Layout.fillWidth: true }

                                GhostButton {
                                    text: "Delete"
                                    tint: Theme.danger
                                    onClicked: {
                                        deleteConfirm.targetId = modelData.id
                                        deleteConfirm.targetLabel = modelData.make + " " + modelData.model
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

    // --- Add Car dialog ---
    AppDialog {
        id: addDialog
        title: "Add Car"
        confirmText: "Add Vehicle"

        confirmEnabled: makeField.text.trim().length > 0
                         && modelField.text.trim().length > 0
                         && vinField.text.trim().length > 0

        onConfirmed: {
            busy = true
            var ok = AppController.addCar({
                make: makeField.text,
                model: modelField.text,
                year: parseInt(yearField.text) || 0,
                price: parseFloat(priceField.text) || 0,
                mileage: parseInt(mileageField.text) || 0,
                color: colorField.text,
                engineName: engineField.text,
                horsepower: parseInt(horsepowerField.text) || 0,
                transmission: transmissionCombo.currentValue !== undefined ? transmissionCombo.currentValue : 1,
                bodyType: bodyTypeCombo.currentValue !== undefined ? bodyTypeCombo.currentValue : 0,
                isNew: newSwitch.checked,
                vin: vinField.text
            })
            busy = false

            if (ok) {
                close()
                makeField.text = ""; modelField.text = ""; yearField.text = ""
                priceField.text = ""; mileageField.text = ""; colorField.text = ""
                engineField.text = ""; horsepowerField.text = ""; vinField.text = ""
                root.refresh()
            } else {
                errorText = AppController.lastCarError() || "Could not add this vehicle. Check the VIN is unique."
            }
        }

        RowLayout {
            width: parent.width
            spacing: Theme.spacingMedium
            AppTextField { id: makeField; Layout.fillWidth: true; label: "Make"; placeholder: "Toyota" }
            AppTextField { id: modelField; Layout.fillWidth: true; label: "Model"; placeholder: "Camry" }
        }
        RowLayout {
            width: parent.width
            spacing: Theme.spacingMedium
            AppTextField { id: yearField; Layout.fillWidth: true; label: "Year"; placeholder: "2024" }
            AppTextField { id: priceField; Layout.fillWidth: true; label: "Price ($)"; placeholder: "25000" }
        }
        RowLayout {
            width: parent.width
            spacing: Theme.spacingMedium
            AppTextField { id: mileageField; Layout.fillWidth: true; label: "Mileage"; placeholder: "0" }
            AppTextField { id: colorField; Layout.fillWidth: true; label: "Color"; placeholder: "Black" }
        }
        RowLayout {
            width: parent.width
            spacing: Theme.spacingMedium
            AppTextField { id: engineField; Layout.fillWidth: true; label: "Engine"; placeholder: "2.5L I4" }
            AppTextField { id: horsepowerField; Layout.fillWidth: true; label: "Horsepower"; placeholder: "200" }
        }
        AppTextField { id: vinField; width: parent.width; label: "VIN"; placeholder: "Unique vehicle identifier" }
        RowLayout {
            width: parent.width
            spacing: Theme.spacingMedium
            AppComboBox { id: bodyTypeCombo; Layout.fillWidth: true; label: "Body Type"; options: root.bodyTypeOptions; currentValue: 0 }
            AppComboBox { id: transmissionCombo; Layout.fillWidth: true; label: "Transmission"; options: root.transmissionOptions; currentValue: 1 }
        }
        RowLayout {
            width: parent.width
            spacing: Theme.spacingSmall
            Controls.CheckBox {
                id: newSwitch
                checked: true
                text: "This is a brand-new vehicle"
                contentItem: Text {
                    text: newSwitch.text
                    color: Theme.textPrimary
                    font.pixelSize: Theme.fontBody
                    leftPadding: newSwitch.indicator.width + 8
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    // --- Delete confirmation ---
    AppDialog {
        id: deleteConfirm
        title: "Remove Vehicle"
        confirmText: "Delete"
        property int targetId: -1
        property string targetLabel: ""

        onConfirmed: {
            busy = true
            var ok = AppController.removeCar(targetId)
            busy = false
            if (ok) {
                close()
                root.refresh()
            } else {
                errorText = AppController.lastCarError() || "Could not delete this vehicle."
            }
        }

        Text {
            width: parent.width
            wrapMode: Text.WordWrap
            color: Theme.textPrimary
            font.pixelSize: Theme.fontBody
            text: "Remove " + deleteConfirm.targetLabel + " from inventory? This cannot be undone."
        }
    }

    Component.onCompleted: refresh()
}
