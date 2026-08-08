#ifndef SALEFILTERCRITERIA_H
#define SALEFILTERCRITERIA_H

#include <optional>

#include <QDate>

// * Set only the fields you want to filter on; unset fields are ignored.
struct SaleFilterCriteria
{
    std::optional<int> carId;
    std::optional<int> customerId;
    std::optional<int> employeeId;

    std::optional<double> minSalePrice;
    std::optional<double> maxSalePrice;

    std::optional<QDate> fromDate;
    std::optional<QDate> toDate;
};

#endif // SALEFILTERCRITERIA_H
