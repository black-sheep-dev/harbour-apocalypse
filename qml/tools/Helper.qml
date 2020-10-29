import QtQuick 2.0

import org.nubecula.harbour.apocalypse 1.0

Item {
    function getCategoryIcon(categories, severity) {
        var suffix

        switch (severity) {
        case Message.SeverityMinor:
            suffix = "-minor"
            break;

        case Message.SeverityModerate:
            suffix = "-moderate"
            break;

        case Message.SeveritySevere:
            suffix = "-severe"
            break;

        default:
            suffix = ""
            break;
        }


        // retrun icons
        if ((categories & Message.CategoryFire) === Message.CategoryFire)
            return "qrc:/icons/category-fire" + suffix

        if ((categories & Message.CategoryHealth) === Message.CategoryHealth)
            return "qrc:/icons/category-health" + suffix

        if ((categories & Message.CategoryMet) === Message.CategoryMet)
            return "qrc:/icons/category-meteorology" + suffix

        if ((categories & Message.CategorySecurity) === Message.CategorySecurity)
            return "qrc:/icons/category-security" + suffix

        if ((categories & Message.CategorySafety) === Message.CategorySafety)
            return "qrc:/icons/category-safety" + suffix

        return "qrc:/icons/category-info" + suffix
    }

    function getSeverityColor(severity) {
        var color

        switch (severity) {
        case Message.SeverityMinor:
            color = "#ffea00"
            break;

        case Message.SeverityModerate:
            color = "#ff9100"
            break;

        case Message.SeveritySevere:
            color = "#ff3d00"
            break;

        default:
            color = "#00000000"
            break;
        }

        return color
    }
}
