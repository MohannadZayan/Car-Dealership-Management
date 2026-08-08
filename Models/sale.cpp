#include "sale.h"

#include <stdexcept>

// * Default constructor
Sale::Sale()
    : m_id(0),
      m_carId(0),
      m_customerId(0),
      m_employeeId(0),
      m_salePrice(0.0),
      m_saleDate()
{
}

// * Parameterized constructor
Sale::Sale(
    int carId,
    int customerId,
    int employeeId,
    double salePrice,
    const QDate& saleDate)
    : Sale(
          0,
          carId,
          customerId,
          employeeId,
          salePrice,
          saleDate)
{
}

// * Database constructor
Sale::Sale(
    int id,
    int carId,
    int customerId,
    int employeeId,
    double salePrice,
    const QDate& saleDate)
    : m_id(id),
      m_carId(carId),
      m_customerId(customerId),
      m_employeeId(employeeId),
      m_salePrice(salePrice),
      m_saleDate(saleDate)
{
    //! First layer of validation

    if (id < 0)
        throw std::invalid_argument("Sale ID cannot be negative");

    if (carId <= 0)
        throw std::invalid_argument("Car ID must be positive");

    if (customerId <= 0)
        throw std::invalid_argument("Customer ID must be positive");

    if (employeeId <= 0)
        throw std::invalid_argument("Employee ID must be positive");

    if (salePrice < 0.0)
        throw std::invalid_argument("Sale price cannot be negative");

    if (!saleDate.isValid())
        throw std::invalid_argument("Sale date is invalid");
}

// * Getters
int Sale::id() const
{
    return m_id;
}

int Sale::carId() const
{
    return m_carId;
}

int Sale::customerId() const
{
    return m_customerId;
}

int Sale::employeeId() const
{
    return m_employeeId;
}

double Sale::salePrice() const
{
    return m_salePrice;
}

const QDate& Sale::saleDate() const
{
    return m_saleDate;
}

// * Setters
bool Sale::setSalePrice(double salePrice)
{
    //! Second layer of validation

    if (salePrice < 0.0)
    {
        return false;
    }

    m_salePrice = salePrice;
    return true;
}

bool Sale::setSaleDate(const QDate& saleDate)
{
    //! Second layer of validation

    if (!saleDate.isValid())
    {
        return false;
    }

    m_saleDate = saleDate;
    return true;
}