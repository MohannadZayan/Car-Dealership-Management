#include "authservice.h"

#include <QDebug>

AuthService::AuthService(EmployeeService* employeeService)
    : m_employeeService(employeeService)
{
}

bool AuthService::login(const QString& email, const QString& password)
{
    if (m_employeeService == nullptr)
    {
        qWarning() << "AuthService: No EmployeeService configured.";
        return false;
    }

    Employee* employee = m_employeeService->findEmployeeByEmail(email);

    // Deliberately the same generic failure for "no such employee" and "wrong
    // password" — distinguishing them would let a caller enumerate valid emails.
    if (employee == nullptr || !employee->verifyPassword(password))
    {
        qWarning() << "AuthService: Login failed for" << email;
        return false;
    }

    m_currentEmployeeId = employee->id();
    m_isLoggedIn = true;
    return true;
}

void AuthService::logout()
{
    m_currentEmployeeId = 0;
    m_isLoggedIn = false;
}

bool AuthService::isLoggedIn() const
{
    return m_isLoggedIn && currentEmployee() != nullptr;
}

Employee* AuthService::currentEmployee() const
{
    if (!m_isLoggedIn || m_employeeService == nullptr)
    {
        return nullptr;
    }

    return m_employeeService->findEmployeeById(m_currentEmployeeId);
}
