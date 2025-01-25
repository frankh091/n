#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <memory>

class Logger {
public:
    void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }

    void error(const std::string& message) {
        std::cerr << "ERROR: " << message << std::endl;
    }
};

class Event {
public:
    long id;
    long deviceId;
    std::string description;

    Event(long id, long deviceId, const std::string& description)
        : id(id), deviceId(deviceId), description(description) {}
};

class PermissionsService {
public:
    void checkPermission(long userId, const std::string& resource, long resourceId) {
        if (userId != 12345) {
            throw std::runtime_error("Permission denied for resource: " + resource);
        }
    }
};

class Storage {
private:
    std::map<long, Event> database;

public:
    void addEvent(const Event& event) {
        database[event.id] = event;
    }

    Event getEvent(long id) {
        if (database.find(id) != database.end()) {
            return database[id];
        } else {
            throw std::runtime_error("Event not found with ID: " + std::to_string(id));
        }
    }
};

class EventResource {
private:
    Logger logger;
    PermissionsService permissionsService;
    Storage storage;
    long userId;

public:
    EventResource(long userId) : userId(userId) {
        logger.info("EventResource initialized for user: " + std::to_string(userId));
    }

    Event getEvent(long id) {
        try {
            Event event = storage.getEvent(id);
            permissionsService.checkPermission(userId, "event", event.deviceId);
            logger.info("Event fetched for ID: " + std::to_string(id));
            return event;
        } catch (const std::exception& e) {
            logger.error("Error fetching event: " + std::string(e.what()));
            throw;
        }
    }
};

int main() {
    EventResource resource(12345);

    // Agregar evento para la simulación
    resource.getEvent(1);

    return 0;
}
