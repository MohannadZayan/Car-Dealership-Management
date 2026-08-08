#include "authorizationservice.h"

AuthorizationService::AuthorizationService(
    AuthService* authService,
    EmployeeService* employeeService,
    CustomerService* customerService,
    SaleService* saleService,
    InvoiceService* invoiceService
)
    : m_authService(authService),
      m_employeeService(employeeService),
      m_customerService(customerService),
      m_saleService(saleService),
      m_invoiceService(invoiceService)
{
}

bool AuthorizationService::currentEmployeeOwnsCustomer(int customerId) const
{
    Employee* currentEmployee = m_authService->currentEmployee();

    if (currentEmployee == nullptr)
    {
        return false;
    }

    return m_saleService->find([&](const Sale& sale) {
        return sale.employeeId() == currentEmployee->id() && sale.customerId() == customerId;
    }) != nullptr;
}

bool AuthorizationService::currentEmployeeOwnsSale(int saleId) const
{
    Employee* currentEmployee = m_authService->currentEmployee();

    if (currentEmployee == nullptr)
    {
        return false;
    }

    Sale* sale = m_saleService->findSaleById(saleId);

    return sale != nullptr && sale->employeeId() == currentEmployee->id();
}

QList<EmployeeSummary> AuthorizationService::employeeSummaries() const
{
    if (!m_authService->isLoggedIn())
    {
        return {};
    }

    QList<EmployeeSummary> result;

    for (const Employee& employee : m_employeeService->employees())
    {
        result.append(EmployeeSummary{
            employee.id(),
            employee.firstName(),
            employee.lastName(),
            employee.role()
        });
    }

    return result;
}

Employee* AuthorizationService::employeeDetails(int employeeId) const
{
    Employee* currentEmployee = m_authService->currentEmployee();

    if (currentEmployee == nullptr)
    {
        return nullptr;
    }

    if (currentEmployee->role() == EmployeeRole::Manager || currentEmployee->id() == employeeId)
    {
        return m_employeeService->findEmployeeById(employeeId);
    }

    return nullptr;
}

QList<Customer> AuthorizationService::visibleCustomers() const
{
    Employee* currentEmployee = m_authService->currentEmployee();

    if (currentEmployee == nullptr)
    {
        return {};
    }

    if (currentEmployee->role() == EmployeeRole::Manager)
    {
        return m_customerService->customers();
    }

    return m_customerService->filter([this](const Customer& customer) {
        return currentEmployeeOwnsCustomer(customer.id());
    });
}

Customer* AuthorizationService::customerDetails(int customerId) const
{
    Employee* currentEmployee = m_authService->currentEmployee();

    if (currentEmployee == nullptr)
    {
        return nullptr;
    }

    if (currentEmployee->role() == EmployeeRole::Manager || currentEmployeeOwnsCustomer(customerId))
    {
        return m_customerService->findCustomerById(customerId);
    }

    return nullptr;
}

QList<Sale> AuthorizationService::visibleSales() const
{
    Employee* currentEmployee = m_authService->currentEmployee();

    if (currentEmployee == nullptr)
    {
        return {};
    }

    if (currentEmployee->role() == EmployeeRole::Manager)
    {
        return m_saleService->sales();
    }

    const int employeeId = currentEmployee->id();

    return m_saleService->filter([employeeId](const Sale& sale) {
        return sale.employeeId() == employeeId;
    });
}

Sale* AuthorizationService::saleDetails(int saleId) const
{
    Employee* currentEmployee = m_authService->currentEmployee();

    if (currentEmployee == nullptr)
    {
        return nullptr;
    }

    if (currentEmployee->role() == EmployeeRole::Manager || currentEmployeeOwnsSale(saleId))
    {
        return m_saleService->findSaleById(saleId);
    }

    return nullptr;
}

QList<Invoice> AuthorizationService::visibleInvoices() const
{
    Employee* currentEmployee = m_authService->currentEmployee();

    if (currentEmployee == nullptr)
    {
        return {};
    }

    if (currentEmployee->role() == EmployeeRole::Manager)
    {
        return m_invoiceService->invoices();
    }

    return m_invoiceService->filter([this](const Invoice& invoice) {
        return currentEmployeeOwnsSale(invoice.saleId());
    });
}

Invoice* AuthorizationService::invoiceDetails(int invoiceId) const
{
    Employee* currentEmployee = m_authService->currentEmployee();

    if (currentEmployee == nullptr)
    {
        return nullptr;
    }

    Invoice* invoice = m_invoiceService->findInvoiceById(invoiceId);

    if (invoice == nullptr)
    {
        return nullptr;
    }

    if (currentEmployee->role() == EmployeeRole::Manager || currentEmployeeOwnsSale(invoice->saleId()))
    {
        return invoice;
    }

    return nullptr;
}
