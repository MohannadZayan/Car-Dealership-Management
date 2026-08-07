#include "databasemanager.h"

#include <QDebug>
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