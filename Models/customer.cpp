#include "customer.h"

#include <stdexcept>

// * Default constructor
Customer::Customer()
    : m_id(0),
      m_firstName(""),
      m_lastName(""),
      m_phone(""),
      m_email(""),
      m_address("")
{
}

// * Parameterized constructor
Customer::Customer(
    const QString& firstName,
    const QString& lastName,
    const QString& phone,
    const QString& email,
    const QString& address)
    : Customer(
          0,
          firstName,
          lastName,
          phone,
          email,
          address)
{
}

// * Database constructor
Customer::Customer(
    int id,
    const QString& firstName,
    const QString& lastName,
    const QString& phone,
    const QString& email,
    const QString& address)
    : m_id(id),
      m_firstName(firstName),
      m_lastName(lastName),
      m_phone(phone),
      m_email(email),
      m_address(address)
{
    //! First layer of validation

    if (id < 0)
    {
        throw std::invalid_argument("Customer ID cannot be negative");
    }

    if (firstName.trimmed().isEmpty())
    {
        throw std::invalid_argument("First name cannot be empty");
    }

    if (lastName.trimmed().isEmpty())
    {
        throw std::invalid_argument("Last name cannot be empty");
    }

    if (phone.trimmed().isEmpty())
    {
        throw std::invalid_argument("Phone cannot be empty");
    }

    if (phone.length() < 8)
    {
        throw std::invalid_argument("Phone must be at least 8 characters long");
    }

    if (email.trimmed().isEmpty())
    {
        throw std::invalid_argument("Email cannot be empty");
    }

    if (!email.contains('@'))
    {
        throw std::invalid_argument("Invalid email address");
    }

    if (address.trimmed().isEmpty())
    {
        throw std::invalid_argument("Address cannot be empty");
    }
}

// * Getters
int Customer::id() const
{
    return m_id;
}

const QString& Customer::firstName() const
{
    return m_firstName;
}

const QString& Customer::lastName() const
{
    return m_lastName;
}

const QString& Customer::phone() const
{
    return m_phone;
}

const QString& Customer::email() const
{
    return m_email;
}

const QString& Customer::address() const
{
    return m_address;
}

// * Setters
bool Customer::setPhone(const QString& phone)
{
    //! Second layer of validation

    if (phone.trimmed().isEmpty())
    {
        return false;
    }

    if (phone.length() < 8)
    {
        return false;
    }

    m_phone = phone;
    return true;
}

bool Customer::setEmail(const QString& email)
{
    //! Second layer of validation

    if (email.trimmed().isEmpty())
    {
        return false;
    }

    if (!email.contains('@'))
    {
        return false;
    }

    m_email = email;
    return true;
}

bool Customer::setAddress(const QString& address)
{
    //! Second layer of validation

    if (address.trimmed().isEmpty())
    {
        return false;
    }

    m_address = address;
    return true;
}