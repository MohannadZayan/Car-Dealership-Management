#ifndef INVOICESERVICE_H
#define INVOICESERVICE_H

#include <QList>

#include "Models/Invoice.h"
#include "Database/databasemanager.h"

class InvoiceService
{
private:
    QList<Invoice> m_invoices;
    DatabaseManager* m_databaseManager;

public:
    explicit InvoiceService(DatabaseManager* databaseManager);
    ~InvoiceService() = default;

    //* Invoice Management
    bool addInvoice(const Invoice& invoice);
    bool updateInvoice(const Invoice& updatedInvoice);
    bool removeInvoice(int id);

    //* Database
    bool loadInvoices();

    //* Retrieval
    const QList<Invoice>& invoices() const;

    //? Returns a pointer to the invoice with the specified ID,
    //? or nullptr if no invoice with that ID exists.
    Invoice* findInvoiceById(int id);
};

#endif // INVOICESERVICE_H