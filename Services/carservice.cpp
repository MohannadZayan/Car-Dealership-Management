#include "carservice.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <exception>

CarService::CarService(DatabaseManager* databaseManager)
    : BaseService<Car>(databaseManager)
{
}

const QList<Car>& CarService::cars() const
{
    return entities();
}

Car* CarService::findCarById(int id)
{
    return find([id](const Car& car) { return car.id() == id; });
}

QList<Car> CarService::carsByBodyType(VehicleBodyType bodyType) const
{
    return filter([bodyType](const Car& car) { return car.bodyType() == bodyType; });
}

QList<Car> CarService::filterCars(const CarFilterCriteria& criteria) const
{
    return filter([&criteria](const Car& car) { return car.matches(criteria); });
}

bool CarService::addCar(const Car& car)
{
    return guardedExecute("CarService", "addCar", [&]() -> bool
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
                body_type,
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
                ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?
            )
        )");

        query.addBindValue(car.make());
        query.addBindValue(car.model());
        query.addBindValue(car.year());
        query.addBindValue(car.price());
        query.addBindValue(car.mileage());
        query.addBindValue(car.color());
        query.addBindValue(static_cast<int>(car.bodyType()));
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
    });
}

bool CarService::loadCars()
{
    return guardedExecute("CarService", "loadCars", [&]() -> bool
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
                body_type,
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

        m_entities.clear();

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
                static_cast<VehicleBodyType>(query.value("body_type").toInt()),
                query.value("is_new").toBool(),
                query.value("license_plate").toString(),
                query.value("vin").toString(),
                static_cast<CarStatus>(query.value("status").toInt())
            );

            m_entities.append(car);
        }

        return true;
    });
}

bool CarService::updateCar(const Car& updatedCar)
{
    return guardedExecute("CarService", "updateCar", [&]() -> bool
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
                body_type = ?,
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
        query.addBindValue(static_cast<int>(updatedCar.bodyType()));
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

        // Make sure a row was actually updated.
        if (query.numRowsAffected() == 0)
        {
            m_lastError = ServiceError::NotFound;
            qWarning() << "CarService: No car found with ID" << updatedCar.id();
            return false;
        }

        return loadCars();
    });
}

bool CarService::removeCar(int id)
{
    return guardedExecute("CarService", "removeCar", [&]() -> bool
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
            m_lastError = ServiceError::NotFound;
            qWarning() << "CarService: No car found with ID" << id;
            return false;
        }

        return loadCars();
    });
}
