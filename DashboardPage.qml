import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic as Controls
import Car_Dealership_Management

// * Overview of the whole dealership: inventory breakdown, customers/sales in scope
// * for the current employee, and revenue from invoices — all real data from AppController.
Item {
    id: root

    property var carsData: []
    property var customersData: []
    property var employeesData: []
    property var salesData: []
    property var invoicesData: []

    function refresh() {
        carsData = AppController.cars()
        customersData = AppController.customers()
        employeesData = AppController.employees()
        salesData = AppController.sales()
        invoicesData = AppController.invoices()
    }

    function countByStatus(status) {
        var n = 0
        for (var i = 0; i < carsData.length; i++) {
            if (carsData[i].status === status) n++
        }
        return n
    }

    function totalRevenue() {
        var sum = 0
        for (var i = 0; i < invoicesData.length; i++) sum += invoicesData[i].total
        return sum
    }

    function formatMoney(n) {
        return "$" + Math.round(n).toLocaleString()
    }

    Controls.ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth
        clip: true

        ColumnLayout {
            width: root.width - Theme.spacingXLarge * 2
            x: Theme.spacingXLarge
            y: Theme.spacingLarge
            spacing: Theme.spacingLarge

            Text {
                text: "Welcome back, " + AppController.currentEmployeeName.split(" ")[0] + " 👋"
                color: Theme.textPrimary
                font.pixelSize: Theme.fontLarge
                font.weight: Font.Bold
            }

            GridLayout {
                Layout.fillWidth: true
                columns: 4
                columnSpacing: Theme.spacingLarge
                rowSpacing: Theme.spacingLarge

                StatCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: "Total Inventory"
                    value: root.carsData.length.toString()
                    subtitle: "Vehicles on the lot"
                }
                StatCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: "Available"
                    value: root.countByStatus(0).toString()
                    subtitle: "Ready to sell"
                    trend: root.carsData.length > 0 ? Math.round(100 * root.countByStatus(0) / root.carsData.length) + "%" : ""
                    trendPositive: true
                }
                StatCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: "Reserved"
                    value: root.countByStatus(1).toString()
                    subtitle: "Pending sale"
                }
                StatCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: "Sold"
                    value: root.countByStatus(2).toString()
                    subtitle: "Completed sales"
                }
            }

            GridLayout {
                Layout.fillWidth: true
                columns: 3
                columnSpacing: Theme.spacingLarge
                rowSpacing: Theme.spacingLarge

                StatCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: "Customers"
                    value: root.customersData.length.toString()
                    subtitle: AppController.isManager ? "All customers" : "Assigned to you"
                }
                StatCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: "Sales"
                    value: root.salesData.length.toString()
                    subtitle: AppController.isManager ? "All transactions" : "Made by you"
                }
                StatCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: "Revenue"
                    value: root.formatMoney(root.totalRevenue())
                    subtitle: "From " + root.invoicesData.length + " invoice(s)"
                    trendPositive: true
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: recentColumn.implicitHeight + Theme.spacingLarge * 2
                radius: Theme.radiusLarge
                color: Theme.card
                border.width: 1
                border.color: Theme.border

                Column {
                    id: recentColumn
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: Theme.spacingLarge
                    spacing: Theme.spacingMedium

                    Text {
                        text: "Recent Sales"
                        color: Theme.textPrimary
                        font.pixelSize: Theme.fontMedium
                        font.weight: Font.Bold
                    }

                    Text {
                        text: "No sales yet."
                        color: Theme.textMuted
                        font.pixelSize: Theme.fontBody
                        visible: root.salesData.length === 0
                    }

                    Repeater {
                        model: root.salesData.slice(-5).reverse()

                        delegate: Rectangle {
                            width: recentColumn.width
                            height: 46
                            radius: Theme.radiusSmall
                            color: rowHover.containsMouse ? Theme.cardAlt : "transparent"
                            Behavior on color { ColorAnimation { duration: Theme.animFast } }

                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 10
                                anchors.rightMargin: 10

                                Text {
                                    text: "Sale #" + modelData.id
                                    color: Theme.textPrimary
                                    font.pixelSize: Theme.fontBody
                                    Layout.fillWidth: true
                                }
                                Text {
                                    text: Qt.formatDate(modelData.saleDate, "MMM d, yyyy")
                                    color: Theme.textMuted
                                    font.pixelSize: Theme.fontSmall
                                }
                                Text {
                                    text: root.formatMoney(modelData.salePrice)
                                    color: Theme.success
                                    font.pixelSize: Theme.fontBody
                                    font.weight: Font.DemiBold
                                    Layout.preferredWidth: 90
                                    horizontalAlignment: Text.AlignRight
                                }
                            }

                            MouseArea {
                                id: rowHover
                                anchors.fill: parent
                                hoverEnabled: true
                                acceptedButtons: Qt.NoButton
                            }
                        }
                    }
                }
            }
        }
    }
}
