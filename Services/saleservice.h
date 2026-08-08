#ifndef SALESERVICE_H
#define SALESERVICE_H

#include <QList>

#include "Models/sale.h"
#include "Database/databasemanager.h"

class SaleService
{
private:
    QList<Sale> m_sales;
    DatabaseManager* m_databaseManager;

public:
    explicit SaleService(DatabaseManager* databaseManager);
    ~SaleService() = default;

    //* Sale Management
    bool addSale(const Sale& sale);
    bool updateSale(const Sale& updatedSale);
    bool removeSale(int id);

    //* Database
    bool loadSales();

    //* Retrieval
    const QList<Sale>& sales() const;

    //? Returns a pointer to the sale with the specified ID,
    //? or nullptr if no sale with that ID exists.
    Sale* findSaleById(int id);
};

#endif // SALESERVICE_H