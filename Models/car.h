#ifndef CAR_H
#define CAR_H

#include <QString>

// ? Represents the current availability status of a car.
enum class CarStatus {
    Available,
    Reserved,
    Sold,
    InTransit
};

// ? Represents the transmission type of a car.
enum class TransmissionType {
    Manual,
    Automatic
};

// * Represents a single vehicle in the dealership inventory.
class Car
{
    // * Private fields
private:
    int m_id;

    QString m_make;
    QString m_model;

    int m_year;

    double m_price;
    int m_mileage;

    QString m_color;

    QString m_engineName;
    int m_horsepower;

    TransmissionType m_transmission;

    bool m_isNew;

    QString m_licensePlate;
    QString m_vin;

    CarStatus m_status;

    // * Public members
public:
    // * Constructors & Destructor
    Car();

    Car(
        const QString& make,
        const QString& model,
        int year,
        double price,
        int mileage,
        const QString& color,
        const QString& engineName,
        int horsepower,
        TransmissionType transmission,
        bool isNew,
        const QString& licensePlate,
        const QString& vin,
        CarStatus status
    );

    //* Used when constructing a car object from a database record.
Car(int id,
    const QString& make,
    const QString& model,
    int year,
    double price,
    int mileage,
    const QString& color,
    const QString& engineName,
    int horsepower,
    TransmissionType transmission,
    bool isNew,
    const QString& licensePlate,
    const QString& vin,
    CarStatus status);

    ~Car() = default;

    // * Getters
    int id() const;
    const QString& make() const;
    const QString& model() const;
    int year() const;
    double price() const;
    int mileage() const;
    const QString& color() const;
    const QString& engineName() const;
    int horsepower() const;
    TransmissionType transmission() const;
    bool isNew() const;
    const QString& licensePlate() const;
    const QString& vin() const;
    CarStatus status() const;

    // * Setters
    bool setPrice(double price);
    bool setMileage(int mileage);
    void setStatus(CarStatus status);
    void setLicensePlate(const QString& licensePlate);
};

#endif // CAR_H