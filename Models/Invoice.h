#ifndef INVOICE_H
#define INVOICE_H

#include <QString>
#include <QDate>

// ? Represents the payment method used for an invoice.
enum class PaymentMethod
{
    Cash,
    CreditCard,
    BankTransfer,
    Financing
};

//* Represents an invoice generated for a completed vehicle sale.
class Invoice
{
private:
    int m_id;

    int m_saleId;

    QString m_invoiceNumber;

    QDate m_issueDate;

    double m_subtotal;
    double m_tax;
    double m_total;

    PaymentMethod m_paymentMethod;

public:
    //* Constructors & Destructor
    Invoice();

    ~Invoice() = default;

    //? Used when creating a new invoice before saving it to the database.
    Invoice(
        int saleId,
        const QString& invoiceNumber,
        const QDate& issueDate,
        double subtotal,
        double tax,
        double total,
        PaymentMethod paymentMethod
    );

    //? Used when constructing an invoice object from an existing database record.
    Invoice(
        int id,
        int saleId,
        const QString& invoiceNumber,
        const QDate& issueDate,
        double subtotal,
        double tax,
        double total,
        PaymentMethod paymentMethod
    );

    //* Getters
    int id() const;

    int saleId() const;

    const QString& invoiceNumber() const;

    const QDate& issueDate() const;

    double subtotal() const;
    double tax() const;
    double total() const;

    PaymentMethod paymentMethod() const;

    //* Setters
    //! IDs are immutable after creation.

    bool setInvoiceNumber(const QString& invoiceNumber);

    bool setIssueDate(const QDate& issueDate);

    bool setSubtotal(double subtotal);

    bool setTax(double tax);

    bool setTotal(double total);

    void setPaymentMethod(PaymentMethod paymentMethod);
};

#endif // INVOICE_H