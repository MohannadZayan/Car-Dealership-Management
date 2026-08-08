#include "customerservice.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <exception>

CustomerService::CustomerService(DatabaseManager* databaseManager)
    : BaseService<Customer>(databaseManager)
{

}

const QList<Customer>& CustomerService::customers() const
{
    return entities();
}

Customer* CustomerService::findCustomerById(int id)
{
    return find([id](const Customer& customer) { return customer.id() == id; });
}

Customer* CustomerService::findCustomerByPhone(const QString& phone)
{
    return find([&phone](const Customer& customer) {
        return customer.phone().compare(phone, Qt::CaseInsensitive) == 0;
    });
}

Customer* CustomerService::findCustomerByEmail(const QString& email)
{
    return find([&email](const Customer& customer) {
        return customer.email().compare(email, Qt::CaseInsensitive) == 0;
    });
}

bool CustomerService::addCustomer(const Customer& customer)
{
    return guardedExecute("CustomerService", "addCustomer", [&]() -> bool
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
    });
}

bool CustomerService::loadCustomers()
{
    return guardedExecute("CustomerService", "loadCustomers", [&]() -> bool
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

        m_entities.clear();

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

            m_entities.append(customer);
        }

        return true;
    });
}

bool CustomerService::updateCustomer(const Customer& updatedCustomer)
{
    return guardedExecute("CustomerService", "updateCustomer", [&]() -> bool
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
            m_lastError = ServiceError::NotFound;
            qWarning() << "CustomerService: No customer found with ID"
                       << updatedCustomer.id();
            return false;
        }

        return loadCustomers();
    });
}

bool CustomerService::removeCustomer(int id)
{
    return guardedExecute("CustomerService", "removeCustomer", [&]() -> bool
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
            m_lastError = ServiceError::NotFound;
            qWarning() << "CustomerService: No customer found with ID"
                       << id;
            return false;
        }

        return loadCustomers();
    });
}
