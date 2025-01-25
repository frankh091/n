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
    std::map<long, std::map<std::string, std::string>> database;

public:
    void add(long id, const std::map<std::string, std::string>& attributes) {
        database[id] = attributes;
    }

    std::map<std::string, std::string> get(long id) {
        if (database.find(id) != database.end()) {
            return database[id];
        } else {
            throw std::runtime_error("Resource not found with ID: " + std::to_string(id));
        }
    }

    void update(long id, const std::map<std::string, std::string>& attributes) {
        if (database.find(id) != database.end()) {
            database[id] = attributes;
        } else {
            throw std::runtime_error("Cannot update non-existing resource with ID: " + std::to_string(id));
        }
    }

    void remove(long id) {
        if (database.find(id) != database.end()) {
            database.erase(id);
        } else {
            throw std::runtime_error("Cannot remove non-existing resource with ID: " + std::to_string(id));
        }
    }
};

class AttributeResource {
private:
    Logger logger;
    PermissionsService permissionsService;
    Storage storage;
    long userId;

public:
    AttributeResource(long userId) : userId(userId) {
        logger.info("AttributeResource initialized for user: " + std::to_string(userId));
    }

    void addAttribute(long id, const std::map<std::string, std::string>& attributes) {
        try {
            permissionsService.checkPermission(userId, "attribute", id);
            storage.add(id, attributes);
            logger.info("Attribute added with ID: " + std::to_string(id));
        } catch (const std::exception& e) {
            logger.error("Error adding attribute: " + std::string(e.what()));
        }
    }

    void getAttribute(long id) {
        try {
            permissionsService.checkPermission(userId, "attribute", id);
            auto attributes = storage.get(id);
            logger.info("Attributes fetched for ID: " + std::to_string(id));
            for (const auto& [key, value] : attributes) {
                std::cout << key << ": " << value << std::endl;
            }
        } catch (const std::exception& e) {
            logger.error("Error fetching attribute: " + std::string(e.what()));
        }
    }

    void updateAttribute(long id, const std::map<std::string, std::string>& attributes) {
        try {
            permissionsService.checkPermission(userId, "attribute", id);
            storage.update(id, attributes);
            logger.info("Attribute updated for ID: " + std::to_string(id));
        } catch (const std::exception& e) {
            logger.error("Error updating attribute: " + std::string(e.what()));
        }
    }

    void removeAttribute(long id) {
        try {
            permissionsService.checkPermission(userId, "attribute", id);
            storage.remove(id);
            logger.info("Attribute removed with ID: " + std::to_string(id));
        } catch (const std::exception& e) {
            logger.error("Error removing attribute: " + std::string(e.what()));
        }
    }
};

int main() {
    AttributeResource resource(12345);

    // Simula operaciones CRUD sobre atributos
    resource.addAttribute(1, {{"key1", "value1"}, {"key2", "value2"}});
    resource.getAttribute(1);
    resource.updateAttribute(1, {{"key1", "newValue1"}});
    resource.removeAttribute(1);

    return 0;
}
