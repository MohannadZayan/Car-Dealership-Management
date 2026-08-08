#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    bool connectDatabase(); //? This method establishes a connection to the database and returns true if successful, false otherwise.

    bool disconnectDatabase(); //? This method closes the database connection.

    bool isConnected() const; /* 
    ? Returns whether the database is currently connected.*/
    // ! This function is const because it does not modify the DatabaseManager.  
    
    bool executeQuery ( QSqlQuery& query); //? This method executes a query on the database and returns true if successful, false otherwise.

    bool createTables(); //? This method creates the tables in the database if they do not already exist.

    private :
    QSqlDatabase m_database; //? This member variable holds the database connection object.

    bool createCarsTable(); //? This method creates the cars table in the database if it does not already exist.
    bool createCustomersTable(); //? This method creates the customers table in the database if it does not already exist.
    bool createEmployeesTable(); //? This method creates the employees table in the database if it does not already exist.
    bool createVehicleSalesTable(); //? This method creates the vehicle_sales table in the database if it does not already exist.
    bool createInvoicesTable(); //? This method creates the invoices table in the database if it does not already exist.

signals:
};

#endif // DATABASEMANAGER_H
