#ifndef AUTHORIZATIONSERVICE_H
#define AUTHORIZATIONSERVICE_H

#include <QList>

#include "Models/customer.h"
#include "Models/employee.h"
#include "Models/employeesummary.h"
#include "Models/Invoice.h"
#include "Models/sale.h"
#include "Services/authservice.h"
#include "Services/customerservice.h"
#include "Services/employeeservice.h"
#include "Services/Invoiceservice.h"
#include "Services/saleservice.h"

// * Answers "what is the currently logged-in employee allowed to see" —
// * authorization, as distinct from AuthService's authentication ("who are they").
//
// ? Rules:
// ?  - Employees: every authenticated employee can see everyone's name + role
// ?    (employeeSummaries()). Full detail (salary/phone/email) is only visible
// ?    for a Manager, or an employee looking at their own record.
// ?  - Customers: a Manager sees every customer. A SalesPerson/CSR only sees
// ?    customers tied to a sale where they were the assigned employee.
// ?  - Sales: a Manager sees every sale. A SalesPerson/CSR only sees sales
// ?    where they were the assigned employee.
// ?  - Invoices: same rule as Sales, inherited through the invoice's sale
// ?    (an invoice is visible iff the sale it belongs to is visible).
//
// ! This class doesn't own any data itself — it's a read-only lens over
// ! AuthService + the data services, applied at query time, not stored state.
class AuthorizationService
{
private:
    AuthService* m_authService;
    EmployeeService* m_employeeService;
    CustomerService* m_customerService;
    SaleService* m_saleService;
    InvoiceService* m_invoiceService;

    bool currentEmployeeOwnsCustomer(int customerId) const;
    bool currentEmployeeOwnsSale(int saleId) const;

public:
    AuthorizationService(
        AuthService* authService,
        EmployeeService* employeeService,
        CustomerService* customerService,
        SaleService* saleService,
        InvoiceService* invoiceService
    );
    ~AuthorizationService() = default;

    //? Name + role for every employee. Requires being logged in; empty otherwise.
    QList<EmployeeSummary> employeeSummaries() const;

    //? Full Employee record for employeeId, or nullptr if the current employee
    //? isn't allowed to see it (not a Manager and not their own record) or
    //? isn't logged in.
    Employee* employeeDetails(int employeeId) const;

    //? Every customer the current employee is allowed to see: all of them for
    //? a Manager, only their own (by sale ownership) for anyone else. Requires
    //? being logged in; empty otherwise.
    QList<Customer> visibleCustomers() const;

    //? Single customer, or nullptr if the current employee isn't allowed to see
    //? them or isn't logged in.
    Customer* customerDetails(int customerId) const;

    //? Every sale the current employee is allowed to see: all of them for a
    //? Manager, only sales they were the assigned employee on otherwise.
    //? Requires being logged in; empty otherwise.
    QList<Sale> visibleSales() const;

    //? Single sale, or nullptr if the current employee isn't allowed to see it
    //? or isn't logged in.
    Sale* saleDetails(int saleId) const;

    //? Every invoice the current employee is allowed to see, following the
    //? same rule as the sale it belongs to. Requires being logged in;
    //? empty otherwise.
    QList<Invoice> visibleInvoices() const;

    //? Single invoice, or nullptr if the current employee isn't allowed to see
    //? it (i.e. can't see the sale it belongs to) or isn't logged in.
    Invoice* invoiceDetails(int invoiceId) const;
};

#endif // AUTHORIZATIONSERVICE_H
