#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <Database/databasemanager.h>

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

    private :
    QSqlDatabase m_database; //? This member variable holds the database connection object.

signals:
};

#endif // DATABASEMANAGER_H
