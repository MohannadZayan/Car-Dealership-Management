#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>

struct EmployeeFilterCriteria;

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

    QString m_passwordHash;
    QString m_passwordSalt;

public:
    //* Constructors & Destructor
    Employee();

    ~Employee() = default;

    //? Used when creating a new employee before saving it to the database.
    //? password is plaintext here only — it is hashed immediately and never stored as-is.
    Employee(
        const QString& firstName,
        const QString& lastName,
        EmployeeRole role,
        double salary,
        const QString& phone,
        const QString& email,
        const QString& password
    );

    //? Used when constructing an employee object from an existing database record.
    //? passwordHash/passwordSalt are already-hashed values read back from the database,
    //? never plaintext. May be empty for employees created before authentication existed —
    //? verifyPassword() simply refuses to authenticate such an employee until a password is set.
    Employee(
        int id,
        const QString& firstName,
        const QString& lastName,
        EmployeeRole role,
        double salary,
        const QString& phone,
        const QString& email,
        const QString& passwordHash,
        const QString& passwordSalt
    );

    //* Getters
    int id() const;

    const QString& firstName() const;
    const QString& lastName() const;

    EmployeeRole role() const;

    double salary() const;

    const QString& phone() const;
    const QString& email() const;

    //? The stored PBKDF2 hash/salt (hex-encoded) — never the plaintext password.
    //? Needed by EmployeeService to persist/reload authentication state.
    const QString& passwordHash() const;
    const QString& passwordSalt() const;

    //* Filtering
    bool matches(const EmployeeFilterCriteria& criteria) const; //? Returns true if this employee satisfies every field set in criteria.

    //* Authentication
    //? Hashes password with a freshly generated salt and stores both. Fails (returns false)
    //? if password is shorter than the minimum length — nothing is changed in that case.
    bool setPassword(const QString& password);

    //? Re-derives the hash from password using this employee's stored salt and compares.
    //? Always false if no password has been set yet (e.g. a pre-authentication legacy record).
    bool verifyPassword(const QString& password) const;

    //* Setters
    //! First and last name are immutable after creation.

    void setRole(EmployeeRole role);

    bool setSalary(double salary);

    bool setPhone(const QString& phone);

    bool setEmail(const QString& email);
};

#endif // EMPLOYEE_H