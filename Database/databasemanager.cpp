#include "databasemanager.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject{parent}
{

}

bool DatabaseManager::connectDatabase()
{
    // Establish a connection to the database
    m_database = QSqlDatabase::addDatabase("QSQLITE");

    // Store the database in the app's standard data location rather than
    // wherever the process happens to be launched from, so it's always
    // the same file regardless of working directory.
    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataDir);

    const QString dbPath = dataDir + QDir::separator() + "dealership.db";

    // First run on this machine: seed from the snapshot shipped inside the
    // binary so the app isn't empty out of the box. Never overwrites an
    // existing database.
    if (!QFile::exists(dbPath) && QFile::exists(":/seed/dealership.db"))
    {
        if (QFile::copy(":/seed/dealership.db", dbPath))
            QFile::setPermissions(dbPath, QFile::ReadOwner | QFile::WriteOwner);
        else
            qDebug() << "Failed to seed database from bundled snapshot.";
    }

    m_database.setDatabaseName(dbPath);

    // Open the database connection
    if (!m_database.open())
    {
        qDebug() << "Failed to open database connection:"
                 << m_database.lastError().text();
        return false;
    }

    // Enable foreign key constraints
    QSqlQuery pragmaQuery;
    if (!pragmaQuery.exec("PRAGMA foreign_keys = ON;"))
    {
        qDebug() << "Failed to enable foreign keys:"
                 << pragmaQuery.lastError().text();
        return false;
    }

    qDebug() << "Database connection established!";

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

bool DatabaseManager::isConnected() const
{
    return m_database.isOpen();
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

    // CREATE TABLE IF NOT EXISTS only shapes a brand-new Cars table; a database created
    // before body_type existed needs this to actually gain the column.
    if (!ensureColumnExists("Cars", "body_type", "INTEGER NOT NULL DEFAULT 0"))
        return false;

    qDebug() << "Creating Customers table...";
    if (!createCustomersTable())
        return false;

    qDebug() << "Creating Employees table...";
    if (!createEmployeesTable())
        return false;

    // Same reasoning as the Cars/body_type migration above — brings an existing
    // Employees table up to date with the columns authentication needs.
    if (!ensureColumnExists("Employees", "password_hash", "TEXT NOT NULL DEFAULT ''"))
        return false;

    if (!ensureColumnExists("Employees", "password_salt", "TEXT NOT NULL DEFAULT ''"))
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

bool DatabaseManager::ensureColumnExists(const QString& tableName, const QString& columnName, const QString& columnDefinition)
{
    QSqlQuery pragmaQuery;

    if (!pragmaQuery.exec(QString("PRAGMA table_info(%1);").arg(tableName)))
    {
        qDebug() << "Failed to inspect table" << tableName << ":" << pragmaQuery.lastError().text();
        return false;
    }

    while (pragmaQuery.next())
    {
        if (pragmaQuery.value("name").toString().compare(columnName, Qt::CaseInsensitive) == 0)
        {
            return true; // Column already present, nothing to migrate.
        }
    }

    QSqlQuery alterQuery;

    if (!alterQuery.exec(QString("ALTER TABLE %1 ADD COLUMN %2 %3;").arg(tableName, columnName, columnDefinition)))
    {
        qDebug() << "Failed to add column" << columnName << "to" << tableName << ":" << alterQuery.lastError().text();
        return false;
    }

    qDebug() << "Migrated table" << tableName << ": added column" << columnName;
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
            body_type INTEGER NOT NULL,

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

            role INTEGER NOT NULL,

            salary REAL NOT NULL CHECK(salary >= 0),

            phone TEXT,
            email TEXT UNIQUE,

            password_hash TEXT NOT NULL DEFAULT '',
            password_salt TEXT NOT NULL DEFAULT ''
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

            payment_method INTEGER NOT NULL,

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