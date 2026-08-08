import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic as Controls
import Car_Dealership_Management

// * Invoices visible to the current employee (inherits visibility from their sale).
// * Print renders a PDF and opens it in the OS's default viewer — one click to print.
Item {
    id: root

    property var invoicesData: []
    property var salesData: []
    property var customersData: []

    property string printStatus: ""

    function refresh() {
        invoicesData = AppController.invoices()
        salesData = AppController.sales()
        customersData = AppController.customers()
    }

    function saleForId(id) {
        for (var i = 0; i < salesData.length; i++) {
            if (salesData[i].id === id) return salesData[i]
        }
        return null
    }
    function customerLabelForSale(saleId) {
        var sale = saleForId(saleId)
        if (sale === null) return "—"
        for (var i = 0; i < customersData.length; i++) {
            if (customersData[i].id === sale.customerId) return customersData[i].firstName + " " + customersData[i].lastName
        }
        return "Customer #" + sale.customerId
    }
    function saleOptions() {
        var opts = []
        for (var i = 0; i < salesData.length; i++) {
            opts.push({ text: "Sale #" + salesData[i].id + " — $" + salesData[i].salePrice.toLocaleString(), value: salesData[i].id })
        }
        return opts
    }

    readonly property var paymentOptions: [
        { text: "Cash", value: 0 }, { text: "CreditCard", value: 1 },
        { text: "BankTransfer", value: 2 }, { text: "Financing", value: 3 }
    ]

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingXLarge
        spacing: Theme.spacingLarge

        RowLayout {
            Layout.fillWidth: true

            Text {
                text: root.invoicesData.length + " invoice(s) visible to you"
                color: Theme.textMuted
                font.pixelSize: Theme.fontBody
                Layout.fillWidth: true
            }

            Text {
                text: root.printStatus
                color: Theme.textMuted
                font.pixelSize: Theme.fontSmall
                visible: root.printStatus.length > 0
            }

            PrimaryButton {
                text: "+ New Invoice"
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
                text: "No invoices yet."
                color: Theme.textMuted
                font.pixelSize: Theme.fontBody
                visible: root.invoicesData.length === 0
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 0
                visible: root.invoicesData.length > 0

                RowLayout {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    Layout.margins: Theme.spacingMedium

                    Text { text: "Invoice #"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 140 }
                    Text { text: "Customer"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 180 }
                    Text { text: "Payment"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 130 }
                    Text { text: "Total"; color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.fillWidth: true }
                    Item { Layout.preferredWidth: 90 }
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: Theme.border }

                Controls.ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    ListView {
                        model: root.invoicesData
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

                                Text { text: modelData.invoiceNumber; color: Theme.textPrimary; font.pixelSize: Theme.fontBody; font.weight: Font.DemiBold; Layout.preferredWidth: 140 }
                                Text { text: root.customerLabelForSale(modelData.saleId); color: Theme.textPrimary; font.pixelSize: Theme.fontBody; Layout.preferredWidth: 180 }
                                Text { text: modelData.paymentMethodName; color: Theme.textMuted; font.pixelSize: Theme.fontBody; Layout.preferredWidth: 130 }
                                Text { text: "$" + modelData.total.toLocaleString(); color: Theme.success; font.pixelSize: Theme.fontBody; font.weight: Font.DemiBold; Layout.fillWidth: true }

                                GhostButton {
                                    text: "🖨 Print"
                                    onClicked: {
                                        root.printStatus = "Rendering PDF…"
                                        var ok = AppController.printInvoice(modelData.id)
                                        root.printStatus = ok ? "Opened invoice PDF." : "PDF saved — open it from your file manager."
                                        printStatusTimer.restart()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Timer {
        id: printStatusTimer
        interval: 4000
        onTriggered: root.printStatus = ""
    }

    AppDialog {
        id: addDialog
        title: "New Invoice"
        confirmText: "Create Invoice"

        confirmEnabled: saleCombo.currentValue !== undefined
                         && invoiceNumberField.text.trim().length > 0
                         && totalField.text.trim().length > 0

        onConfirmed: {
            busy = true
            var subtotal = parseFloat(subtotalField.text) || 0
            var tax = parseFloat(taxField.text) || 0
            var total = totalField.text.trim().length > 0 ? (parseFloat(totalField.text) || 0) : (subtotal + tax)

            var ok = AppController.addInvoice({
                saleId: saleCombo.currentValue,
                invoiceNumber: invoiceNumberField.text,
                issueDate: new Date(),
                subtotal: subtotal,
                tax: tax,
                total: total,
                paymentMethod: paymentCombo.currentValue !== undefined ? paymentCombo.currentValue : 0
            })
            busy = false

            if (ok) {
                close()
                invoiceNumberField.text = ""; subtotalField.text = ""; taxField.text = ""; totalField.text = ""
                root.refresh()
            } else {
                errorText = AppController.lastInvoiceError() || "Could not create this invoice."
            }
        }

        AppComboBox { id: saleCombo; width: parent.width; label: "Sale"; options: root.saleOptions() }
        AppTextField { id: invoiceNumberField; width: parent.width; label: "Invoice Number"; placeholder: "INV-0001" }
        RowLayout {
            width: parent.width
            spacing: Theme.spacingMedium
            AppTextField { id: subtotalField; Layout.fillWidth: true; label: "Subtotal ($)"; placeholder: "25000" }
            AppTextField { id: taxField; Layout.fillWidth: true; label: "Tax ($)"; placeholder: "1500" }
        }
        AppTextField { id: totalField; width: parent.width; label: "Total ($)"; placeholder: "26500" }
        AppComboBox { id: paymentCombo; width: parent.width; label: "Payment Method"; options: root.paymentOptions; currentValue: 0 }
    }

    Component.onCompleted: refresh()
}
