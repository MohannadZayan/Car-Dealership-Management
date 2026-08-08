#include "authorizationservice.h"

AuthorizationService::AuthorizationService(
    AuthService* authService,
    EmployeeService* employeeService,
    CustomerService* customerService,
    SaleService* saleService
)
    : m_authService(authService),
      m_employeeService(employeeService),
      m_customerService(customerService),
      m_saleService(saleService)
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
