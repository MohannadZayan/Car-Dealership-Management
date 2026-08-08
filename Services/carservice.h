#ifndef CARSERVICE_H
#define CARSERVICE_H
#include <QList>
#include "Models/car.h"
#include "Database/databasemanager.h"

class CarService
{

    private: 
    QList<Car> m_cars;
    DatabaseManager* m_databaseManager;

public:
    explicit CarService(DatabaseManager* databaseManager); //? Explicit for constructor to prevent implicit conversions.
    ~CarService() = default;

    //* Inventory Management
bool addCar(const Car& car); //? This method adds a new car to the inventory.
bool removeCar(int id); // ? This method removes a car from the inventory.
bool updateCar(const Car& updatedCar); //? This method updates a car in the inventory.

//* Database

bool loadCars(); //? This method loads all cars from the database into the inventory.

//* Retrieval
const QList<Car>& cars() const; //? This method returns a const reference to the list of cars in the inventory.
Car* findCarById(int id); //? This method returns a pointer to the car with the specified id, or nullptr if no car with the specified id exists.
};

#endif // CARSERVICE_H
    