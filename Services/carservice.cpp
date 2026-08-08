#include "carservice.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <exception>

CarService::CarService(DatabaseManager* databaseManager)
    : m_databaseManager(databaseManager)
{
}

const QList<Car>& CarService::cars() const
{
    return m_cars;
}

Car* CarService::findCarById(int id)
{
    for (Car& car : m_cars)
    {
        if (car.id() == id)
        {
            return &car;
        }
    }

    return nullptr;
}

bool CarService::addCar(const Car& car)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "CarService: Database is not connected.";
        return false;
    }

    try
    {
        QSqlQuery query;

        query.prepare(R"(
            INSERT INTO Cars
            (
                make,
                model,
                year,
                price,
                mileage,
                color,
                engine_name,
                horsepower,
                transmission,
                is_new,
                license_plate,
                vin,
                status
            )
            VALUES
            (
                ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?
            )
        )");

        query.addBindValue(car.make());
        query.addBindValue(car.model());
        query.addBindValue(car.year());
        query.addBindValue(car.price());
        query.addBindValue(car.mileage());
        query.addBindValue(car.color());
        query.addBindValue(car.engineName());
        query.addBindValue(car.horsepower());
        query.addBindValue(static_cast<int>(car.transmission()));
        query.addBindValue(car.isNew());
        query.addBindValue(car.licensePlate());
        query.addBindValue(car.vin());
        query.addBindValue(static_cast<int>(car.status()));

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        //* Reload the cache so every Car has the database-generated ID.
        return loadCars();
    }
    catch (const std::exception& e)
    {
        qCritical() << "CarService::addCar():" << e.what();
        return false;
    }
}

bool CarService::loadCars()
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "CarService: Database is not connected.";
        return false;
    }

    try
    {
        QSqlQuery query;

        query.prepare(R"(
            SELECT
                id,
                make,
                model,
                year,
                price,
                mileage,
                color,
                engine_name,
                horsepower,
                transmission,
                is_new,
                license_plate,
                vin,
                status
            FROM Cars;
        )");

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        m_cars.clear();

        while (query.next())
        {
            Car car(
                query.value("id").toInt(),
                query.value("make").toString(),
                query.value("model").toString(),
                query.value("year").toInt(),
                query.value("price").toDouble(),
                query.value("mileage").toInt(),
                query.value("color").toString(),
                query.value("engine_name").toString(),
                query.value("horsepower").toInt(),
                static_cast<TransmissionType>(query.value("transmission").toInt()),
                query.value("is_new").toBool(),
                query.value("license_plate").toString(),
                query.value("vin").toString(),
                static_cast<CarStatus>(query.value("status").toInt())
            );

            m_cars.append(car);
        }

        return true;
    }
    catch (const std::exception& e)
    {
        qCritical() << "CarService::loadCars():" << e.what();
        return false;
    }
}

bool CarService::updateCar(const Car& updatedCar)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "CarService: Database is not connected.";
        return false;
    }

    try
    {
        QSqlQuery query;

        query.prepare(R"(
            UPDATE Cars
            SET
                make = ?,
                model = ?,
                year = ?,
                price = ?,
                mileage = ?,
                color = ?,
                engine_name = ?,
                horsepower = ?,
                transmission = ?,
                is_new = ?,
                license_plate = ?,
                vin = ?,
                status = ?
            WHERE id = ?
        )");

        query.addBindValue(updatedCar.make());
        query.addBindValue(updatedCar.model());
        query.addBindValue(updatedCar.year());
        query.addBindValue(updatedCar.price());
        query.addBindValue(updatedCar.mileage());
        query.addBindValue(updatedCar.color());
        query.addBindValue(updatedCar.engineName());
        query.addBindValue(updatedCar.horsepower());
        query.addBindValue(static_cast<int>(updatedCar.transmission()));
        query.addBindValue(updatedCar.isNew());
        query.addBindValue(updatedCar.licensePlate());
        query.addBindValue(updatedCar.vin());
        query.addBindValue(static_cast<int>(updatedCar.status()));

        // The WHERE id = ? placeholder
        query.addBindValue(updatedCar.id());

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        return loadCars();
    }
    catch (const std::exception& e)
    {
        qCritical() << "CarService::updateCar():" << e.what();
        return false;
    }
}

bool CarService::removeCar(int id)
{
    if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
    {
        qWarning() << "CarService: Database is not connected.";
        return false;
    }

    try
    {
        QSqlQuery query;

        query.prepare(R"(
            DELETE FROM Cars
            WHERE id = ?
        )");

        query.addBindValue(id);

        if (!m_databaseManager->executeQuery(query))
        {
            return false;
        }

        // Make sure a row was actually deleted.
        if (query.numRowsAffected() == 0)
        {
            qWarning() << "CarService: No car found with ID" << id;
            return false;
        }

        return loadCars();
    }
    catch (const std::exception& e)
    {
        qCritical() << "CarService::removeCar():" << e.what();
        return false;
    }
}