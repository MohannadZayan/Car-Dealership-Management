#ifndef BASESERVICE_H
#define BASESERVICE_H

#include <functional>

#include <QList>

#include "Database/databasemanager.h"

// * Shared cache + lookup/filter logic for every *Service class.
// ! CRUD methods (add/update/remove/load) stay in each derived service —
// ! only add/update/remove/load's own SQL is per-table, everything below is not.
template <typename T>
class BaseService
{
protected:
    QList<T> m_entities;
    DatabaseManager* m_databaseManager;

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
};

#endif // BASESERVICE_H
