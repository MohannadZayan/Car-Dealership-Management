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

Employee* EmployeeService::findEmployeeByEmail(const QString& email)
{
    return find([&email](const Employee& employee) {
        return employee.email().compare(email, Qt::CaseInsensitive) == 0;
    });
}

QList<Employee> EmployeeService::filterEmployees(const EmployeeFilterCriteria& criteria) const
{
    return filter([&criteria](const Employee& employee) { return employee.matches(criteria); });
}

bool EmployeeService::addEmployee(const Employee& employee)
{
    return guardedExecute("EmployeeService", "addEmployee", [&]() -> bool
    {
        // Employees.email has no DB-level UNIQUE enforcement that also covers
        // case-insensitivity, and login-by-email would be ambiguous with duplicates.
        if (find([&](const Employee& existing) {
                return existing.email().compare(employee.email(), Qt::CaseInsensitive) == 0;
            }) != nullptr)
        {
            m_lastError = ServiceError::DuplicateEmail;
            qWarning() << "EmployeeService: Email already in use:" << employee.email();
            return false;
        }

        QSqlQuery query;

        query.prepare(R"(
            INSERT INTO Employees
            (
                first_name,
                last_name,
                role,
                salary,
                phone,
                email,
                password_hash,
                password_salt
            )
            VALUES
            (
                ?, ?, ?, ?, ?, ?, ?, ?
            )
        )");

        query.addBindValue(employee.firstName());
        query.addBindValue(employee.lastName());
        query.addBindValue(static_cast<int>(employee.role()));
        query.addBindValue(employee.salary());
        query.addBindValue(employee.phone());
        query.addBindValue(employee.email());
        query.addBindValue(employee.passwordHash());
        query.addBindValue(employee.passwordSalt());

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
                email,
                password_hash,
                password_salt
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
                query.value("email").toString(),
                query.value("password_hash").toString(),
                query.value("password_salt").toString()
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
                email = ?,
                password_hash = ?,
                password_salt = ?
            WHERE id = ?
        )");

        query.addBindValue(updatedEmployee.firstName());
        query.addBindValue(updatedEmployee.lastName());
        query.addBindValue(static_cast<int>(updatedEmployee.role()));
        query.addBindValue(updatedEmployee.salary());
        query.addBindValue(updatedEmployee.phone());
        query.addBindValue(updatedEmployee.email());
        query.addBindValue(updatedEmployee.passwordHash());
        query.addBindValue(updatedEmployee.passwordSalt());

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
