#include "databasemanager.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject{parent}
{

}

bool DatabaseManager::connectDatabase() {
    //? Establish a connection to the database
    // ! addDatabase() is a static method, returns a QSqlDatabase object
    m_database = QSqlDatabase::addDatabase("QSQLITE");

    // ? Tell the database which file to use
    // * Automatically creates the file if it doesn't exist
    m_database.setDatabaseName("dealership.db");

    // * Open the database connection
    if (!m_database.open()) {
        qDebug() << "Failed to open database connection" << m_database.lastError().text();
        return false;
    }
    qDebug () << "Database connection established!";
    if (!createTables()) {
        qDebug() << "Failed to create tables";
        return false;
    }

    return true;
}

bool DatabaseManager::disconnectDatabase() {
    
    // ? Close the database connection
    if (!m_database.isOpen()) {
        qDebug() << "Database connection is already closed!";
        return false;
    }
    m_database.close();
    qDebug() << "Database connection closed!";
    return true;
    
}

bool DatabaseManager::executeQuery(QSqlQuery& query)
{
    if (!m_database.isOpen()) {
        qDebug() << "Database connection is not open!";
        return false;
    }

    if (!query.exec())
    {
        qDebug() << "Database query failed:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::createTables()
{
    qDebug() << "Entered createTables()";

    qDebug() << "Creating Cars table...";
    if (!createCarsTable())
        return false;

    qDebug() << "Creating Customers table...";
    if (!createCustomersTable())
        return false;

    qDebug() << "Creating Employees table...";
    if (!createEmployeesTable())
        return false;

    qDebug() << "Creating VehicleSales table...";
    if (!createVehicleSalesTable())
        return false;

    qDebug() << "Creating Invoices table...";
    if (!createInvoicesTable())
        return false;

    qDebug() << "All tables created successfully.";

    return true;
}


bool DatabaseManager::createCarsTable()
{
    QSqlQuery query;

    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS Cars
        (
            id INTEGER PRIMARY KEY AUTOINCREMENT,

            make TEXT NOT NULL,
            model TEXT NOT NULL,
            year INTEGER NOT NULL CHECK(year >= 1886),

            price REAL NOT NULL CHECK(price >= 0),
            mileage INTEGER NOT NULL CHECK(mileage >= 0),

            color TEXT NOT NULL,

            engine_name TEXT,
            horsepower INTEGER CHECK(horsepower >= 0),

            transmission INTEGER NOT NULL,

            is_new INTEGER NOT NULL,

            license_plate TEXT,
            vin TEXT NOT NULL UNIQUE,

            status INTEGER NOT NULL
        );
    )"))
    {
        qDebug() << "Failed to create Cars table:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::createCustomersTable()
{
    QSqlQuery query;

    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS Customers
        (
            id INTEGER PRIMARY KEY AUTOINCREMENT,

            first_name TEXT NOT NULL,
            last_name TEXT NOT NULL,

            phone TEXT NOT NULL,
            email TEXT UNIQUE,

            address TEXT
        );
    )"))
    {
        qDebug() << "Failed to create Customers table:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::createEmployeesTable()
{
    QSqlQuery query;

    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS Employees
        (
            id INTEGER PRIMARY KEY AUTOINCREMENT,

            first_name TEXT NOT NULL,
            last_name TEXT NOT NULL,

            role TEXT NOT NULL,

            salary REAL NOT NULL CHECK(salary >= 0),

            phone TEXT,
            email TEXT UNIQUE
        );
    )"))
    {
        qDebug() << "Failed to create Employees table:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::createVehicleSalesTable()
{
    QSqlQuery query;

    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS VehicleSales
        (
            id INTEGER PRIMARY KEY AUTOINCREMENT,

            car_id INTEGER NOT NULL,
            customer_id INTEGER NOT NULL,
            employee_id INTEGER NOT NULL,

            sale_price REAL NOT NULL CHECK(sale_price >= 0),

            sale_date TEXT NOT NULL,

            FOREIGN KEY(car_id) REFERENCES Cars(id),
            FOREIGN KEY(customer_id) REFERENCES Customers(id),
            FOREIGN KEY(employee_id) REFERENCES Employees(id)
        );
    )"))
    {
        qDebug() << "Failed to create VehicleSales table:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::createInvoicesTable()
{
    QSqlQuery query;

    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS Invoices
        (
            id INTEGER PRIMARY KEY AUTOINCREMENT,

            sale_id INTEGER NOT NULL,

            invoice_number TEXT NOT NULL UNIQUE,

            issue_date TEXT NOT NULL,

            subtotal REAL NOT NULL CHECK(subtotal >= 0),
            tax REAL NOT NULL CHECK(tax >= 0),
            total REAL NOT NULL CHECK(total >= 0),

            payment_method TEXT NOT NULL,

            FOREIGN KEY(sale_id) REFERENCES VehicleSales(id)
        );
    )"))
    {
        qDebug() << "Failed to create Invoices table:"
                 << query.lastError().text();
        return false;
    }

    return true;
}