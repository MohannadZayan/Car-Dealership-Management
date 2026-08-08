#ifndef EMPLOYEESERVICE_H
#define EMPLOYEESERVICE_H

#include <QList>

#include "Models/employee.h"
#include "Database/databasemanager.h"

class EmployeeService
{
private:
    QList<Employee> m_employees;
    DatabaseManager* m_databaseManager;

public:
    explicit EmployeeService(DatabaseManager* databaseManager);
    ~EmployeeService() = default;

    //* Employee Management
    bool addEmployee(const Employee& employee);
    bool updateEmployee(const Employee& updatedEmployee);
    bool removeEmployee(int id);

    //* Database
    bool loadEmployees();

    //* Retrieval
    const QList<Employee>& employees() const;

    //? Returns a pointer to the employee with the specified ID,
    //? or nullptr if no employee with that ID exists.
    Employee* findEmployeeById(int id);
};

#endif // EMPLOYEESERVICE_H