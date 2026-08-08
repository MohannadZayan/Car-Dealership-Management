#include "Invoiceservice.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <exception>

InvoiceService::InvoiceService(DatabaseManager* databaseManager)
    : m_databaseManager(databaseManager)
{

}

const QList<Invoice>& InvoiceService::invoices() const
{
    return m_invoices;
}

Invoice* InvoiceService::findInvoiceById(int id)
{
    for (Invoice& invoice : m_invoices)
    {
        if (invoice.id() == id)
        {
            return &invoice;
        }
    }

    return nullptr;
}

bool InvoiceService::addInvoice(const Invoice& invoice)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "InvoiceService: Database is not connected.";
        return false;
    }

    try
    {
        QSqlQuery query;

        query.prepare(R"(
            INSERT INTO Invoices
            (
                sale_id,
                invoice_number,
                issue_date,
                subtotal,
                tax,
                total,
                payment_method
            )
            VALUES
            (
                ?, ?, ?, ?, ?, ?, ?
            )
        )");

        query.addBindValue(invoice.saleId());
        query.addBindValue(invoice.invoiceNumber());
        query.addBindValue(invoice.issueDate().toString(Qt::ISODate));
        query.addBindValue(invoice.subtotal());
        query.addBindValue(invoice.tax());
        query.addBindValue(invoice.total());
        query.addBindValue(static_cast<int>(invoice.paymentMethod()));

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        return loadInvoices();
    }
    catch (const std::exception& e)
    {
        qCritical() << "InvoiceService::addInvoice():" << e.what();
        return false;
    }
}

bool InvoiceService::loadInvoices()
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "InvoiceService: Database is not connected.";
        return false;
    }

    try
    {
        QSqlQuery query;

        query.prepare(R"(
            SELECT
                id,
                sale_id,
                invoice_number,
                issue_date,
                subtotal,
                tax,
                total,
                payment_method
            FROM Invoices;
        )");

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        m_invoices.clear();

        while (query.next())
        {
            Invoice invoice(
                query.value("id").toInt(),
                query.value("sale_id").toInt(),
                query.value("invoice_number").toString(),
                QDate::fromString(
                    query.value("issue_date").toString(),
                    Qt::ISODate
                ),
                query.value("subtotal").toDouble(),
                query.value("tax").toDouble(),
                query.value("total").toDouble(),
                static_cast<PaymentMethod>(
                    query.value("payment_method").toInt()
                )
            );

            m_invoices.append(invoice);
        }

        return true;
    }
    catch (const std::exception& e)
    {
        qCritical() << "InvoiceService::loadInvoices():" << e.what();
        return false;
    }
}

bool InvoiceService::updateInvoice(const Invoice& updatedInvoice)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "InvoiceService: Database is not connected.";
        return false;
    }

    try
    {
        QSqlQuery query;

        query.prepare(R"(
            UPDATE Invoices
            SET
                sale_id = ?,
                invoice_number = ?,
                issue_date = ?,
                subtotal = ?,
                tax = ?,
                total = ?,
                payment_method = ?
            WHERE id = ?
        )");

        query.addBindValue(updatedInvoice.saleId());
        query.addBindValue(updatedInvoice.invoiceNumber());
        query.addBindValue(updatedInvoice.issueDate().toString(Qt::ISODate));
        query.addBindValue(updatedInvoice.subtotal());
        query.addBindValue(updatedInvoice.tax());
        query.addBindValue(updatedInvoice.total());
        query.addBindValue(static_cast<int>(updatedInvoice.paymentMethod()));
        query.addBindValue(updatedInvoice.id());

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        if (query.numRowsAffected() == 0)
        {
            qWarning() << "InvoiceService: No invoice found with ID"
                       << updatedInvoice.id();
            return false;
        }

        return loadInvoices();
    }
    catch (const std::exception& e)
    {
        qCritical() << "InvoiceService::updateInvoice():" << e.what();
        return false;
    }
}

bool InvoiceService::removeInvoice(int id)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "InvoiceService: Database is not connected.";
        return false;
    }

    try
    {
        QSqlQuery query;

        query.prepare(R"(
            DELETE FROM Invoices
            WHERE id = ?
        )");

        query.addBindValue(id);

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        if (query.numRowsAffected() == 0)
        {
            qWarning() << "InvoiceService: No invoice found with ID"
                       << id;
            return false;
        }

        return loadInvoices();
    }
    catch (const std::exception& e)
    {
        qCritical() << "InvoiceService::removeInvoice():" << e.what();
        return false;
    }
}