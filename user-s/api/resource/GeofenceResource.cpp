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

class Geofence {
public:
    long id;
    std::string name;

    Geofence(long id, const std::string& name) : id(id), name(name) {}
};

class Storage {
private:
    std::map<long, Geofence> database;

public:
    void addGeofence(const Geofence& geofence) {
        database[geofence.id] = geofence;
    }

    Geofence getGeofence(long id) {
        if (database.find(id) != database.end()) {
            return database[id];
        } else {
            throw std::runtime_error("Geofence not found with ID: " + std::to_string(id));
        }
    }

    std::vector<Geofence> getAllGeofences() {
        std::vector<Geofence> result;
        for (const auto& entry : database) {
            result.push_back(entry.second);
        }
        return result;
    }
};

class GeofenceResource {
private:
    Logger logger;
    Storage storage;

public:
    GeofenceResource() {
        logger.info("GeofenceResource initialized.");
    }

    void addGeofence(long id, const std::string& name) {
        try {
            storage.addGeofence(Geofence(id, name));
            logger.info("Geofence added with ID: " + std::to_string(id));
        } catch (const std::exception& e) {
            logger.error("Error adding geofence: " + std::string(e.what()));
        }
    }

    void getGeofence(long id) {
        try {
            Geofence geofence = storage.getGeofence(id);
            logger.info("Geofence fetched with ID: " + std::to_string(id));
            std::cout << "Geofence Name: " << geofence.name << std::endl;
        } catch (const std::exception& e) {
            logger.error("Error fetching geofence: " + std::string(e.what()));
        }
    }

    void listGeofences() {
        try {
            auto geofences = storage.getAllGeofences();
            logger.info("Listing all geofences.");
            for (const auto& geofence : geofences) {
                std::cout << "Geofence ID: " << geofence.id << ", Name: " << geofence.name << std::endl;
            }
        } catch (const std::exception& e) {
            logger.error("Error listing geofences: " + std::string(e.what()));
        }
    }
};

int main() {
    GeofenceResource resource;

    // Simula operaciones CRUD sobre geocercas
    resource.addGeofence(1, "Home Zone");
    resource.addGeofence(2, "Work Zone");

    resource.getGeofence(1);

    resource.listGeofences();

    return 0;
}
