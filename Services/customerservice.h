#ifndef CUSTOMERSERVICE_H
#define CUSTOMERSERVICE_H

#include <QList>

#include "Models/customer.h"
#include "Database/databasemanager.h"
#include "Services/baseservice.h"

class CustomerService : public BaseService<Customer>
{
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

    //? Returns a pointer to the customer with the specified phone number,
    //? or nullptr if no customer with that phone number exists.
    Customer* findCustomerByPhone(const QString& phone);

    //? Returns a pointer to the customer with the specified email address,
    //? or nullptr if no customer with that email address exists.
    Customer* findCustomerByEmail(const QString& email);
};

#endif // CUSTOMERSERVICE_H