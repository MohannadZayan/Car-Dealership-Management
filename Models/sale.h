#ifndef SALE_H
#define SALE_H

#include <QDate>

//* Represents a completed vehicle sale in the dealership.
class Sale
{
private:
    int m_id;

    int m_carId;
    int m_customerId;
    int m_employeeId;

    double m_salePrice;

    QDate m_saleDate;

public:
    //* Constructors & Destructor
    Sale();

    ~Sale() = default;

    //? Used when creating a new sale before saving it to the database.
    Sale(
        int carId,
        int customerId,
        int employeeId,
        double salePrice,
        const QDate& saleDate
    );

    //? Used when constructing a sale object from an existing database record.
    Sale(
        int id,
        int carId,
        int customerId,
        int employeeId,
        double salePrice,
        const QDate& saleDate
    );

    //* Getters
    int id() const;

    int carId() const;
    int customerId() const;
    int employeeId() const;

    double salePrice() const;

    const QDate& saleDate() const;

    //* Setters
    //! IDs are immutable after creation.

    bool setSalePrice(double salePrice);

    bool setSaleDate(const QDate& saleDate);
};

#endif // SALE_H