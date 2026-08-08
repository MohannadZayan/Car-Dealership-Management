#include "saleservice.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <exception>

SaleService::SaleService(DatabaseManager* databaseManager)
    : m_databaseManager(databaseManager)
{

}

const QList<Sale>& SaleService::sales() const
{
    return m_sales;
}

Sale* SaleService::findSaleById(int id)
{
    for (Sale& sale : m_sales)
    {
        if (sale.id() == id)
        {
            return &sale;
        }
    }

    return nullptr;
}

bool SaleService::addSale(const Sale& sale)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "SaleService: Database is not connected.";
        return false;
    }

    try
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
    }
    catch (const std::exception& e)
    {
        qCritical() << "SaleService::addSale():" << e.what();
        return false;
    }
}

bool SaleService::loadSales()
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "SaleService: Database is not connected.";
        return false;
    }

    try
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

        m_sales.clear();

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

            m_sales.append(sale);
        }

        return true;
    }
    catch (const std::exception& e)
    {
        qCritical() << "SaleService::loadSales():" << e.what();
        return false;
    }
}

bool SaleService::updateSale(const Sale& updatedSale)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "SaleService: Database is not connected.";
        return false;
    }

    try
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
            qWarning() << "SaleService: No sale found with ID"
                       << updatedSale.id();
            return false;
        }

        return loadSales();
    }
    catch (const std::exception& e)
    {
        qCritical() << "SaleService::updateSale():" << e.what();
        return false;
    }
}

bool SaleService::removeSale(int id)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "SaleService: Database is not connected.";
        return false;
    }

    try
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
            qWarning() << "SaleService: No sale found with ID"
                       << id;
            return false;
        }

        return loadSales();
    }
    catch (const std::exception& e)
    {
        qCritical() << "SaleService::removeSale():" << e.what();
        return false;
    }
}