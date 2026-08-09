import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic as Controls
import Car_Dealership_Management

// * Every employee's name + role is visible to everyone; full detail (salary,
// * phone, email) only opens for a Manager or your own row — AppController
// * returns an empty map for anything else, which this page treats as "locked".
Item {
    id: root

    property var employeesData: []

    function refresh() {
        employeesData = AppController.employees()
    }

    readonly property var roleOptions: [
        { text: qsTr("Manager"), value: 0 },
        { text: qsTr("SalesPerson"), value: 1 },
        { text: qsTr("CustomerServiceRepresentative"), value: 2 }
    ]

    function roleTone(role) {
        return role === 0 ? "accent" : role === 1 ? "success" : "warning"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingXLarge
        spacing: Theme.spacingLarge

        RowLayout {
            Layout.fillWidth: true

            Text {
                text: qsTr("%n employee(s)", "", root.employeesData.length)
                color: Theme.textMuted
                font.pixelSize: Theme.fontBody
                Layout.fillWidth: true
            }

            PrimaryButton {
                text: qsTr("+ Add Employee")
                visible: AppController.isManager
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

                    Text { text: qsTr("Name"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.preferredWidth: 220 }
                    Text { text: qsTr("Role"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; Layout.fillWidth: true }
                    Text { text: ""; Layout.preferredWidth: 90 }
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: Theme.border }

                Controls.ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    ListView {
                        model: root.employeesData
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
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    var details = AppController.employeeDetails(modelData.id)
                                    detailDialog.employeeName = modelData.firstName + " " + modelData.lastName
                                    detailDialog.employeeRole = modelData.roleName
                                    detailDialog.hasAccess = Object.keys(details).length > 0
                                    detailDialog.employeeSalary = details.salary !== undefined ? details.salary : 0
                                    detailDialog.employeePhone = details.phone !== undefined ? details.phone : ""
                                    detailDialog.employeeEmail = details.email !== undefined ? details.email : ""
                                    detailDialog.open()
                                }
                            }

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: Theme.spacingMedium

                                Text { text: modelData.firstName + " " + modelData.lastName; color: Theme.textPrimary; font.pixelSize: Theme.fontBody; font.weight: Font.DemiBold; Layout.preferredWidth: 220 }

                                Item {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Badge {
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: modelData.roleName
                                        tone: root.roleTone(modelData.role)
                                    }
                                }

                                Text {
                                    text: qsTr("View →")
                                    color: Theme.textMuted
                                    font.pixelSize: Theme.fontSmall
                                    Layout.preferredWidth: 90
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // --- Employee detail (view-only) ---
    AppDialog {
        id: detailDialog
        title: employeeName
        showFooter: false

        property string employeeName: ""
        property string employeeRole: ""
        property bool hasAccess: false
        property real employeeSalary: 0
        property string employeePhone: ""
        property string employeeEmail: ""

        Text {
            width: parent.width
            text: qsTr("🔒 You don't have permission to view this employee's details.")
            color: Theme.textMuted
            font.pixelSize: Theme.fontBody
            wrapMode: Text.WordWrap
            visible: !detailDialog.hasAccess
        }

        Column {
            width: parent.width
            spacing: Theme.spacingMedium
            visible: detailDialog.hasAccess

            Row {
                width: parent.width
                Text { text: qsTr("Role"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; width: 100 }
                Text { text: detailDialog.employeeRole; color: Theme.textPrimary; font.pixelSize: Theme.fontBody }
            }
            Row {
                width: parent.width
                Text { text: qsTr("Salary"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; width: 100 }
                Text { text: "$" + Math.round(detailDialog.employeeSalary).toLocaleString(); color: Theme.textPrimary; font.pixelSize: Theme.fontBody }
            }
            Row {
                width: parent.width
                Text { text: qsTr("Phone"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; width: 100 }
                Text { text: detailDialog.employeePhone; color: Theme.textPrimary; font.pixelSize: Theme.fontBody }
            }
            Row {
                width: parent.width
                Text { text: qsTr("Email"); color: Theme.textMuted; font.pixelSize: Theme.fontSmall; width: 100 }
                Text { text: detailDialog.employeeEmail; color: Theme.textPrimary; font.pixelSize: Theme.fontBody }
            }
        }

        GhostButton {
            text: qsTr("Close")
            onClicked: detailDialog.close()
        }
    }

    // --- Add Employee (Manager only) ---
    AppDialog {
        id: addDialog
        title: qsTr("Add Employee")
        confirmText: qsTr("Add Employee")

        confirmEnabled: firstNameField.text.trim().length > 0
                         && lastNameField.text.trim().length > 0
                         && emailField.text.trim().length > 0
                         && passwordField.text.length >= 8

        onConfirmed: {
            busy = true
            var ok = AppController.addEmployee({
                firstName: firstNameField.text,
                lastName: lastNameField.text,
                role: roleCombo.currentValue !== undefined ? roleCombo.currentValue : 1,
                salary: parseFloat(salaryField.text) || 0,
                phone: phoneField.text,
                email: emailField.text,
                password: passwordField.text
            })
            busy = false

            if (ok) {
                close()
                firstNameField.text = ""; lastNameField.text = ""; salaryField.text = ""
                phoneField.text = ""; emailField.text = ""; passwordField.text = ""
                root.refresh()
            } else {
                errorText = AppController.lastEmployeeError() || qsTr("Could not add this employee.")
            }
        }

        RowLayout {
            width: parent.width
            spacing: Theme.spacingMedium
            AppTextField { id: firstNameField; Layout.fillWidth: true; label: qsTr("First Name"); placeholder: qsTr("Sam") }
            AppTextField { id: lastNameField; Layout.fillWidth: true; label: qsTr("Last Name"); placeholder: qsTr("SalesOne") }
        }
        AppComboBox { id: roleCombo; width: parent.width; label: qsTr("Role"); options: root.roleOptions; currentValue: 1 }
        RowLayout {
            width: parent.width
            spacing: Theme.spacingMedium
            AppTextField { id: salaryField; Layout.fillWidth: true; label: qsTr("Salary ($/yr)"); placeholder: "50000" }
            AppTextField { id: phoneField; Layout.fillWidth: true; label: qsTr("Phone"); placeholder: "555-0100" }
        }
        AppTextField { id: emailField; width: parent.width; label: qsTr("Email"); placeholder: "sam@dealership.com" }
        AppTextField { id: passwordField; width: parent.width; label: qsTr("Temporary Password"); placeholder: qsTr("At least 8 characters"); isPassword: true }
    }

    Component.onCompleted: refresh()
}
