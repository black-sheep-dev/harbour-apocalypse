import QtQuick 2.0

import org.nubecula.harbour.apocalypse 1.0

Item {
    function getCategoryIcon(categories) {
        if ((categories & Message.CategoryFire) === Message.CategoryFire)
            return "qrc:/icons/category-fire"

        if ((categories & Message.CategoryHealth) === Message.CategoryHealth)
            return "qrc:/icons/category-health"

        if ((categories & Message.CategoryMet) === Message.CategoryMet)
            return "qrc:/icons/category-meteorology"

        if ((categories & Message.CategorySecurity) === Message.CategorySecurity)
            return "qrc:/icons/category-security"

        if ((categories & Message.CategorySafety) === Message.CategorySafety)
            return "qrc:/icons/category-safety"

        return "qrc:/icons/category-info"
    }

    function getSeverityColor(severity) {
        var color

        switch (severity) {
        case Message.SeverityMinor:
            color = "#ffff00"
            break;

        case Message.SeverityModerate:
            color = "#ff6d00"
            break;

        case Message.SeveritySevere:
            color = "#d50000"
            break;

        default:
            color = "#00000000"
            break;
        }

        return color
    }
}
