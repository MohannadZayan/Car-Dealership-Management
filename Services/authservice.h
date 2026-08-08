#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include "Models/employee.h"
#include "Services/employeeservice.h"

// * Handles employee login/logout and tracks who is currently authenticated.
// ! Authorization — what a role is allowed to see/do — lives on Employee::role();
// ! this class only answers "who is logged in", not "what are they allowed to do".
class AuthService
{
private:
    EmployeeService* m_employeeService;

    // Only the id is kept, not a pointer into EmployeeService's cache: that cache is
    // rebuilt (m_entities cleared + refilled) on every add/update/remove, which would
    // leave a stored Employee* dangling. Re-looking-up by id on every call instead
    // means the session can never dangle and never returns stale data after an edit.
    int m_currentEmployeeId = 0;
    bool m_isLoggedIn = false;

public:
    explicit AuthService(EmployeeService* employeeService);
    ~AuthService() = default;

    //? Looks the employee up by email and verifies password against their stored hash.
    //? On success, currentEmployee() reflects them and this returns true.
    bool login(const QString& email, const QString& password);

    void logout();

    bool isLoggedIn() const;

    //? Returns the currently logged-in employee, or nullptr if no one is logged in
    //? (including if they were deleted from the database mid-session).
    Employee* currentEmployee() const;
};

#endif // AUTHSERVICE_H
