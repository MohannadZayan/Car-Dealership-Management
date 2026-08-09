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
        return qsTr("Customer #%1").arg(sale.customerId)
    }
    function saleOptions() {
        var opts = []
        for (var i = 0; i < salesData.length; i++) {
            opts.push({ text: qsTr("Sale #%1 — $%2").arg(salesData[i].id).arg(salesData[i].salePrice.toLocaleString()), value: salesData[i].id })
        }
        return opts
    }

    readonly property var paymentOptions: [
        { text: qsTr("Cash"), value: 0 }, { text: qsTr("CreditCard"), value: 1 },
        { text: qsTr("BankTransfer"), value: 2 }, { text: qsTr("Financing"), value: 3 }
    ]

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingXLarge
        spacing: Theme.spacingLarge

        RowLayout {
            Layout.fillWidth: true

            Text {
                text: qsTr("%n invoice(s) visible to you", "", root.invoicesData.length)
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
                text: qsTr("+ New Invoice")
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
                text: qsTr("No invoices yet.")
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

                    Text { text: qsTr("Invoice #"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 140 }
                    Text { text: qsTr("Customer"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 180 }
                    Text { text: qsTr("Payment"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 130 }
                    Text { text: qsTr("Total"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.fillWidth: true }
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
                                    text: qsTr("🖨 Print")
                                    onClicked: {
                                        root.printStatus = qsTr("Rendering PDF…")
                                        var ok = AppController.printInvoice(modelData.id)
                                        root.printStatus = ok ? qsTr("Opened invoice PDF.") : qsTr("PDF saved — open it from your file manager.")
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
        title: qsTr("New Invoice")
        confirmText: qsTr("Create Invoice")

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
                errorText = AppController.lastInvoiceError() || qsTr("Could not create this invoice.")
            }
        }

        AppComboBox { id: saleCombo; width: parent.width; label: qsTr("Sale"); options: root.saleOptions() }
        AppTextField { id: invoiceNumberField; width: parent.width; label: qsTr("Invoice Number"); placeholder: "INV-0001" }
        RowLayout {
            width: parent.width
            spacing: Theme.spacingMedium
            AppTextField { id: subtotalField; Layout.fillWidth: true; label: qsTr("Subtotal ($)"); placeholder: "25000" }
            AppTextField { id: taxField; Layout.fillWidth: true; label: qsTr("Tax ($)"); placeholder: "1500" }
        }
        AppTextField { id: totalField; width: parent.width; label: qsTr("Total ($)"); placeholder: "26500" }
        AppComboBox { id: paymentCombo; width: parent.width; label: qsTr("Payment Method"); options: root.paymentOptions; currentValue: 0 }
    }

    Component.onCompleted: refresh()
}
