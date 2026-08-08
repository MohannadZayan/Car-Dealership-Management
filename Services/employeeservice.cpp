#include "employeeservice.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <exception>

EmployeeService::EmployeeService(DatabaseManager* databaseManager)
    : m_databaseManager(databaseManager)
{

}

const QList<Employee>& EmployeeService::employees() const
{
    return m_employees;
}

Employee* EmployeeService::findEmployeeById(int id)
{
    for (Employee& employee : m_employees)
    {
        if (employee.id() == id)
        {
            return &employee;
        }
    }

    return nullptr;
}

bool EmployeeService::addEmployee(const Employee& employee)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "EmployeeService: Database is not connected.";
        return false;
    }

    try
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
    }
    catch (const std::exception& e)
    {
        qCritical() << "EmployeeService::addEmployee():" << e.what();
        return false;
    }
}

bool EmployeeService::loadEmployees()
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "EmployeeService: Database is not connected.";
        return false;
    }

    try
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

        m_employees.clear();

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

            m_employees.append(employee);
        }

        return true;
    }
    catch (const std::exception& e)
    {
        qCritical() << "EmployeeService::loadEmployees():" << e.what();
        return false;
    }
}

bool EmployeeService::updateEmployee(const Employee& updatedEmployee)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "EmployeeService: Database is not connected.";
        return false;
    }

    try
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
            qWarning() << "EmployeeService: No employee found with ID"
                       << updatedEmployee.id();
            return false;
        }

        return loadEmployees();
    }
    catch (const std::exception& e)
    {
        qCritical() << "EmployeeService::updateEmployee():" << e.what();
        return false;
    }
}

bool EmployeeService::removeEmployee(int id)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "EmployeeService: Database is not connected.";
        return false;
    }

    try
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
            qWarning() << "EmployeeService: No employee found with ID"
                       << id;
            return false;
        }

        return loadEmployees();
    }
    catch (const std::exception& e)
    {
        qCritical() << "EmployeeService::removeEmployee():" << e.what();
        return false;
    }
}