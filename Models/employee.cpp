#include "employee.h"

#include <stdexcept>

// * Default constructor
Employee::Employee()
    : m_id(0),
      m_firstName(""),
      m_lastName(""),
      m_role(EmployeeRole::SalesPerson),
      m_salary(0.0),
      m_phone(""),
      m_email("")
{
}

// * Parameterized constructor
Employee::Employee(
    const QString& firstName,
    const QString& lastName,
    EmployeeRole role,
    double salary,
    const QString& phone,
    const QString& email)
    : Employee(
          0,
          firstName,
          lastName,
          role,
          salary,
          phone,
          email)
{
}

// * Database constructor
Employee::Employee(
    int id,
    const QString& firstName,
    const QString& lastName,
    EmployeeRole role,
    double salary,
    const QString& phone,
    const QString& email)
    : m_id(id),
      m_firstName(firstName),
      m_lastName(lastName),
      m_role(role),
      m_salary(salary),
      m_phone(phone),
      m_email(email)
{
    //! First layer of validation

    if (id < 0)
        throw std::invalid_argument("Employee ID cannot be negative");

    if (firstName.trimmed().isEmpty())
        throw std::invalid_argument("First name cannot be empty");

    if (lastName.trimmed().isEmpty())
        throw std::invalid_argument("Last name cannot be empty");

    if (salary < 0.0)
        throw std::invalid_argument("Salary cannot be negative");

    if (phone.trimmed().isEmpty())
        throw std::invalid_argument("Phone cannot be empty");

    if (phone.length() < 8)
        throw std::invalid_argument("Phone must be at least 8 characters long");

    if (email.trimmed().isEmpty())
        throw std::invalid_argument("Email cannot be empty");

    if (!email.contains('@'))
        throw std::invalid_argument("Invalid email address");
}

// * Getters
int Employee::id() const
{
    return m_id;
}

const QString& Employee::firstName() const
{
    return m_firstName;
}

const QString& Employee::lastName() const
{
    return m_lastName;
}

EmployeeRole Employee::role() const
{
    return m_role;
}

double Employee::salary() const
{
    return m_salary;
}

const QString& Employee::phone() const
{
    return m_phone;
}

const QString& Employee::email() const
{
    return m_email;
}

void Employee::setRole(EmployeeRole role)
{
    m_role = role;
}

bool Employee::setSalary(double salary)
{
    //! Second layer of validation

    if (salary < 0.0)
        return false;

    m_salary = salary;
    return true;
}

bool Employee::setPhone(const QString& phone)
{
    //! Second layer of validation

    if (phone.trimmed().isEmpty())
        return false;

    if (phone.length() < 8)
        return false;

    m_phone = phone;
    return true;
}

bool Employee::setEmail(const QString& email)
{
    //! Second layer of validation

    if (email.trimmed().isEmpty())
        return false;

    if (!email.contains('@'))
        return false;

    m_email = email;
    return true;
}