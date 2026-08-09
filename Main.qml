import QtQuick
import QtQuick.Controls.Basic
import Car_Dealership_Management

ApplicationWindow {
    id: window
    width: 1320
    height: 840
    minimumWidth: 980
    minimumHeight: 620
    visible: true
    title: qsTr("Dealership Management")
    color: Theme.background

    LayoutMirroring.enabled: Qt.application.layoutDirection === Qt.RightToLeft
    LayoutMirroring.childrenInherit: true

    Behavior on color { ColorAnimation { duration: Theme.animMedium } }

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: loginComponent

        pushEnter: Transition { NumberAnimation { property: "opacity"; from: 0; to: 1; duration: Theme.animMedium } }
        pushExit: Transition { NumberAnimation { property: "opacity"; from: 1; to: 0; duration: Theme.animMedium } }
        replaceEnter: Transition { NumberAnimation { property: "opacity"; from: 0; to: 1; duration: Theme.animMedium } }
        replaceExit: Transition { NumberAnimation { property: "opacity"; from: 1; to: 0; duration: Theme.animMedium } }
        popEnter: Transition { NumberAnimation { property: "opacity"; from: 0; to: 1; duration: Theme.animMedium } }
        popExit: Transition { NumberAnimation { property: "opacity"; from: 1; to: 0; duration: Theme.animMedium } }
    }

    Component {
        id: loginComponent
        LoginPage {
            onLoggedIn: stack.replace(shellComponent)
        }
    }

    Component {
        id: shellComponent
        Shell {
            onLoggedOut: stack.replace(loginComponent)
        }
    }
}
