#include "employeeservice.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <exception>

EmployeeService::EmployeeService(DatabaseManager* databaseManager)
    : BaseService<Employee>(databaseManager)
{

}

const QList<Employee>& EmployeeService::employees() const
{
    return entities();
}

Employee* EmployeeService::findEmployeeById(int id)
{
    return find([id](const Employee& employee) { return employee.id() == id; });
}

QList<Employee> EmployeeService::filterEmployees(const EmployeeFilterCriteria& criteria) const
{
    return filter([&criteria](const Employee& employee) { return employee.matches(criteria); });
}

bool EmployeeService::addEmployee(const Employee& employee)
{
    return guardedExecute("EmployeeService", "addEmployee", [&]() -> bool
    {
        QSqlQuery query;

        query.prepare(R"(
            INSERT INTO Employees
            (
                first_name,
                last_name,
                role,
                salary,
                phone,
                email
            )
            VALUES
            (
                ?, ?, ?, ?, ?, ?
            )
        )");

        query.addBindValue(employee.firstName());
        query.addBindValue(employee.lastName());
        query.addBindValue(static_cast<int>(employee.role()));
        query.addBindValue(employee.salary());
        query.addBindValue(employee.phone());
        query.addBindValue(employee.email());

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        return loadEmployees();
    });
}

bool EmployeeService::loadEmployees()
{
    return guardedExecute("EmployeeService", "loadEmployees", [&]() -> bool
    {
        QSqlQuery query;

        query.prepare(R"(
            SELECT
                id,
                first_name,
                last_name,
                role,
                salary,
                phone,
                email
            FROM Employees;
        )");

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        m_entities.clear();

        while (query.next())
        {
            Employee employee(
                query.value("id").toInt(),
                query.value("first_name").toString(),
                query.value("last_name").toString(),
                static_cast<EmployeeRole>(query.value("role").toInt()),
                query.value("salary").toDouble(),
                query.value("phone").toString(),
                query.value("email").toString()
            );

            m_entities.append(employee);
        }

        return true;
    });
}

bool EmployeeService::updateEmployee(const Employee& updatedEmployee)
{
    return guardedExecute("EmployeeService", "updateEmployee", [&]() -> bool
    {
        QSqlQuery query;

        query.prepare(R"(
            UPDATE Employees
            SET
                first_name = ?,
                last_name = ?,
                role = ?,
                salary = ?,
                phone = ?,
                email = ?
            WHERE id = ?
        )");

        query.addBindValue(updatedEmployee.firstName());
        query.addBindValue(updatedEmployee.lastName());
        query.addBindValue(static_cast<int>(updatedEmployee.role()));
        query.addBindValue(updatedEmployee.salary());
        query.addBindValue(updatedEmployee.phone());
        query.addBindValue(updatedEmployee.email());

        query.addBindValue(updatedEmployee.id());

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        if (query.numRowsAffected() == 0)
        {
            m_lastError = ServiceError::NotFound;
            qWarning() << "EmployeeService: No employee found with ID"
                       << updatedEmployee.id();
            return false;
        }

        return loadEmployees();
    });
}

bool EmployeeService::removeEmployee(int id)
{
    return guardedExecute("EmployeeService", "removeEmployee", [&]() -> bool
    {
        QSqlQuery query;

        query.prepare(R"(
            DELETE FROM Employees
            WHERE id = ?
        )");

        query.addBindValue(id);

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        if (query.numRowsAffected() == 0)
        {
            m_lastError = ServiceError::NotFound;
            qWarning() << "EmployeeService: No employee found with ID"
                       << id;
            return false;
        }

        return loadEmployees();
    });
}
