#ifndef EMPLOYEESUMMARY_H
#define EMPLOYEESUMMARY_H

#include <QString>

#include "employee.h"

// * A non-sensitive view of an Employee — id, name, and role only.
// ? Unlike the full Employee record, this is safe to show to any authenticated
// ? employee regardless of role: no salary, phone, email, or password data.
struct EmployeeSummary
{
    int id;
    QString firstName;
    QString lastName;
    EmployeeRole role;
};

#endif // EMPLOYEESUMMARY_H
