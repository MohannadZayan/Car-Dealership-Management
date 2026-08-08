#include "car.h"
#include <stdexcept>

 // *Default constructor

Car::Car()
    : m_id(0),
      m_make(""),
      m_model(""),
      m_year(0),
      m_price(0.0),
      m_mileage(0),
      m_color(""),
      m_engineName(""),
      m_horsepower(0),
      m_transmission(TransmissionType::Automatic),
      m_isNew(false),
      m_licensePlate(""),
      m_vin(""),
      m_status(CarStatus::Available)
{
}

// *Parameterized constructor
Car::Car(int id,
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
         CarStatus status)
    : m_id(id),
      m_make(make),
      m_model(model),
      m_year(year),
      m_price(price),
      m_mileage(mileage),
      m_color(color),
      m_engineName(engineName),
      m_horsepower(horsepower),
      m_transmission(transmission),
      m_isNew(isNew),
      m_licensePlate(licensePlate),
      m_vin(vin),
      m_status(status)
{
    if (price < 0.0)
        throw std::invalid_argument("Price cannot be negative");

    if (mileage < 0)
        throw std::invalid_argument("Mileage cannot be negative");

    if (year < 1886 || year > 2100)
        throw std::invalid_argument("Year is out of valid range");

    if (make.trimmed().isEmpty())
        throw std::invalid_argument("Make cannot be empty");

    if (model.trimmed().isEmpty())
        throw std::invalid_argument("Model cannot be empty");

    if (horsepower < 0)
        throw std::invalid_argument("Horsepower cannot be negative");
}

// *Getters implementation
int Car::id() const
{
    return m_id;
}

const QString& Car::make() const
{
    return m_make;
}

const QString& Car::model() const
{
    return m_model;
}

int Car::year() const
{
    return m_year;
}

double Car::price() const
{
    return m_price;
}

int Car::mileage() const
{
    return m_mileage;
}

const QString& Car::color() const
{
    return m_color;
}

const QString& Car::engineName() const
{
    return m_engineName;
}

int Car::horsepower() const
{
    return m_horsepower;
}

TransmissionType Car::transmission() const
{
    return m_transmission;
}

bool Car::isNew() const
{
    return m_isNew;
}

const QString& Car::licensePlate() const
{
    return m_licensePlate;
}

const QString& Car::vin() const
{
    return m_vin;
}

CarStatus Car::status() const
{
    return m_status;
}

// *Setters implementation
bool Car::setPrice(double price)
{
    // ! Second layer of price validation
    if (price < 0.0) {
        return false;
    }
    m_price = price;
    return true;
}

bool Car::setMileage(int mileage)
{
    // ! Second layer of mileage validation
    if (mileage < 0) {
        return false;
    }
    m_mileage = mileage;
    return true;
}

void Car::setStatus(CarStatus status)
{
    m_status = status;
}

void Car::setLicensePlate(const QString& licensePlate)
{
    m_licensePlate = licensePlate;
}
