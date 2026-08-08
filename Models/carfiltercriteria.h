#ifndef CARFILTERCRITERIA_H
#define CARFILTERCRITERIA_H

#include <optional>

#include <QString>

#include "car.h"

// * Set only the fields you want to filter on; unset fields are ignored.
struct CarFilterCriteria
{
    std::optional<QString> make;
    std::optional<QString> model;

    std::optional<int> minYear;
    std::optional<int> maxYear;

    std::optional<double> minPrice;
    std::optional<double> maxPrice;

    std::optional<int> maxMileage;

    std::optional<VehicleBodyType> bodyType;
    std::optional<TransmissionType> transmission;

    std::optional<bool> isNew;
    std::optional<CarStatus> status;
};

#endif // CARFILTERCRITERIA_H
