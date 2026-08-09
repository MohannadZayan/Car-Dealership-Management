# Dealership Management

A desktop application for running a car dealership's day-to-day operations — inventory, customers, employees, sales, and invoices — built with **Qt 6 / QML** for the frontend and **C++** for the backend.

<p>
  <img alt="Qt" src="https://img.shields.io/badge/Qt-6.10-41CD52?logo=qt&logoColor=white">
  <img alt="C++" src="https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white">
  <img alt="SQLite" src="https://img.shields.io/badge/SQLite-embedded-003B57?logo=sqlite&logoColor=white">
  <img alt="Platform" src="https://img.shields.io/badge/platform-Linux-informational">
</p>

## What it does

Log in as an employee and the app gives you a role-aware dashboard for the whole dealership:

- **Dashboard** — inventory/sales/revenue stats at a glance, scoped to what you're allowed to see.
- **Inventory** — add, browse, and remove vehicles (make/model/year/price/mileage/body type/transmission/VIN, etc.). Selling a car automatically flips it to *Sold*; removing a sale reverts it.
- **Customers** — track contact info and see which customers are "yours."
- **Employees** — Manager-only roster management (roles, salary, contact info); everyone can see names and roles, only Managers (or the employee themselves) can see the rest.
- **Sales** — record a sale linking a car, customer, and employee.
- **Invoices** — generate an invoice off a sale and print/export it straight to PDF.
- **Settings** — change your password, toggle dark/light mode, and switch the app's language.

## Who sees what

Access isn't just login-gated — it's role-aware, enforced centrally by an `AuthorizationService` rather than left to each page to get right:

| Role | Cars | Customers / Sales | Employees |
|---|---|---|---|
| **Manager** | Full access | Everyone's | Full roster + full detail |
| **SalesPerson** / **CustomerServiceRepresentative** | Full access (shared inventory) | Only their own (a customer "belongs" to whoever made the first sale to them) | Names + roles only; full detail on their own record |

## Multi-language

The whole UI is available in **English and Arabic**, switchable live from Settings — no restart needed. Arabic also gets proper right-to-left layout mirroring. Under the hood this uses Qt's standard Linguist toolchain (`qt_add_lupdate` → `qt_add_lrelease` → `qt_add_resources`), so adding another language later is a matter of translating one more `.ts` file, not touching application code.

## Architecture

```
QML UI  →  AppController (facade)  →  *Service layer  →  DatabaseManager (SQLite)
```

- **Models** (`Models/`) — plain data classes (`Car`, `Customer`, `Employee`, `Sale`, `Invoice`) plus their enums (`VehicleBodyType`, `CarStatus`, `EmployeeRole`, `PaymentMethod`, …).
- **Services** (`Services/`) — one service per entity (`CarService`, `CustomerService`, `EmployeeService`, `SaleService`, `InvoiceService`), all sharing a common `BaseService` for find/filter logic, plus `AuthService` (login/session) and `AuthorizationService` (the visibility rules above).
- **AppController** (`Managers/appcontroller.h/.cpp`) — the single object QML talks to. A Facade over every service, so the frontend never touches raw services or bypasses authorization.
- **DatabaseManager** (`Database/`) — owns the SQLite connection, schema creation, and lightweight in-place migrations (`ALTER TABLE` when a column is missing).
- **QML frontend** — a shared design system (`Theme.qml`, `AppDialog`, `AppComboBox`, `AppTextField`, `PrimaryButton`, …) used consistently across every page, with a `Sidebar` + `TopBar` shell and per-page views (`DashboardPage`, `InventoryPage`, etc.).

This layering means the backend has no idea QML exists — `AppController` is the only bridge — which is what makes it possible to unit-test the services in isolation or swap the frontend later without touching business logic.

## Tech stack

- **Qt 6.10** (Quick, Sql, Network, PrintSupport, LinguistTools)
- **C++17**
- **QML** for the UI, with Qt Quick Controls (Basic style) for form widgets
- **SQLite** via `QtSql`, with foreign keys enforced
- **CMake** build system (`qt_add_qml_module`, `qt_add_lupdate`/`lrelease`/`resources`)

## Building from source

Requires Qt 6.10+ with the Quick/Sql/Network/PrintSupport/LinguistTools components, and CMake 3.16+.

```bash
cmake -B build -S .
cmake --build build -j$(nproc)
./build/appCar_Dealership_Management
```

On first run, the app seeds itself with a sample database (a handful of vehicles and two employee accounts) if it doesn't find an existing one at its standard data location — you won't start with an empty app.

## Running the packaged build

No Qt installation needed — grab the AppImage, mark it executable, and run it:

```bash
chmod +x Dealership_Management-x86_64.AppImage
./Dealership_Management-x86_64.AppImage
```

**Demo login:** `manager@dealership.com` / `Manager123!`

## Project layout

```
.
├── Main.qml, Shell.qml, *Page.qml     — QML screens
├── App*.qml, Theme.qml, ...           — shared QML design-system components
├── Managers/appcontroller.*           — AppController facade (QML <-> C++ boundary)
├── Services/                          — business logic + authorization
├── Models/                            — data classes + enums
├── Database/                          — SQLite connection, schema, seed data
├── i18n/                              — Qt Linguist translation sources (.ts)
└── CMakeLists.txt
```
