#ifndef SALESERVICE_H
#define SALESERVICE_H

#include <QList>

#include "Models/sale.h"
#include "Models/salefiltercriteria.h"
#include "Database/databasemanager.h"
#include "Services/baseservice.h"

class SaleService : public BaseService<Sale>
{
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

    //? Returns the sales that match every field set in criteria (e.g. car, customer, employee, price range, date range).
    QList<Sale> filterSales(const SaleFilterCriteria& criteria) const;
};

#endif // SALESERVICE_H