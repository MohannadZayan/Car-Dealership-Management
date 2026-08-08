#ifndef BASESERVICE_H
#define BASESERVICE_H

#include <exception>
#include <functional>

#include <QDebug>
#include <QList>
#include <QString>

#include "Database/databasemanager.h"

// ? Why a CRUD call returned false — inspect via lastError() after a false return.
enum class ServiceError
{
    None,             //? The most recent call succeeded.
    NotConnected,     //? No database connection was available.
    NotFound,         //? The targeted row (by id) doesn't exist.
    ValidationFailed, //? Constructing/validating the entity threw (e.g. bad DB row data).
    DatabaseError,    //? The query itself failed (bad SQL, constraint violation, etc.).
    DuplicateEmail    //? Another record already uses that email address.
};

// * Shared cache + lookup/filter logic for every *Service class.
// ! CRUD methods (add/update/remove/load) stay in each derived service —
// ! only add/update/remove/load's own SQL is per-table, everything below is not.
template <typename T>
class BaseService
{
protected:
    QList<T> m_entities;
    DatabaseManager* m_databaseManager;
    ServiceError m_lastError = ServiceError::None;

public:
    explicit BaseService(DatabaseManager* databaseManager)
        : m_databaseManager(databaseManager)
    {
    }

    virtual ~BaseService() = default;

    const QList<T>& entities() const
    {
        return m_entities;
    }

    //? Why the most recent add/update/remove/load call returned false.
    ServiceError lastError() const
    {
        return m_lastError;
    }

    //? Human-readable form of lastError(), suitable for showing to a user.
    QString lastErrorString() const
    {
        switch (m_lastError)
        {
        case ServiceError::None:
            return QString();
        case ServiceError::NotConnected:
            return "Database is not connected.";
        case ServiceError::NotFound:
            return "No matching record was found.";
        case ServiceError::ValidationFailed:
            return "The record failed validation.";
        case ServiceError::DatabaseError:
            return "The database operation failed.";
        case ServiceError::DuplicateEmail:
            return "That email address is already in use.";
        }

        return QString();
    }

    //? Returns a pointer to the first entity satisfying predicate, or nullptr if none does.
    T* find(const std::function<bool(const T&)>& predicate)
    {
        for (T& entity : m_entities)
        {
            if (predicate(entity))
            {
                return &entity;
            }
        }

        return nullptr;
    }

    //? Returns every entity satisfying predicate.
    QList<T> filter(const std::function<bool(const T&)>& predicate) const
    {
        QList<T> result;

        for (const T& entity : m_entities)
        {
            if (predicate(entity))
            {
                result.append(entity);
            }
        }

        return result;
    }

protected:
    //? Runs action only if the database is connected, and turns any exception it throws
    //? into a logged failure — the shared scaffolding every add/update/remove/load method needs.
    //? action() may set m_lastError itself (e.g. to NotFound) before returning false;
    //? if it doesn't, a false result is attributed to a generic DatabaseError.
    template <typename Func>
    bool guardedExecute(const char* serviceName, const char* operationName, Func&& action)
    {
        m_lastError = ServiceError::None;

        if (m_databaseManager == nullptr || !m_databaseManager->isConnected())
        {
            m_lastError = ServiceError::NotConnected;
            qWarning("%s: Database is not connected.", serviceName);
            return false;
        }

        try
        {
            bool succeeded = action();

            if (!succeeded && m_lastError == ServiceError::None)
            {
                m_lastError = ServiceError::DatabaseError;
            }

            return succeeded;
        }
        catch (const std::exception& e)
        {
            m_lastError = ServiceError::ValidationFailed;
            qCritical("%s::%s(): %s", serviceName, operationName, e.what());
            return false;
        }
    }
};

#endif // BASESERVICE_H
