#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <map>

class Logger {
public:
    void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }

    void error(const std::string& message) {
        std::cerr << "ERROR: " << message << std::endl;
    }
};

struct ReportItem {
    long id;
    std::string name;
    std::string timestamp;
};

class PermissionsService {
public:
    void checkRestriction(long userId, const std::string& restriction) {
        if (userId != 12345) {
            throw std::runtime_error("Restriction applied: " + restriction);
        }
    }
};

class ExportProvider {
public:
    void exportToCsv(const std::vector<ReportItem>& items, const std::string& filePath) {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to create file: " + filePath);
        }
        file << "ID,Name,Timestamp\n";
        for (const auto& item : items) {
            file << item.id << "," << item.name << "," << item.timestamp << "\n";
        }
        file.close();
    }
};

class ReportResource {
private:
    Logger logger;
    PermissionsService permissionsService;
    ExportProvider exportProvider;

    std::vector<ReportItem> mockDatabase;

public:
    ReportResource() {
        logger.info("ReportResource initialized.");
        mockDatabase.push_back({1, "Item 1", "2025-01-01T10:00:00Z"});
        mockDatabase.push_back({2, "Item 2", "2025-01-01T10:30:00Z"});
    }

    std::vector<ReportItem> getSummary(long userId) {
        try {
            permissionsService.checkRestriction(userId, "summary");
            logger.info("Fetching summary report.");
            return mockDatabase;
        } catch (const std::exception& e) {
            logger.error("Error fetching summary report: " + std::string(e.what()));
            return {};
        }
    }

    void exportSummaryToCsv(long userId, const std::string& filePath) {
        try {
            permissionsService.checkRestriction(userId, "summary");
            auto items = getSummary(userId);
            exportProvider.exportToCsv(items, filePath);
            logger.info("Summary exported to CSV: " + filePath);
        } catch (const std::exception& e) {
            logger.error("Error exporting summary: " + std::string(e.what()));
        }
    }
};

int main() {
    ReportResource resource;

    long userId = 12345;

    // Obtener resumen
    auto summary = resource.getSummary(userId);
    for (const auto& item : summary) {
        std::cout << "ID: " << item.id << ", Name: " << item.name << ", Timestamp: " << item.timestamp << std::endl;
    }

    // Exportar resumen a CSV
    resource.exportSummaryToCsv(userId, "summary_report.csv");

    return 0;
}
