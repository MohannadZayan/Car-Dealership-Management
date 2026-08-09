#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QTranslator>
#include <QVariantList>
#include <QVariantMap>

#include "Database/databasemanager.h"
#include "Services/authorizationservice.h"
#include "Services/authservice.h"
#include "Services/carservice.h"
#include "Services/customerservice.h"
#include "Services/employeeservice.h"
#include "Services/Invoiceservice.h"
#include "Services/saleservice.h"

// * The single object the QML frontend talks to. Owns the whole backend
// * (DatabaseManager + every *Service + AuthService + AuthorizationService)
// * and exposes it as a simplified, unified interface — a Facade.
//
// ! Every read of Employees/Customers/Sales/Invoices goes through
// ! AuthorizationService, never the raw *Service, so the visibility rules
// ! actually apply to the frontend rather than being easy to bypass.
// ! Cars have no ownership restriction (shared inventory) but every method
// ! except login() still requires being logged in.
class AppController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY authChanged)
    Q_PROPERTY(bool isManager READ isManager NOTIFY authChanged)
    Q_PROPERTY(int currentEmployeeId READ currentEmployeeId NOTIFY authChanged)
    Q_PROPERTY(QString currentEmployeeName READ currentEmployeeName NOTIFY authChanged)
    Q_PROPERTY(QString currentEmployeeRole READ currentEmployeeRole NOTIFY authChanged)
    Q_PROPERTY(QString databaseError READ databaseError NOTIFY databaseErrorChanged)
    Q_PROPERTY(QString currentLanguage READ currentLanguage NOTIFY languageChanged)

private:
    DatabaseManager m_databaseManager;
    CarService m_carService{&m_databaseManager};
    CustomerService m_customerService{&m_databaseManager};
    EmployeeService m_employeeService{&m_databaseManager};
    SaleService m_saleService{&m_databaseManager};
    InvoiceService m_invoiceService{&m_databaseManager};
    AuthService m_authService{&m_employeeService};
    AuthorizationService m_authorizationService{
        &m_authService, &m_employeeService, &m_customerService, &m_saleService, &m_invoiceService
    };

    QString m_databaseError;

    QString m_currentLanguage{"en"};
    QTranslator m_translator;
    QTranslator m_pluralsTranslator;

    //? Every non-auth method starts by checking this; returns true iff a login exists.
    bool requireLogin() const;

    //? Employee CRUD (not summaries/details) is restricted to Managers.
    bool requireManager() const;

public:
    explicit AppController(QObject* parent = nullptr);
    ~AppController() override = default;

    bool isLoggedIn() const;
    bool isManager() const;
    int currentEmployeeId() const;
    QString currentEmployeeName() const;
    QString currentEmployeeRole() const;
    QString databaseError() const;
    QString currentLanguage() const;

    //* Language — "en" or "ar". Swaps the installed QTranslators, retranslates
    //* every live QML string in place (no restart), and flips layout direction
    //* for Arabic's right-to-left reading order.
    Q_INVOKABLE bool setLanguage(const QString& languageCode);

    //* Auth
    Q_INVOKABLE bool login(const QString& email, const QString& password);
    Q_INVOKABLE void logout();
    Q_INVOKABLE bool changeMyPassword(const QString& oldPassword, const QString& newPassword);

    //* Cars — shared inventory, no ownership restriction beyond being logged in.
    Q_INVOKABLE QVariantList cars() const;
    Q_INVOKABLE QVariantList carsByBodyType(int bodyType) const;
    Q_INVOKABLE bool addCar(const QVariantMap& data);
    Q_INVOKABLE bool updateCar(const QVariantMap& data);
    Q_INVOKABLE bool removeCar(int id);
    Q_INVOKABLE QString lastCarError() const;

    //* Customers — visibility via AuthorizationService.
    Q_INVOKABLE QVariantList customers() const;
    Q_INVOKABLE bool addCustomer(const QVariantMap& data);
    Q_INVOKABLE bool updateCustomer(const QVariantMap& data);
    Q_INVOKABLE bool removeCustomer(int id);
    Q_INVOKABLE QString lastCustomerError() const;

    //* Employees — summaries visible to everyone, full detail/mutation restricted.
    Q_INVOKABLE QVariantList employees() const;              //? id/name/role only.
    Q_INVOKABLE QVariantMap employeeDetails(int id) const;    //? Full record, or {} if not permitted.
    Q_INVOKABLE bool addEmployee(const QVariantMap& data);    //? Manager only. data needs "password".
    Q_INVOKABLE bool updateEmployee(const QVariantMap& data); //? Manager only.
    Q_INVOKABLE bool removeEmployee(int id);                  //? Manager only.
    Q_INVOKABLE QString lastEmployeeError() const;

    //* Sales — visibility via AuthorizationService.
    Q_INVOKABLE QVariantList sales() const;
    Q_INVOKABLE bool addSale(const QVariantMap& data);
    Q_INVOKABLE bool updateSale(const QVariantMap& data);
    Q_INVOKABLE bool removeSale(int id);
    Q_INVOKABLE QString lastSaleError() const;

    //* Invoices — visibility via AuthorizationService.
    Q_INVOKABLE QVariantList invoices() const;
    Q_INVOKABLE bool addInvoice(const QVariantMap& data);
    Q_INVOKABLE bool updateInvoice(const QVariantMap& data);
    Q_INVOKABLE bool removeInvoice(int id);
    Q_INVOKABLE QString lastInvoiceError() const;

    //? Renders the invoice to a PDF in the temp directory and opens it in the
    //? OS's default PDF viewer — printing from there is one click. Returns
    //? false if the invoice isn't visible to the current employee or doesn't exist.
    Q_INVOKABLE bool printInvoice(int invoiceId);

signals:
    void authChanged();
    void databaseErrorChanged();
    void languageChanged();
};

#endif // APPCONTROLLER_H
