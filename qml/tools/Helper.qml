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
            return "/usr/share/harbour-apocalypse/icons/categories/category-fire" + suffix + ".svg"

        if ((categories & Message.CategoryHealth) === Message.CategoryHealth)
            return "/usr/share/harbour-apocalypse/icons/categories/category-health" + suffix + ".svg"

        if ((categories & Message.CategoryMet) === Message.CategoryMet)
            return "/usr/share/harbour-apocalypse/icons/categories/category-meteorology" + suffix + ".svg"

        if ((categories & Message.CategorySecurity) === Message.CategorySecurity)
            return "/usr/share/harbour-apocalypse/icons/categories/category-security" + suffix + ".svg"

        if ((categories & Message.CategorySafety) === Message.CategorySafety)
            return "/usr/share/harbour-apocalypse/icons/categories/category-safety" + suffix + ".svg"

        return "/usr/share/harbour-apocalypse/icons/categories/category-info" + suffix + ".svg"
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
