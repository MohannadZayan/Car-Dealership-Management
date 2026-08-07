#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Database/databasemanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // ! Temporary test
    DatabaseManager db;
    db.connectDatabase();

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("Car_Dealership_Management", "Main");

    return app.exec();
}