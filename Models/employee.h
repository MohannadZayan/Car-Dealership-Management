#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>

// ? Represents the role of an employee in the dealership.
enum class EmployeeRole
{
    Manager,
    SalesPerson,
    CustomerServiceRepresentative
};

//* Represents an employee of the dealership.
class Employee
{
private:
    int m_id;

    QString m_firstName;
    QString m_lastName;

    EmployeeRole m_role;

    double m_salary;

    QString m_phone;
    QString m_email;

public:
    //* Constructors & Destructor
    Employee();

    ~Employee() = default;

    //? Used when creating a new employee before saving it to the database.
    Employee(
        const QString& firstName,
        const QString& lastName,
        EmployeeRole role,
        double salary,
        const QString& phone,
        const QString& email
    );

    //? Used when constructing an employee object from an existing database record.
    Employee(
        int id,
        const QString& firstName,
        const QString& lastName,
        EmployeeRole role,
        double salary,
        const QString& phone,
        const QString& email
    );

    //* Getters
    int id() const;

    const QString& firstName() const;
    const QString& lastName() const;

    EmployeeRole role() const;

    double salary() const;

    const QString& phone() const;
    const QString& email() const;

    //* Setters
    //! First and last name are immutable after creation.

    void setRole(EmployeeRole role);

    bool setSalary(double salary);

    bool setPhone(const QString& phone);

    bool setEmail(const QString& email);
};

#endif // EMPLOYEE_H