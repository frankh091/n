#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <ctime>
#include <algorithm>

class Logger {
public:
    void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }

    void error(const std::string& message) {
        std::cerr << "ERROR: " << message << std::endl;
    }
};

struct Statistics {
    std::time_t captureTime;
    std::string data;

    Statistics(std::time_t time, const std::string& data) : captureTime(time), data(data) {}
};

class PermissionsService {
public:
    void checkAdmin(long userId) {
        if (userId != 1) {
            throw std::runtime_error("User is not an administrator.");
        }
    }

    long getUserId() {
        return 1; // Simula el ID del usuario administrador
    }
};

class Storage {
private:
    std::vector<Statistics> database;

public:
    Storage() {
        // Simula una base de datos de estadísticas
        database.emplace_back(1672531200, "Statistic A"); // 2023-01-01 00:00:00
        database.emplace_back(1672617600, "Statistic B"); // 2023-01-02 00:00:00
        database.emplace_back(1672704000, "Statistic C"); // 2023-01-03 00:00:00
    }

    std::vector<Statistics> getStatistics(std::time_t from, std::time_t to) {
        std::vector<Statistics> result;
        for (const auto& stat : database) {
            if (stat.captureTime >= from && stat.captureTime <= to) {
                result.push_back(stat);
            }
        }
        std::sort(result.begin(), result.end(), [](const Statistics& a, const Statistics& b) {
            return a.captureTime < b.captureTime;
        });
        return result;
    }
};

class StatisticsResource {
private:
    Logger logger;
    PermissionsService permissionsService;
    Storage storage;

public:
    StatisticsResource() {
        logger.info("StatisticsResource initialized.");
    }

    std::vector<Statistics> getStatistics(std::time_t from, std::time_t to) {
        try {
            permissionsService.checkAdmin(permissionsService.getUserId());
            auto stats = storage.getStatistics(from, to);
            logger.info("Statistics fetched successfully.");
            return stats;
        } catch (const std::exception& e) {
            logger.error("Error fetching statistics: " + std::string(e.what()));
            throw;
        }
    }
};

int main() {
    StatisticsResource resource;

    // Obtener estadísticas entre dos fechas
    try {
        std::time_t from = 1672531200; // 2023-01-01 00:00:00
        std::time_t to = 1672704000;   // 2023-01-03 00:00:00

        auto stats = resource.getStatistics(from, to);
        for (const auto& stat : stats) {
            std::cout << "Capture Time: " << stat.captureTime << ", Data: " << stat.data << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
