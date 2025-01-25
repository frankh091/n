#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>

class Logger {
public:
    void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }

    void error(const std::string& message) {
        std::cerr << "ERROR: " << message << std::endl;
    }
};

class Maintenance {
public:
    long id;
    std::string name;

    Maintenance(long id, const std::string& name) : id(id), name(name) {}
};

class Storage {
private:
    std::map<long, Maintenance> database;

public:
    void addMaintenance(const Maintenance& maintenance) {
        database[maintenance.id] = maintenance;
    }

    Maintenance getMaintenance(long id) {
        if (database.find(id) != database.end()) {
            return database[id];
        } else {
            throw std::runtime_error("Maintenance record not found with ID: " + std::to_string(id));
        }
    }

    std::vector<Maintenance> getAllMaintenanceRecords() {
        std::vector<Maintenance> result;
        for (const auto& entry : database) {
            result.push_back(entry.second);
        }
        return result;
    }
};

class MaintenanceResource {
private:
    Logger logger;
    Storage storage;

public:
    MaintenanceResource() {
        logger.info("MaintenanceResource initialized.");
    }

    void addMaintenance(long id, const std::string& name) {
        try {
            storage.addMaintenance(Maintenance(id, name));
            logger.info("Maintenance record added with ID: " + std::to_string(id));
        } catch (const std::exception& e) {
            logger.error("Error adding maintenance record: " + std::string(e.what()));
        }
    }

    void getMaintenance(long id) {
        try {
            Maintenance maintenance = storage.getMaintenance(id);
            logger.info("Maintenance record fetched with ID: " + std::to_string(id));
            std::cout << "Maintenance Name: " << maintenance.name << std::endl;
        } catch (const std::exception& e) {
            logger.error("Error fetching maintenance record: " + std::string(e.what()));
        }
    }

    void listMaintenanceRecords() {
        try {
            auto records = storage.getAllMaintenanceRecords();
            logger.info("Listing all maintenance records.");
            for (const auto& record : records) {
                std::cout << "Maintenance ID: " << record.id << ", Name: " << record.name << std::endl;
            }
        } catch (const std::exception& e) {
            logger.error("Error listing maintenance records: " + std::string(e.what()));
        }
    }
};

int main() {
    MaintenanceResource resource;

    // Simula operaciones CRUD sobre registros de mantenimiento
    resource.addMaintenance(1, "Oil Change");
    resource.addMaintenance(2, "Tire Rotation");

    resource.getMaintenance(1);

    resource.listMaintenanceRecords();

    return 0;
}
