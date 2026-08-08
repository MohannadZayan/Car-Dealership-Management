#include "car.h"
#include "carfiltercriteria.h"

#include <stdexcept>

// * Default constructor
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
      m_bodyType(VehicleBodyType::Sedan),
      m_isNew(false),
      m_licensePlate(""),
      m_vin(""),
      m_status(CarStatus::Available)
{
}

// * Parameterized constructor
Car::Car(
    const QString& make,
    const QString& model,
    int year,
    double price,
    int mileage,
    const QString& color,
    const QString& engineName,
    int horsepower,
    TransmissionType transmission,
    VehicleBodyType bodyType,
    bool isNew
)
    : m_id(0),
      m_make(make),
      m_model(model),
      m_year(year),
      m_price(price),
      m_mileage(mileage),
      m_color(color),
      m_engineName(engineName),
      m_horsepower(horsepower),
      m_transmission(transmission),
      m_bodyType(bodyType),
      m_isNew(isNew),
      m_licensePlate(""),
      m_vin(""),
      m_status(CarStatus::Available)
{
    // ! First layer of validation

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

// * Database constructor
Car::Car(
    int id,
    const QString& make,
    const QString& model,
    int year,
    double price,
    int mileage,
    const QString& color,
    const QString& engineName,
    int horsepower,
    TransmissionType transmission,
    VehicleBodyType bodyType,
    bool isNew,
    const QString& licensePlate,
    const QString& vin,
    CarStatus status
)
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
      m_bodyType(bodyType),
      m_isNew(isNew),
      m_licensePlate(licensePlate),
      m_vin(vin),
      m_status(status)
{
    // ! First layer of validation

    if (id <= 0)
        throw std::invalid_argument("ID must be a positive database identifier");

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

// * Getters implementation

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

VehicleBodyType Car::bodyType() const
{
    return m_bodyType;
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

// * Filtering implementation

bool Car::matches(const CarFilterCriteria& criteria) const
{
    if (criteria.make && m_make.compare(*criteria.make, Qt::CaseInsensitive) != 0)
        return false;

    if (criteria.model && m_model.compare(*criteria.model, Qt::CaseInsensitive) != 0)
        return false;

    if (criteria.minYear && m_year < *criteria.minYear)
        return false;

    if (criteria.maxYear && m_year > *criteria.maxYear)
        return false;

    if (criteria.minPrice && m_price < *criteria.minPrice)
        return false;

    if (criteria.maxPrice && m_price > *criteria.maxPrice)
        return false;

    if (criteria.maxMileage && m_mileage > *criteria.maxMileage)
        return false;

    if (criteria.bodyType && m_bodyType != *criteria.bodyType)
        return false;

    if (criteria.transmission && m_transmission != *criteria.transmission)
        return false;

    if (criteria.isNew && m_isNew != *criteria.isNew)
        return false;

    if (criteria.status && m_status != *criteria.status)
        return false;

    return true;
}

// * Setters implementation

bool Car::setPrice(double price)
{
    // ! Second layer of price validation

    if (price < 0.0)
        return false;

    m_price = price;
    return true;
}

bool Car::setMileage(int mileage)
{
    // ! Second layer of mileage validation

    if (mileage < 0)
        return false;

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