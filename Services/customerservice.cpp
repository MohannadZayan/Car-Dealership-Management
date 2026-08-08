#include "customerservice.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <exception>

CustomerService::CustomerService(DatabaseManager* databaseManager) : m_databaseManager (databaseManager) 
{

}

const QList<Customer>& CustomerService::customers() const
{
    return m_customers;
}

Customer* CustomerService::findCustomerById(int id)
{
    for (Customer& customer : m_customers)
    {
        if (customer.id() == id)
        {
            return &customer;
        }
    }

    return nullptr;
}
bool CustomerService::addCustomer(const Customer& customer)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "CustomerService: Database is not connected.";
        return false;
    }

    try
    {
        QSqlQuery query;

        query.prepare(R"(
            INSERT INTO Customers
            (
                first_name,
                last_name,
                phone,
                email,
                address
            )
            VALUES
            (
                ?, ?, ?, ?, ?
            )
        )");

        query.addBindValue(customer.firstName());
        query.addBindValue(customer.lastName());
        query.addBindValue(customer.phone());
        query.addBindValue(customer.email());
        query.addBindValue(customer.address());

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        //? Reload the cache so every Customer has the correct database-generated ID.
        return loadCustomers();
    }
    catch (const std::exception& e)
    {
        qCritical() << "CustomerService::addCustomer():" << e.what();
        return false;
    }
}

bool CustomerService::loadCustomers()
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "CustomerService: Database is not connected.";
        return false;
    }

    try
    {
        QSqlQuery query;

        query.prepare(R"(
            SELECT
                id,
                first_name,
                last_name,
                phone,
                email,
                address
            FROM Customers;
        )");

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        m_customers.clear();

        while (query.next())
        {
            Customer customer(
                query.value("id").toInt(),
                query.value("first_name").toString(),
                query.value("last_name").toString(),
                query.value("phone").toString(),
                query.value("email").toString(),
                query.value("address").toString()
            );

            m_customers.append(customer);
        }

        return true;
    }
    catch (const std::exception& e)
    {
        qCritical() << "CustomerService::loadCustomers():" << e.what();
        return false;
    }
}

bool CustomerService::updateCustomer(const Customer& updatedCustomer)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "CustomerService: Database is not connected.";
        return false;
    }

    try
    {
        QSqlQuery query;

        query.prepare(R"(
            UPDATE Customers
            SET
                first_name = ?,
                last_name = ?,
                phone = ?,
                email = ?,
                address = ?
            WHERE id = ?
        )");

        query.addBindValue(updatedCustomer.firstName());
        query.addBindValue(updatedCustomer.lastName());
        query.addBindValue(updatedCustomer.phone());
        query.addBindValue(updatedCustomer.email());
        query.addBindValue(updatedCustomer.address());

        // WHERE id = ?
        query.addBindValue(updatedCustomer.id());

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        // Ensure a customer with this ID actually existed.
        if (query.numRowsAffected() == 0)
        {
            qWarning() << "CustomerService: No customer found with ID"
                       << updatedCustomer.id();
            return false;
        }

        return loadCustomers();
    }
    catch (const std::exception& e)
    {
        qCritical() << "CustomerService::updateCustomer():" << e.what();
        return false;
    }
}

bool CustomerService::removeCustomer(int id)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "CustomerService: Database is not connected.";
        return false;
    }

    try
    {
        QSqlQuery query;

        query.prepare(R"(
            DELETE FROM Customers
            WHERE id = ?
        )");

        query.addBindValue(id);

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        // Ensure a customer with this ID actually existed.
        if (query.numRowsAffected() == 0)
        {
            qWarning() << "CustomerService: No customer found with ID"
                       << id;
            return false;
        }

        return loadCustomers();
    }
    catch (const std::exception& e)
    {
        qCritical() << "CustomerService::removeCustomer():" << e.what();
        return false;
    }
}
