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
                text: qsTr("Welcome back, %1 👋").arg(AppController.currentEmployeeName.split(" ")[0])
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
                    title: qsTr("Total Inventory")
                    value: root.carsData.length.toString()
                    subtitle: qsTr("Vehicles on the lot")
                }
                StatCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: qsTr("Available")
                    value: root.countByStatus(0).toString()
                    subtitle: qsTr("Ready to sell")
                    trend: root.carsData.length > 0 ? Math.round(100 * root.countByStatus(0) / root.carsData.length) + "%" : ""
                    trendPositive: true
                }
                StatCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: qsTr("Reserved")
                    value: root.countByStatus(1).toString()
                    subtitle: qsTr("Pending sale")
                }
                StatCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: qsTr("Sold")
                    value: root.countByStatus(2).toString()
                    subtitle: qsTr("Completed sales")
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
                    title: qsTr("Customers")
                    value: root.customersData.length.toString()
                    subtitle: AppController.isManager ? qsTr("All customers") : qsTr("Assigned to you")
                }
                StatCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: qsTr("Sales")
                    value: root.salesData.length.toString()
                    subtitle: AppController.isManager ? qsTr("All transactions") : qsTr("Made by you")
                }
                StatCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 130
                    title: qsTr("Revenue")
                    value: root.formatMoney(root.totalRevenue())
                    subtitle: qsTr("From %n invoice(s)", "", root.invoicesData.length)
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
                        text: qsTr("Recent Sales")
                        color: Theme.textPrimary
                        font.pixelSize: Theme.fontMedium
                        font.weight: Font.Bold
                    }

                    Text {
                        text: qsTr("No sales yet.")
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
                                    text: qsTr("Sale #%1").arg(modelData.id)
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
