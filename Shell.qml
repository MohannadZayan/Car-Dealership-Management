import QtQuick
import QtQuick.Layouts
import Car_Dealership_Management

// * The authenticated app shell: sidebar + top bar stay fixed, the page area swaps.
// * Each page exposes a `refresh()` function that's called every time it becomes visible,
// * so data is never stale after switching tabs.
Rectangle {
    id: root

    signal loggedOut()

    color: Theme.background

    readonly property var pageIds: ["dashboard", "inventory", "customers", "employees", "sales", "invoices", "settings"]
    readonly property var pageTitles: ({
        "dashboard": "Dashboard",
        "inventory": "Inventory",
        "customers": "Customers",
        "employees": "Employees",
        "sales": "Sales",
        "invoices": "Invoices",
        "settings": "Settings"
    })

    property string currentPage: "dashboard"

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Sidebar {
            Layout.preferredWidth: 232
            Layout.fillHeight: true
            currentPage: root.currentPage
            onNavigate: (page) => { root.currentPage = page }
            onLogout: {
                AppController.logout()
                root.loggedOut()
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            TopBar {
                Layout.fillWidth: true
                Layout.preferredHeight: 76
                title: root.pageTitles[root.currentPage]
            }

            StackLayout {
                id: pages
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: root.pageIds.indexOf(root.currentPage)

                onCurrentIndexChanged: {
                    var item = itemAt(currentIndex)
                    if (item && item.refresh) item.refresh()
                }

                DashboardPage { id: dashboardPage }
                InventoryPage { id: inventoryPage }
                CustomersPage { id: customersPage }
                EmployeesPage { id: employeesPage }
                SalesPage { id: salesPage }
                InvoicesPage { id: invoicesPage }
                SettingsPage { id: settingsPage }
            }
        }
    }

    Component.onCompleted: {
        var item = pages.itemAt(pages.currentIndex)
        if (item && item.refresh) item.refresh()
    }
}
