import QtQuick
import QtQuick.Layouts
import Car_Dealership_Management

// * Persistent left navigation. Emits `navigate(page)` with a page-id string that
// * Shell.qml maps to the visible page; highlights whichever one is current.
Rectangle {
    id: root

    property string currentPage: "dashboard"
    property bool isManager: false

    signal navigate(string page)
    signal logout()

    color: Theme.surface

    Rectangle {
        anchors.right: parent.right
        width: 1
        height: parent.height
        color: Theme.border
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingMedium
        spacing: 4

        RowLayout {
            Layout.fillWidth: true
            Layout.bottomMargin: Theme.spacingLarge
            Layout.topMargin: Theme.spacingSmall
            spacing: 10

            Text { text: "🚗"; font.pixelSize: 22 }
            Text {
                text: "Dealership"
                color: Theme.textPrimary
                font.pixelSize: 18
                font.weight: Font.Bold
            }
        }

        NavButton {
            Layout.fillWidth: true
            icon: "▦"
            label: "Dashboard"
            active: root.currentPage === "dashboard"
            onClicked: root.navigate("dashboard")
        }
        NavButton {
            Layout.fillWidth: true
            icon: "🚘"
            label: "Inventory"
            active: root.currentPage === "inventory"
            onClicked: root.navigate("inventory")
        }
        NavButton {
            Layout.fillWidth: true
            icon: "👤"
            label: "Customers"
            active: root.currentPage === "customers"
            onClicked: root.navigate("customers")
        }
        NavButton {
            Layout.fillWidth: true
            icon: "💼"
            label: "Employees"
            active: root.currentPage === "employees"
            onClicked: root.navigate("employees")
        }
        NavButton {
            Layout.fillWidth: true
            icon: "🤝"
            label: "Sales"
            active: root.currentPage === "sales"
            onClicked: root.navigate("sales")
        }
        NavButton {
            Layout.fillWidth: true
            icon: "🧾"
            label: "Invoices"
            active: root.currentPage === "invoices"
            onClicked: root.navigate("invoices")
        }

        Item { Layout.fillHeight: true }

        Rectangle { Layout.fillWidth: true; height: 1; color: Theme.border; Layout.bottomMargin: 4 }

        NavButton {
            Layout.fillWidth: true
            icon: "⚙"
            label: "Settings"
            active: root.currentPage === "settings"
            onClicked: root.navigate("settings")
        }
        NavButton {
            Layout.fillWidth: true
            icon: "⏻"
            label: "Log Out"
            onClicked: root.logout()
        }
    }
}
