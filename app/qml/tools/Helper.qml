import QtQuick 2.0


Item {
    function getCategoryIcon(categories, severity) {
        var suffix

        switch (severity) {
        case 1:
            suffix = "-minor"
            break;

        case 2:
            suffix = "-moderate"
            break;

        case 3:
            suffix = "-severe"
            break;

        default:
            suffix = ""
            break;
        }


        // retrun icons
        if (categories.indexOf("Fire") >= 0)
            return "/usr/share/harbour-apocalypse/icons/categories/category-fire" + suffix + ".svg"

        if (categories.indexOf("Health") >= 0)
            return "/usr/share/harbour-apocalypse/icons/categories/category-health" + suffix + ".svg"

        if (categories.indexOf("Met") >= 0)
            return "/usr/share/harbour-apocalypse/icons/categories/category-meteorology" + suffix + ".svg"

        if (categories.indexOf("Security") >= 0)
            return "/usr/share/harbour-apocalypse/icons/categories/category-security" + suffix + ".svg"

        if (categories.indexOf("Safety") >= 0)
            return "/usr/share/harbour-apocalypse/icons/categories/category-safety" + suffix + ".svg"

        return "/usr/share/harbour-apocalypse/icons/categories/category-info" + suffix + ".svg"
    }

    function getSeverityColor(severity) {
        var color

        switch (severity) {
        case 1: // Minor
            color = "#ffea00"
            break;

        case 2: // Moderate
            color = "#ff9100"
            break;

        case 3: // Severe
            color = "#ff3d00"
            break;

        default:
            color = "#00000000"
            break;
        }

        return color
    }
}
