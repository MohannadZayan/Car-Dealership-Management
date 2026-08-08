#include "employee.h"
#include "employeefiltercriteria.h"

#include <stdexcept>

#include <QCryptographicHash>
#include <QPasswordDigestor>
#include <QRandomGenerator>

namespace
{
    // ? PBKDF2-HMAC-SHA256 parameters. 210000 iterations is well above older-generation
    // ? recommendations; this runs once at login on a single local desktop client, not
    // ? server-side under load, so the extra cost is negligible in practice.
    constexpr int kPbkdf2Iterations = 210000;
    constexpr int kDerivedKeyLength = 32; // 256 bits, matching SHA-256's output size.
    constexpr int kSaltLength = 16;
    constexpr int kMinPasswordLength = 8;

    QByteArray generateSalt()
    {
        QByteArray salt(kSaltLength, '\0');

        // QRandomGenerator::system() draws from the OS's CSPRNG, appropriate for a
        // security-sensitive salt (unlike the faster QRandomGenerator::global()).
        QRandomGenerator::system()->fillRange(
            reinterpret_cast<quint32*>(salt.data()),
            kSaltLength / static_cast<int>(sizeof(quint32))
        );

        return salt;
    }

    QByteArray derive(const QString& password, const QByteArray& salt)
    {
        return QPasswordDigestor::deriveKeyPbkdf2(
            QCryptographicHash::Sha256,
            password.toUtf8(),
            salt,
            kPbkdf2Iterations,
            kDerivedKeyLength
        );
    }
}

// * Default constructor
Employee::Employee()
    : m_id(0),
      m_firstName(""),
      m_lastName(""),
      m_role(EmployeeRole::SalesPerson),
      m_salary(0.0),
      m_phone(""),
      m_email(""),
      m_passwordHash(""),
      m_passwordSalt("")
{
}

// * Parameterized constructor
Employee::Employee(
    const QString& firstName,
    const QString& lastName,
    EmployeeRole role,
    double salary,
    const QString& phone,
    const QString& email,
    const QString& password)
    : m_id(0),
      m_firstName(firstName),
      m_lastName(lastName),
      m_role(role),
      m_salary(salary),
      m_phone(phone),
      m_email(email),
      m_passwordHash(""),
      m_passwordSalt("")
{
    //! First layer of validation

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

    if (password.length() < kMinPasswordLength)
        throw std::invalid_argument("Password must be at least 8 characters long");

    // Hashing needs a freshly generated salt via setPassword() — it can't be done
    // as a plain member-initializer-list copy the way the other fields are.
    setPassword(password);
}

// * Database constructor
Employee::Employee(
    int id,
    const QString& firstName,
    const QString& lastName,
    EmployeeRole role,
    double salary,
    const QString& phone,
    const QString& email,
    const QString& passwordHash,
    const QString& passwordSalt)
    : m_id(id),
      m_firstName(firstName),
      m_lastName(lastName),
      m_role(role),
      m_salary(salary),
      m_phone(phone),
      m_email(email),
      m_passwordHash(passwordHash),
      m_passwordSalt(passwordSalt)
{
    //! First layer of validation

    if (id <= 0)
        throw std::invalid_argument("ID must be a positive database identifier");

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

    // ! passwordHash/passwordSalt are deliberately NOT validated as non-empty here:
    // ! an employee row created before authentication existed will have both empty,
    // ! and that's a legitimate state — verifyPassword() refuses to authenticate it
    // ! rather than this constructor throwing and breaking loadEmployees() for
    // ! every employee whenever one legacy row without a password exists.
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

const QString& Employee::passwordHash() const
{
    return m_passwordHash;
}

const QString& Employee::passwordSalt() const
{
    return m_passwordSalt;
}

// * Authentication implementation

bool Employee::setPassword(const QString& password)
{
    //! Second layer of validation

    if (password.length() < kMinPasswordLength)
        return false;

    const QByteArray salt = generateSalt();
    const QByteArray hash = derive(password, salt);

    m_passwordSalt = QString::fromLatin1(salt.toHex());
    m_passwordHash = QString::fromLatin1(hash.toHex());
    return true;
}

bool Employee::verifyPassword(const QString& password) const
{
    if (m_passwordHash.isEmpty() || m_passwordSalt.isEmpty())
        return false; // No password has been set on this record yet.

    const QByteArray salt = QByteArray::fromHex(m_passwordSalt.toLatin1());
    const QByteArray expectedHash = QByteArray::fromHex(m_passwordHash.toLatin1());

    // Plain QByteArray comparison, not constant-time — an acceptable trade-off for a
    // local single-user desktop client where there's no network attacker positioned
    // to measure comparison timing, but worth knowing if this code is ever reused
    // in a server context.
    return derive(password, salt) == expectedHash;
}

// * Filtering implementation

bool Employee::matches(const EmployeeFilterCriteria& criteria) const
{
    if (criteria.role && m_role != *criteria.role)
        return false;

    if (criteria.minSalary && m_salary < *criteria.minSalary)
        return false;

    if (criteria.maxSalary && m_salary > *criteria.maxSalary)
        return false;

    return true;
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