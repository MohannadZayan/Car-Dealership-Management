#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QString>
// *Represents a customer of the dealership.
class Customer
{
    private: 
    int m_id;

    QString m_firstName;
    QString m_lastName;

    QString m_phone;
    QString m_email;

    QString m_address;

public:
    Customer();
    ~Customer() = default;

    //? Used when creating a new customer before saving it to the database.
    Customer (
        const QString& firstName,
        const QString& lastName,
        const QString& phone,
        const QString& email,
        const QString& address
    );

    // ? Used when constructing a customer object from an existing database record.
    Customer (
        int id, 
        const QString& firstName,
        const QString& lastName,
        const QString& phone,
        const QString& email,
        const QString& address
    );

    // * Getters
    int id() const;
    const QString& firstName() const;
    const QString& lastName() const;
    const QString& phone() const;
    const QString& email() const;
    const QString& address() const;

    // * Setters
    // ! Notice here we didn't declare setters for the first and last name fields, they are immutable.

    bool setPhone(const QString& phone);
    bool setEmail(const QString& email);
    bool setAddress(const QString& address);
};

#endif // CUSTOMER_H
