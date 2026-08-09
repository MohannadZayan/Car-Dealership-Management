import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic as Controls
import Car_Dealership_Management

// * Sales visible to the current employee — everyone for a Manager, only sales
// * they were the assigned employee on otherwise. Car/customer/employee names
// * are resolved client-side from the other lists since Sale only stores ids.
Item {
    id: root

    property var salesData: []
    property var carsData: []
    property var customersData: []
    property var employeesData: []

    function refresh() {
        salesData = AppController.sales()
        carsData = AppController.cars()
        customersData = AppController.customers()
        employeesData = AppController.employees()
    }

    function carLabel(id) {
        for (var i = 0; i < carsData.length; i++) {
            if (carsData[i].id === id) return carsData[i].make + " " + carsData[i].model
        }
        return qsTr("Car #%1").arg(id)
    }
    function customerLabel(id) {
        for (var i = 0; i < customersData.length; i++) {
            if (customersData[i].id === id) return customersData[i].firstName + " " + customersData[i].lastName
        }
        return qsTr("Customer #%1").arg(id)
    }
    function employeeLabel(id) {
        for (var i = 0; i < employeesData.length; i++) {
            if (employeesData[i].id === id) return employeesData[i].firstName + " " + employeesData[i].lastName
        }
        return qsTr("Employee #%1").arg(id)
    }

    function carOptions() {
        var opts = []
        for (var i = 0; i < carsData.length; i++) {
            opts.push({ text: carsData[i].make + " " + carsData[i].model + " (" + carsData[i].vin + ")", value: carsData[i].id })
        }
        return opts
    }
    function customerOptions() {
        var opts = []
        for (var i = 0; i < customersData.length; i++) {
            opts.push({ text: customersData[i].firstName + " " + customersData[i].lastName, value: customersData[i].id })
        }
        return opts
    }
    function employeeOptions() {
        var opts = []
        for (var i = 0; i < employeesData.length; i++) {
            opts.push({ text: employeesData[i].firstName + " " + employeesData[i].lastName, value: employeesData[i].id })
        }
        return opts
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingXLarge
        spacing: Theme.spacingLarge

        RowLayout {
            Layout.fillWidth: true

            Text {
                text: qsTr("%n sale(s) visible to you", "", root.salesData.length)
                color: Theme.textMuted
                font.pixelSize: Theme.fontBody
                Layout.fillWidth: true
            }

            PrimaryButton {
                text: qsTr("+ Record Sale")
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
                text: qsTr("No sales recorded yet.")
                color: Theme.textMuted
                font.pixelSize: Theme.fontBody
                visible: root.salesData.length === 0
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 0
                visible: root.salesData.length > 0

                RowLayout {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    Layout.margins: Theme.spacingMedium

                    Text { text: qsTr("Vehicle"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 200 }
                    Text { text: qsTr("Customer"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 180 }
                    Text { text: qsTr("Sold By"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 160 }
                    Text { text: qsTr("Date"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 120 }
                    Text { text: qsTr("Price"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.fillWidth: true }
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: Theme.border }

                Controls.ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    ListView {
                        model: root.salesData
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

                                Text { text: root.carLabel(modelData.carId); color: Theme.textPrimary; font.pixelSize: Theme.fontBody; font.weight: Font.DemiBold; Layout.preferredWidth: 200 }
                                Text { text: root.customerLabel(modelData.customerId); color: Theme.textPrimary; font.pixelSize: Theme.fontBody; Layout.preferredWidth: 180 }
                                Text { text: root.employeeLabel(modelData.employeeId); color: Theme.textMuted; font.pixelSize: Theme.fontBody; Layout.preferredWidth: 160 }
                                Text { text: Qt.formatDate(modelData.saleDate, "MMM d, yyyy"); color: Theme.textMuted; font.pixelSize: Theme.fontBody; Layout.preferredWidth: 120 }
                                Text { text: "$" + modelData.salePrice.toLocaleString(); color: Theme.success; font.pixelSize: Theme.fontBody; font.weight: Font.DemiBold; Layout.fillWidth: true }
                            }
                        }
                    }
                }
            }
        }
    }

    AppDialog {
        id: addDialog
        title: qsTr("Record Sale")
        confirmText: qsTr("Record Sale")

        confirmEnabled: carCombo.currentValue !== undefined
                         && customerCombo.currentValue !== undefined
                         && employeeCombo.currentValue !== undefined
                         && priceField.text.trim().length > 0

        onConfirmed: {
            busy = true
            var ok = AppController.addSale({
                carId: carCombo.currentValue,
                customerId: customerCombo.currentValue,
                employeeId: employeeCombo.currentValue,
                salePrice: parseFloat(priceField.text) || 0,
                saleDate: new Date()
            })
            busy = false

            if (ok) {
                close()
                priceField.text = ""
                root.refresh()
            } else {
                errorText = AppController.lastSaleError() || qsTr("Could not record this sale.")
            }
        }

        Text {
            width: parent.width
            wrapMode: Text.WordWrap
            color: Theme.warning
            font.pixelSize: Theme.fontSmall
            text: qsTr("⚠ No customers are visible to you yet — a customer only appears in your list once a sale links them to you. Ask a Manager to record the first sale for a brand-new customer.")
            visible: root.customersData.length === 0
        }

        AppComboBox { id: carCombo; width: parent.width; label: qsTr("Vehicle"); options: root.carOptions() }
        AppComboBox { id: customerCombo; width: parent.width; label: qsTr("Customer"); options: root.customerOptions() }
        AppComboBox {
            id: employeeCombo
            width: parent.width
            label: qsTr("Sold By")
            options: root.employeeOptions()
            currentValue: AppController.currentEmployeeId
        }
        AppTextField { id: priceField; width: parent.width; label: qsTr("Sale Price ($)"); placeholder: "25000" }
    }

    Component.onCompleted: refresh()
}
