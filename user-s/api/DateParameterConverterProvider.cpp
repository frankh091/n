#include <iostream>
#include <string>
#include <sstream>
#include <optional>
#include <iomanip>
#include <ctime>

class DateUtil {
public:
    static std::optional<std::tm> parseDate(const std::string& value) {
        std::tm tm = {};
        std::istringstream ss(value);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        if (ss.fail()) {
            return std::nullopt;
        }
        return tm;
    }

    static std::string formatDate(const std::tm& value) {
        std::ostringstream ss;
        ss << std::put_time(&value, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
};

class DateParameterConverter {
public:
    std::optional<std::tm> fromString(const std::string& value) {
        return DateUtil::parseDate(value);
    }

    std::string toString(const std::tm& value) {
        return DateUtil::formatDate(value);
    }
};

class DateParameterConverterProvider {
public:
    template <typename T>
    std::optional<DateParameterConverter> getConverter() {
        if (typeid(T) == typeid(std::tm)) {
            return DateParameterConverter();
        }
        return std::nullopt;
    }
};

int main() {
    DateParameterConverterProvider provider;

    // Obtener un convertidor para std::tm
    auto converter = provider.getConverter<std::tm>();
    if (converter) {
        // Convertir cadena a fecha
        auto parsedDate = converter->fromString("2025-01-21 15:30:00");
        if (parsedDate) {
            std::cout << "Parsed Date: " << DateUtil::formatDate(*parsedDate) << std::endl;
        } else {
            std::cerr << "Failed to parse date." << std::endl;
        }

        // Convertir fecha a cadena
        if (parsedDate) {
            std::string formattedDate = converter->toString(*parsedDate);
            std::cout << "Formatted Date: " << formattedDate << std::endl;
        }
    } else {
        std::cerr << "No converter available for the given type." << std::endl;
    }

    return 0;
}
