#include "saleservice.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <exception>

SaleService::SaleService(DatabaseManager* databaseManager)
    : BaseService<Sale>(databaseManager)
{

}

const QList<Sale>& SaleService::sales() const
{
    return entities();
}

Sale* SaleService::findSaleById(int id)
{
    return find([id](const Sale& sale) { return sale.id() == id; });
}

QList<Sale> SaleService::filterSales(const SaleFilterCriteria& criteria) const
{
    return filter([&criteria](const Sale& sale) { return sale.matches(criteria); });
}

bool SaleService::addSale(const Sale& sale)
{
    return guardedExecute("SaleService", "addSale", [&]() -> bool
    {
        QSqlQuery query;

        query.prepare(R"(
            INSERT INTO VehicleSales
            (
                car_id,
                customer_id,
                employee_id,
                sale_price,
                sale_date
            )
            VALUES
            (
                ?, ?, ?, ?, ?
            )
        )");

        query.addBindValue(sale.carId());
        query.addBindValue(sale.customerId());
        query.addBindValue(sale.employeeId());
        query.addBindValue(sale.salePrice());
        query.addBindValue(sale.saleDate().toString(Qt::ISODate));

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        return loadSales();
    });
}

bool SaleService::loadSales()
{
    return guardedExecute("SaleService", "loadSales", [&]() -> bool
    {
        QSqlQuery query;

        query.prepare(R"(
            SELECT
                id,
                car_id,
                customer_id,
                employee_id,
                sale_price,
                sale_date
            FROM VehicleSales;
        )");

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        m_entities.clear();

        while (query.next())
        {
            Sale sale(
                query.value("id").toInt(),
                query.value("car_id").toInt(),
                query.value("customer_id").toInt(),
                query.value("employee_id").toInt(),
                query.value("sale_price").toDouble(),
                QDate::fromString(
                    query.value("sale_date").toString(),
                    Qt::ISODate
                )
            );

            m_entities.append(sale);
        }

        return true;
    });
}

bool SaleService::updateSale(const Sale& updatedSale)
{
    return guardedExecute("SaleService", "updateSale", [&]() -> bool
    {
        QSqlQuery query;

        query.prepare(R"(
            UPDATE VehicleSales
            SET
                car_id = ?,
                customer_id = ?,
                employee_id = ?,
                sale_price = ?,
                sale_date = ?
            WHERE id = ?
        )");

        query.addBindValue(updatedSale.carId());
        query.addBindValue(updatedSale.customerId());
        query.addBindValue(updatedSale.employeeId());
        query.addBindValue(updatedSale.salePrice());
        query.addBindValue(updatedSale.saleDate().toString(Qt::ISODate));
        query.addBindValue(updatedSale.id());

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        if (query.numRowsAffected() == 0)
        {
            m_lastError = ServiceError::NotFound;
            qWarning() << "SaleService: No sale found with ID"
                       << updatedSale.id();
            return false;
        }

        return loadSales();
    });
}

bool SaleService::removeSale(int id)
{
    return guardedExecute("SaleService", "removeSale", [&]() -> bool
    {
        QSqlQuery query;

        query.prepare(R"(
            DELETE FROM VehicleSales
            WHERE id = ?
        )");

        query.addBindValue(id);

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        if (query.numRowsAffected() == 0)
        {
            m_lastError = ServiceError::NotFound;
            qWarning() << "SaleService: No sale found with ID"
                       << id;
            return false;
        }

        return loadSales();
    });
}
