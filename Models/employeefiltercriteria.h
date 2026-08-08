#ifndef EMPLOYEEFILTERCRITERIA_H
#define EMPLOYEEFILTERCRITERIA_H

#include <optional>

#include "employee.h"

// * Set only the fields you want to filter on; unset fields are ignored.
struct EmployeeFilterCriteria
{
    std::optional<EmployeeRole> role;

    std::optional<double> minSalary;
    std::optional<double> maxSalary;
};

#endif // EMPLOYEEFILTERCRITERIA_H
