cmake_minimum_required(VERSION 3.16)

project(Car_Dealership_Management VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql Network PrintSupport)

qt_standard_project_setup(REQUIRES 6.10)

# Content of downloaded design from Figma to Qt or similar
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/importedcontent/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/importedcontent)
endif()

qt_add_executable(appCar_Dealership_Management
    main.cpp
)

# `pragma Singleton` inside Theme.qml only governs the QML engine's own runtime
# loading — qt_add_qml_module's static qmldir generation needs this CMake source
# property too, set BEFORE qt_add_qml_module processes QML_FILES, or it registers
# Theme as an ordinary (non-singleton) type and every bare `Theme.xxx` reference
# elsewhere resolves to undefined.
set_source_files_properties(Theme.qml PROPERTIES
    QT_QML_SINGLETON_TYPE true
)

qt_add_qml_module(appCar_Dealership_Management
    URI Car_Dealership_Management

    QML_FILES
        Main.qml
        Theme.qml
        PrimaryButton.qml
        GhostButton.qml
        AppTextField.qml
        AppComboBox.qml
        AppDialog.qml
        Badge.qml
        StatCard.qml
        NavButton.qml
        Sidebar.qml
        TopBar.qml
        Shell.qml
        LoginPage.qml
        DashboardPage.qml
        InventoryPage.qml
        CustomersPage.qml
        EmployeesPage.qml
        SalesPage.qml
        InvoicesPage.qml
        SettingsPage.qml

    SOURCES
        Database/databasemanager.h
        Database/databasemanager.cpp

        Managers/appcontroller.h
        Managers/appcontroller.cpp

        Models/car.h
        Models/car.cpp
        Models/carfiltercriteria.h
        Models/customer.h
        Models/customer.cpp
        Models/employee.h
        Models/employee.cpp
        Models/employeefiltercriteria.h
        Models/employeesummary.h
        Models/sale.h
        Models/sale.cpp
        Models/salefiltercriteria.h
        Models/Invoice.h
        Models/Invoice.cpp

        Services/authorizationservice.h
        Services/authorizationservice.cpp
        Services/authservice.h
        Services/authservice.cpp
        Services/baseservice.h
        Services/carservice.h
        Services/carservice.cpp
        Services/customerservice.h
        Services/customerservice.cpp
        Services/employeeservice.h
        Services/employeeservice.cpp
        Services/saleservice.h
        Services/saleservice.cpp
        Services/Invoiceservice.h
        Services/Invoiceservice.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appCar_Dealership_Management PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appCar_Dealership_Management
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appCar_Dealership_Management
    PRIVATE
        Qt6::Quick
        Qt6::Sql
        Qt6::Network
        Qt6::PrintSupport
)

# QML's automatic type registration includes AppController's header as a bare
# filename (#include <appcontroller.h>), which only resolves if its directory
# is directly on the include path — the project root alone isn't enough.
target_include_directories(appCar_Dealership_Management
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Managers
)

install(TARGETS appCar_Dealership_Management
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
