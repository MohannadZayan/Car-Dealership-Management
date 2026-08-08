#ifndef CUSTOMERSERVICE_H
#define CUSTOMERSERVICE_H

#include <QList>

#include "Models/customer.h"
#include "Database/databasemanager.h"

class CustomerService
{
private:
    QList<Customer> m_customers;
    DatabaseManager* m_databaseManager;

public:
    explicit CustomerService(DatabaseManager* databaseManager);
    ~CustomerService() = default;

    //* Customer Management
    bool addCustomer(const Customer& customer);
    bool updateCustomer(const Customer& updatedCustomer);
    bool removeCustomer(int id);

    //* Database
    bool loadCustomers();

    //* Retrieval
    const QList<Customer>& customers() const;

    //? Returns a pointer to the customer with the specified ID,
    //? or nullptr if no customer with that ID exists.
    Customer* findCustomerById(int id);
};

#endif // CUSTOMERSERVICE_H