#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <fstream>
#include <functional>

class Logger {
public:
    void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }

    void error(const std::string& message) {
        std::cerr << "ERROR: " << message << std::endl;
    }
};

struct Position {
    long id;
    long deviceId;
    std::string timestamp;

    Position(long id, long deviceId, const std::string& timestamp)
        : id(id), deviceId(deviceId), timestamp(timestamp) {}
};

class PermissionsService {
public:
    void checkPermission(long userId, long deviceId) {
        if (userId != 12345) {
            throw std::runtime_error("Permission denied for device ID: " + std::to_string(deviceId));
        }
    }

    void checkRestriction(long userId, const std::function<bool()>& restriction) {
        if (restriction()) {
            throw std::runtime_error("User has restricted access.");
        }
    }
};

class ExportProvider {
public:
    void generateCsv(const std::vector<Position>& positions, const std::string& filePath) {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to create CSV file: " + filePath);
        }
        file << "ID,DeviceID,Timestamp\n";
        for (const auto& position : positions) {
            file << position.id << "," << position.deviceId << "," << position.timestamp << "\n";
        }
        file.close();
    }
};

class PositionResource {
private:
    Logger logger;
    PermissionsService permissionsService;
    ExportProvider exportProvider;

    std::vector<Position> mockDatabase;

public:
    PositionResource() {
        logger.info("PositionResource initialized.");
        mockDatabase.push_back(Position(1, 1001, "2025-01-01T10:00:00Z"));
        mockDatabase.push_back(Position(2, 1001, "2025-01-01T10:05:00Z"));
        mockDatabase.push_back(Position(3, 1002, "2025-01-01T11:00:00Z"));
    }

    std::vector<Position> getPositions(long deviceId) {
        try {
            permissionsService.checkPermission(12345, deviceId);
            std::vector<Position> result;
            for (const auto& position : mockDatabase) {
                if (position.deviceId == deviceId) {
                    result.push_back(position);
                }
            }
            logger.info("Positions fetched for device ID: " + std::to_string(deviceId));
            return result;
        } catch (const std::exception& e) {
            logger.error("Error fetching positions: " + std::string(e.what()));
            return {};
        }
    }

    void exportPositionsToCsv(long deviceId, const std::string& filePath) {
        try {
            permissionsService.checkPermission(12345, deviceId);
            auto positions = getPositions(deviceId);
            exportProvider.generateCsv(positions, filePath);
            logger.info("Positions exported to CSV for device ID: " + std::to_string(deviceId));
        } catch (const std::exception& e) {
            logger.error("Error exporting positions: " + std::string(e.what()));
        }
    }
};

int main() {
    PositionResource resource;

    // Obtener posiciones
    auto positions = resource.getPositions(1001);
    for (const auto& position : positions) {
        std::cout << "Position ID: " << position.id << ", Timestamp: " << position.timestamp << std::endl;
    }

    // Exportar posiciones a CSV
    resource.exportPositionsToCsv(1001, "positions.csv");

    return 0;
}
