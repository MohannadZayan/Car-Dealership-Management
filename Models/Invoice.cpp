#include "Invoice.h"

#include <stdexcept>

// * Default constructor
Invoice::Invoice()
    : m_id(0),
      m_saleId(0),
      m_invoiceNumber(""),
      m_issueDate(),
      m_subtotal(0.0),
      m_tax(0.0),
      m_total(0.0),
      m_paymentMethod(PaymentMethod::Cash)
{
}

// * Parameterized constructor
Invoice::Invoice(
    int saleId,
    const QString& invoiceNumber,
    const QDate& issueDate,
    double subtotal,
    double tax,
    double total,
    PaymentMethod paymentMethod)
    : Invoice(
          0,
          saleId,
          invoiceNumber,
          issueDate,
          subtotal,
          tax,
          total,
          paymentMethod)
{
}

// * Database constructor
Invoice::Invoice(
    int id,
    int saleId,
    const QString& invoiceNumber,
    const QDate& issueDate,
    double subtotal,
    double tax,
    double total,
    PaymentMethod paymentMethod)
    : m_id(id),
      m_saleId(saleId),
      m_invoiceNumber(invoiceNumber),
      m_issueDate(issueDate),
      m_subtotal(subtotal),
      m_tax(tax),
      m_total(total),
      m_paymentMethod(paymentMethod)
{
    //! First layer of validation

    if (id < 0)
        throw std::invalid_argument("Invoice ID cannot be negative");

    if (saleId <= 0)
        throw std::invalid_argument("Sale ID must be positive");

    if (invoiceNumber.trimmed().isEmpty())
        throw std::invalid_argument("Invoice number cannot be empty");

    if (!issueDate.isValid())
        throw std::invalid_argument("Issue date is invalid");

    if (subtotal < 0.0)
        throw std::invalid_argument("Subtotal cannot be negative");

    if (tax < 0.0)
        throw std::invalid_argument("Tax cannot be negative");

    if (total < 0.0)
        throw std::invalid_argument("Total cannot be negative");
}

// * Getters
int Invoice::id() const
{
    return m_id;
}

int Invoice::saleId() const
{
    return m_saleId;
}

const QString& Invoice::invoiceNumber() const
{
    return m_invoiceNumber;
}

const QDate& Invoice::issueDate() const
{
    return m_issueDate;
}

double Invoice::subtotal() const
{
    return m_subtotal;
}

double Invoice::tax() const
{
    return m_tax;
}

double Invoice::total() const
{
    return m_total;
}

PaymentMethod Invoice::paymentMethod() const
{
    return m_paymentMethod;
}

// * Setters
bool Invoice::setInvoiceNumber(const QString& invoiceNumber)
{
    //! Second layer of validation

    if (invoiceNumber.trimmed().isEmpty())
        return false;

    m_invoiceNumber = invoiceNumber;
    return true;
}

bool Invoice::setIssueDate(const QDate& issueDate)
{
    //! Second layer of validation

    if (!issueDate.isValid())
        return false;

    m_issueDate = issueDate;
    return true;
}

bool Invoice::setSubtotal(double subtotal)
{
    //! Second layer of validation

    if (subtotal < 0.0)
        return false;

    m_subtotal = subtotal;
    return true;
}

bool Invoice::setTax(double tax)
{
    //! Second layer of validation

    if (tax < 0.0)
        return false;

    m_tax = tax;
    return true;
}

bool Invoice::setTotal(double total)
{
    //! Second layer of validation

    if (total < 0.0)
        return false;

    m_total = total;
    return true;
}

void Invoice::setPaymentMethod(PaymentMethod paymentMethod)
{
    m_paymentMethod = paymentMethod;
}