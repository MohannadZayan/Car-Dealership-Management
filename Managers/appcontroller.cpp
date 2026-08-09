#include "appcontroller.h"

#include <exception>

#include <QDate>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QPrinter>
#include <QStandardPaths>
#include <QTextDocument>
#include <QUrl>

namespace
{
    QString employeeRoleToString(EmployeeRole role)
    {
        switch (role)
        {
        case EmployeeRole::Manager:
            return "Manager";
        case EmployeeRole::SalesPerson:
            return "SalesPerson";
        case EmployeeRole::CustomerServiceRepresentative:
            return "CustomerServiceRepresentative";
        }
        return QString();
    }

    QString vehicleBodyTypeToString(VehicleBodyType bodyType)
    {
        switch (bodyType)
        {
        case VehicleBodyType::Sedan: return "Sedan";
        case VehicleBodyType::SUV: return "SUV";
        case VehicleBodyType::Coupe: return "Coupe";
        case VehicleBodyType::Hatchback: return "Hatchback";
        case VehicleBodyType::Convertible: return "Convertible";
        case VehicleBodyType::Pickup: return "Pickup";
        case VehicleBodyType::Wagon: return "Wagon";
        case VehicleBodyType::Van: return "Van";
        }
        return QString();
    }

    QString transmissionTypeToString(TransmissionType transmission)
    {
        switch (transmission)
        {
        case TransmissionType::Manual: return "Manual";
        case TransmissionType::Automatic: return "Automatic";
        }
        return QString();
    }

    QString carStatusToString(CarStatus status)
    {
        switch (status)
        {
        case CarStatus::Available: return "Available";
        case CarStatus::Reserved: return "Reserved";
        case CarStatus::Sold: return "Sold";
        case CarStatus::InTransit: return "InTransit";
        }
        return QString();
    }

    QString paymentMethodToString(PaymentMethod method)
    {
        switch (method)
        {
        case PaymentMethod::Cash: return "Cash";
        case PaymentMethod::CreditCard: return "CreditCard";
        case PaymentMethod::BankTransfer: return "BankTransfer";
        case PaymentMethod::Financing: return "Financing";
        }
        return QString();
    }

    // ? Enum fields are sent to/from QML as plain int (the ordinal value) plus a
    // ? "<field>Name" human-readable string for display — QML never needs to know
    // ? the C++ enum type, just the number it already has and a label to show.

    QVariantMap carToMap(const Car& car)
    {
        QVariantMap map;
        map["id"] = car.id();
        map["make"] = car.make();
        map["model"] = car.model();
        map["year"] = car.year();
        map["price"] = car.price();
        map["mileage"] = car.mileage();
        map["color"] = car.color();
        map["engineName"] = car.engineName();
        map["horsepower"] = car.horsepower();
        map["transmission"] = static_cast<int>(car.transmission());
        map["transmissionName"] = transmissionTypeToString(car.transmission());
        map["bodyType"] = static_cast<int>(car.bodyType());
        map["bodyTypeName"] = vehicleBodyTypeToString(car.bodyType());
        map["isNew"] = car.isNew();
        map["licensePlate"] = car.licensePlate();
        map["vin"] = car.vin();
        map["status"] = static_cast<int>(car.status());
        map["statusName"] = carStatusToString(car.status());
        return map;
    }

    QVariantMap customerToMap(const Customer& customer)
    {
        QVariantMap map;
        map["id"] = customer.id();
        map["firstName"] = customer.firstName();
        map["lastName"] = customer.lastName();
        map["phone"] = customer.phone();
        map["email"] = customer.email();
        map["address"] = customer.address();
        return map;
    }

    QVariantMap employeeSummaryToMap(const EmployeeSummary& summary)
    {
        QVariantMap map;
        map["id"] = summary.id;
        map["firstName"] = summary.firstName;
        map["lastName"] = summary.lastName;
        map["role"] = static_cast<int>(summary.role);
        map["roleName"] = employeeRoleToString(summary.role);
        return map;
    }

    QVariantMap employeeToMap(const Employee& employee)
    {
        QVariantMap map;
        map["id"] = employee.id();
        map["firstName"] = employee.firstName();
        map["lastName"] = employee.lastName();
        map["role"] = static_cast<int>(employee.role());
        map["roleName"] = employeeRoleToString(employee.role());
        map["salary"] = employee.salary();
        map["phone"] = employee.phone();
        map["email"] = employee.email();
        return map;
    }

    QVariantMap saleToMap(const Sale& sale)
    {
        QVariantMap map;
        map["id"] = sale.id();
        map["carId"] = sale.carId();
        map["customerId"] = sale.customerId();
        map["employeeId"] = sale.employeeId();
        map["salePrice"] = sale.salePrice();
        map["saleDate"] = sale.saleDate();
        return map;
    }

    QVariantMap invoiceToMap(const Invoice& invoice)
    {
        QVariantMap map;
        map["id"] = invoice.id();
        map["saleId"] = invoice.saleId();
        map["invoiceNumber"] = invoice.invoiceNumber();
        map["issueDate"] = invoice.issueDate();
        map["subtotal"] = invoice.subtotal();
        map["tax"] = invoice.tax();
        map["total"] = invoice.total();
        map["paymentMethod"] = static_cast<int>(invoice.paymentMethod());
        map["paymentMethodName"] = paymentMethodToString(invoice.paymentMethod());
        return map;
    }

    QString buildInvoiceHtml(
        const Invoice& invoice,
        const Sale& sale,
        const Customer& customer,
        const Car& car,
        const Employee& employee)
    {
        QString html;

        html += "<html><body style=\"font-family: sans-serif;\">";

        html += "<h1>Invoice " + invoice.invoiceNumber() + "</h1>";
        html += "<p><b>Issue Date:</b> " + invoice.issueDate().toString(Qt::ISODate) + "<br/>";
        html += "<b>Payment Method:</b> " + paymentMethodToString(invoice.paymentMethod()) + "</p>";

        html += "<h3>Bill To</h3>";
        html += "<p>" + customer.firstName() + " " + customer.lastName() + "<br/>";
        html += customer.phone() + "<br/>";
        html += customer.address() + "</p>";

        html += "<h3>Vehicle Sold</h3>";
        html += "<table border=\"1\" cellpadding=\"6\" cellspacing=\"0\" width=\"100%\">";
        html += "<tr><th>Make</th><th>Model</th><th>Year</th><th>VIN</th><th>Sale Price</th></tr>";
        html += "<tr><td>" + car.make() + "</td><td>" + car.model() + "</td><td>"
                + QString::number(car.year()) + "</td><td>" + car.vin() + "</td><td>$"
                + QString::number(sale.salePrice(), 'f', 2) + "</td></tr>";
        html += "</table>";

        html += "<h3>Totals</h3>";
        html += "<table cellpadding=\"4\">";
        html += "<tr><td>Subtotal</td><td>$" + QString::number(invoice.subtotal(), 'f', 2) + "</td></tr>";
        html += "<tr><td>Tax</td><td>$" + QString::number(invoice.tax(), 'f', 2) + "</td></tr>";
        html += "<tr><td><b>Total</b></td><td><b>$" + QString::number(invoice.total(), 'f', 2) + "</b></td></tr>";
        html += "</table>";

        html += "<p><b>Sold by:</b> " + employee.firstName() + " " + employee.lastName()
                + " (" + employeeRoleToString(employee.role()) + ")</p>";

        html += "</body></html>";

        return html;
    }
}

AppController::AppController(QObject* parent)
    : QObject(parent)
{
    if (!m_databaseManager.connectDatabase())
    {
        m_databaseError = "Failed to connect to the database.";
        return;
    }

    if (!m_databaseManager.createTables())
    {
        m_databaseError = "Failed to set up the database tables.";
        return;
    }

    m_carService.loadCars();
    m_customerService.loadCustomers();
    m_employeeService.loadEmployees();
    m_saleService.loadSales();
    m_invoiceService.loadInvoices();
}

bool AppController::requireLogin() const
{
    return m_authService.isLoggedIn();
}

bool AppController::requireManager() const
{
    Employee* current = m_authService.currentEmployee();
    return current != nullptr && current->role() == EmployeeRole::Manager;
}

bool AppController::isLoggedIn() const
{
    return m_authService.isLoggedIn();
}

bool AppController::isManager() const
{
    return requireManager();
}

int AppController::currentEmployeeId() const
{
    Employee* current = m_authService.currentEmployee();
    return current == nullptr ? 0 : current->id();
}

QString AppController::currentEmployeeName() const
{
    Employee* current = m_authService.currentEmployee();
    return current == nullptr ? QString() : current->firstName() + " " + current->lastName();
}

QString AppController::currentEmployeeRole() const
{
    Employee* current = m_authService.currentEmployee();
    return current == nullptr ? QString() : employeeRoleToString(current->role());
}

QString AppController::databaseError() const
{
    return m_databaseError;
}

// * Auth

bool AppController::login(const QString& email, const QString& password)
{
    const bool success = m_authService.login(email, password);
    emit authChanged();
    return success;
}

void AppController::logout()
{
    m_authService.logout();
    emit authChanged();
}

bool AppController::changeMyPassword(const QString& oldPassword, const QString& newPassword)
{
    if (!requireLogin())
    {
        return false;
    }

    Employee* current = m_authService.currentEmployee();

    if (current == nullptr || !current->verifyPassword(oldPassword))
    {
        return false;
    }

    if (!current->setPassword(newPassword))
    {
        return false; // newPassword too short.
    }

    return m_employeeService.updateEmployee(*current);
}

// * Cars

QVariantList AppController::cars() const
{
    if (!requireLogin())
    {
        return {};
    }

    QVariantList result;

    for (const Car& car : m_carService.cars())
    {
        result.append(carToMap(car));
    }

    return result;
}

QVariantList AppController::carsByBodyType(int bodyType) const
{
    if (!requireLogin())
    {
        return {};
    }

    QVariantList result;

    for (const Car& car : m_carService.carsByBodyType(static_cast<VehicleBodyType>(bodyType)))
    {
        result.append(carToMap(car));
    }

    return result;
}

bool AppController::addCar(const QVariantMap& data)
{
    if (!requireLogin())
    {
        return false;
    }

    try
    {
        Car car(
            data.value("make").toString(),
            data.value("model").toString(),
            data.value("year").toInt(),
            data.value("price").toDouble(),
            data.value("mileage").toInt(),
            data.value("color").toString(),
            data.value("engineName").toString(),
            data.value("horsepower").toInt(),
            static_cast<TransmissionType>(data.value("transmission").toInt()),
            static_cast<VehicleBodyType>(data.value("bodyType").toInt()),
            data.value("isNew").toBool(),
            data.value("vin").toString()
        );

        return m_carService.addCar(car);
    }
    catch (const std::exception& e)
    {
        qWarning() << "AppController::addCar():" << e.what();
        return false;
    }
}

bool AppController::updateCar(const QVariantMap& data)
{
    if (!requireLogin())
    {
        return false;
    }

    try
    {
        Car car(
            data.value("id").toInt(),
            data.value("make").toString(),
            data.value("model").toString(),
            data.value("year").toInt(),
            data.value("price").toDouble(),
            data.value("mileage").toInt(),
            data.value("color").toString(),
            data.value("engineName").toString(),
            data.value("horsepower").toInt(),
            static_cast<TransmissionType>(data.value("transmission").toInt()),
            static_cast<VehicleBodyType>(data.value("bodyType").toInt()),
            data.value("isNew").toBool(),
            data.value("licensePlate").toString(),
            data.value("vin").toString(),
            static_cast<CarStatus>(data.value("status").toInt())
        );

        return m_carService.updateCar(car);
    }
    catch (const std::exception& e)
    {
        qWarning() << "AppController::updateCar():" << e.what();
        return false;
    }
}

bool AppController::removeCar(int id)
{
    return requireLogin() && m_carService.removeCar(id);
}

QString AppController::lastCarError() const
{
    return m_carService.lastErrorString();
}

// * Customers

QVariantList AppController::customers() const
{
    if (!requireLogin())
    {
        return {};
    }

    QVariantList result;

    for (const Customer& customer : m_authorizationService.visibleCustomers())
    {
        result.append(customerToMap(customer));
    }

    return result;
}

bool AppController::addCustomer(const QVariantMap& data)
{
    if (!requireLogin())
    {
        return false;
    }

    try
    {
        Customer customer(
            data.value("firstName").toString(),
            data.value("lastName").toString(),
            data.value("phone").toString(),
            data.value("email").toString(),
            data.value("address").toString()
        );

        return m_customerService.addCustomer(customer);
    }
    catch (const std::exception& e)
    {
        qWarning() << "AppController::addCustomer():" << e.what();
        return false;
    }
}

bool AppController::updateCustomer(const QVariantMap& data)
{
    if (!requireLogin())
    {
        return false;
    }

    try
    {
        Customer customer(
            data.value("id").toInt(),
            data.value("firstName").toString(),
            data.value("lastName").toString(),
            data.value("phone").toString(),
            data.value("email").toString(),
            data.value("address").toString()
        );

        return m_customerService.updateCustomer(customer);
    }
    catch (const std::exception& e)
    {
        qWarning() << "AppController::updateCustomer():" << e.what();
        return false;
    }
}

bool AppController::removeCustomer(int id)
{
    return requireLogin() && m_customerService.removeCustomer(id);
}

QString AppController::lastCustomerError() const
{
    return m_customerService.lastErrorString();
}

// * Employees

QVariantList AppController::employees() const
{
    if (!requireLogin())
    {
        return {};
    }

    QVariantList result;

    for (const EmployeeSummary& summary : m_authorizationService.employeeSummaries())
    {
        result.append(employeeSummaryToMap(summary));
    }

    return result;
}

QVariantMap AppController::employeeDetails(int id) const
{
    if (!requireLogin())
    {
        return {};
    }

    Employee* employee = m_authorizationService.employeeDetails(id);
    return employee == nullptr ? QVariantMap{} : employeeToMap(*employee);
}

bool AppController::addEmployee(const QVariantMap& data)
{
    if (!requireManager())
    {
        return false;
    }

    try
    {
        Employee employee(
            data.value("firstName").toString(),
            data.value("lastName").toString(),
            static_cast<EmployeeRole>(data.value("role").toInt()),
            data.value("salary").toDouble(),
            data.value("phone").toString(),
            data.value("email").toString(),
            data.value("password").toString()
        );

        return m_employeeService.addEmployee(employee);
    }
    catch (const std::exception& e)
    {
        qWarning() << "AppController::addEmployee():" << e.what();
        return false;
    }
}

bool AppController::updateEmployee(const QVariantMap& data)
{
    if (!requireManager())
    {
        return false;
    }

    const int id = data.value("id").toInt();
    Employee* existing = m_employeeService.findEmployeeById(id);

    if (existing == nullptr)
    {
        return false;
    }

    try
    {
        // Password is deliberately carried over from the existing record, not taken
        // from data — changing a password is changeMyPassword()'s job, not this one's.
        Employee employee(
            id,
            data.value("firstName").toString(),
            data.value("lastName").toString(),
            static_cast<EmployeeRole>(data.value("role").toInt()),
            data.value("salary").toDouble(),
            data.value("phone").toString(),
            data.value("email").toString(),
            existing->passwordHash(),
            existing->passwordSalt()
        );

        return m_employeeService.updateEmployee(employee);
    }
    catch (const std::exception& e)
    {
        qWarning() << "AppController::updateEmployee():" << e.what();
        return false;
    }
}

bool AppController::removeEmployee(int id)
{
    return requireManager() && m_employeeService.removeEmployee(id);
}

QString AppController::lastEmployeeError() const
{
    return m_employeeService.lastErrorString();
}

// * Sales

QVariantList AppController::sales() const
{
    if (!requireLogin())
    {
        return {};
    }

    QVariantList result;

    for (const Sale& sale : m_authorizationService.visibleSales())
    {
        result.append(saleToMap(sale));
    }

    return result;
}

bool AppController::addSale(const QVariantMap& data)
{
    if (!requireLogin())
    {
        return false;
    }

    try
    {
        Sale sale(
            data.value("carId").toInt(),
            data.value("customerId").toInt(),
            data.value("employeeId").toInt(),
            data.value("salePrice").toDouble(),
            data.value("saleDate").toDate()
        );

        if (!m_saleService.addSale(sale))
        {
            return false;
        }

        // A sold car is no longer available inventory — reflect that in its status.
        // Not wrapped in a DB transaction with the insert above (this codebase
        // doesn't use transactions for multi-step writes); if this update fails,
        // the sale itself still stands, just with a stale car status, logged below.
        Car* soldCar = m_carService.findCarById(sale.carId());
        if (soldCar != nullptr)
        {
            soldCar->setStatus(CarStatus::Sold);

            if (!m_carService.updateCar(*soldCar))
            {
                qWarning() << "AppController::addSale(): sale recorded but failed to mark car"
                           << sale.carId() << "as sold:" << m_carService.lastErrorString();
            }
        }

        return true;
    }
    catch (const std::exception& e)
    {
        qWarning() << "AppController::addSale():" << e.what();
        return false;
    }
}

bool AppController::updateSale(const QVariantMap& data)
{
    if (!requireLogin())
    {
        return false;
    }

    try
    {
        Sale sale(
            data.value("id").toInt(),
            data.value("carId").toInt(),
            data.value("customerId").toInt(),
            data.value("employeeId").toInt(),
            data.value("salePrice").toDouble(),
            data.value("saleDate").toDate()
        );

        if (!m_saleService.updateSale(sale))
        {
            return false;
        }

        Car* soldCar = m_carService.findCarById(sale.carId());
        if (soldCar != nullptr)
        {
            soldCar->setStatus(CarStatus::Sold);

            if (!m_carService.updateCar(*soldCar))
            {
                qWarning() << "AppController::updateSale(): sale updated but failed to mark car"
                           << sale.carId() << "as sold:" << m_carService.lastErrorString();
            }
        }

        return true;
    }
    catch (const std::exception& e)
    {
        qWarning() << "AppController::updateSale():" << e.what();
        return false;
    }
}

bool AppController::removeSale(int id)
{
    if (!requireLogin())
    {
        return false;
    }

    // Grab the car this sale pointed at *before* removing the sale — once it's
    // gone, there's no way to look that up from the sale record anymore.
    Sale* sale = m_saleService.findSaleById(id);
    const int carId = sale != nullptr ? sale->carId() : -1;

    if (!m_saleService.removeSale(id))
    {
        return false;
    }

    if (carId != -1)
    {
        Car* car = m_carService.findCarById(carId);

        // Only revert if still marked Sold — if it's since been manually set to
        // Reserved/InTransit for some other reason, don't clobber that.
        if (car != nullptr && car->status() == CarStatus::Sold)
        {
            car->setStatus(CarStatus::Available);

            if (!m_carService.updateCar(*car))
            {
                qWarning() << "AppController::removeSale(): sale removed but failed to revert car"
                           << carId << "to available:" << m_carService.lastErrorString();
            }
        }
    }

    return true;
}

QString AppController::lastSaleError() const
{
    return m_saleService.lastErrorString();
}

// * Invoices

QVariantList AppController::invoices() const
{
    if (!requireLogin())
    {
        return {};
    }

    QVariantList result;

    for (const Invoice& invoice : m_authorizationService.visibleInvoices())
    {
        result.append(invoiceToMap(invoice));
    }

    return result;
}

bool AppController::addInvoice(const QVariantMap& data)
{
    if (!requireLogin())
    {
        return false;
    }

    try
    {
        Invoice invoice(
            data.value("saleId").toInt(),
            data.value("invoiceNumber").toString(),
            data.value("issueDate").toDate(),
            data.value("subtotal").toDouble(),
            data.value("tax").toDouble(),
            data.value("total").toDouble(),
            static_cast<PaymentMethod>(data.value("paymentMethod").toInt())
        );

        return m_invoiceService.addInvoice(invoice);
    }
    catch (const std::exception& e)
    {
        qWarning() << "AppController::addInvoice():" << e.what();
        return false;
    }
}

bool AppController::updateInvoice(const QVariantMap& data)
{
    if (!requireLogin())
    {
        return false;
    }

    try
    {
        Invoice invoice(
            data.value("id").toInt(),
            data.value("saleId").toInt(),
            data.value("invoiceNumber").toString(),
            data.value("issueDate").toDate(),
            data.value("subtotal").toDouble(),
            data.value("tax").toDouble(),
            data.value("total").toDouble(),
            static_cast<PaymentMethod>(data.value("paymentMethod").toInt())
        );

        return m_invoiceService.updateInvoice(invoice);
    }
    catch (const std::exception& e)
    {
        qWarning() << "AppController::updateInvoice():" << e.what();
        return false;
    }
}

bool AppController::removeInvoice(int id)
{
    return requireLogin() && m_invoiceService.removeInvoice(id);
}

QString AppController::lastInvoiceError() const
{
    return m_invoiceService.lastErrorString();
}

bool AppController::printInvoice(int invoiceId)
{
    if (!requireLogin())
    {
        return false;
    }

    Invoice* invoice = m_authorizationService.invoiceDetails(invoiceId);

    if (invoice == nullptr)
    {
        return false;
    }

    Sale* sale = m_authorizationService.saleDetails(invoice->saleId());

    if (sale == nullptr)
    {
        return false;
    }

    // Sale visibility already implies the current employee is entitled to see
    // its customer/car/employee, so plain lookups (not AuthorizationService) are fine here.
    Customer* customer = m_customerService.findCustomerById(sale->customerId());
    Car* car = m_carService.findCarById(sale->carId());
    Employee* employee = m_employeeService.findEmployeeById(sale->employeeId());

    if (customer == nullptr || car == nullptr || employee == nullptr)
    {
        return false;
    }

    const QString html = buildInvoiceHtml(*invoice, *sale, *customer, *car, *employee);

    QTextDocument document;
    document.setHtml(html);

    const QString outputDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QDir().mkpath(outputDir);
    const QString outputPath = outputDir + QDir::separator()
                                + "Invoice-" + QString::number(invoice->id()) + ".pdf";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(outputPath);

    document.print(&printer);

    return QDesktopServices::openUrl(QUrl::fromLocalFile(outputPath));
}
