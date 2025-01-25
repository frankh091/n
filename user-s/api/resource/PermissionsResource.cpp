#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <set>
#include <functional>

class Logger {
public:
    void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }

    void warn(const std::string& message) {
        std::cerr << "WARN: " << message << std::endl;
    }

    void error(const std::string& message) {
        std::cerr << "ERROR: " << message << std::endl;
    }
};

class Permission {
public:
    std::string ownerClass;
    long ownerId;
    std::string propertyClass;
    long propertyId;

    Permission(const std::map<std::string, long>& entity) {
        ownerClass = "OwnerClass";
        ownerId = entity.at("ownerId");
        propertyClass = "PropertyClass";
        propertyId = entity.at("propertyId");
    }
};

class PermissionsService {
public:
    bool notAdmin(long userId) {
        return userId != 1; // Simula que el usuario 1 es administrador
    }

    void checkPermission(const std::string& resource, long userId, long resourceId) {
        if (userId != resourceId) {
            throw std::runtime_error("Permission denied for resource: " + resource);
        }
    }

    void checkRestriction(long userId, const std::function<bool()>& restriction) {
        if (restriction()) {
            throw std::runtime_error("User has read-only restrictions.");
        }
    }
};

class Storage {
public:
    void addPermission(const Permission& permission) {
        std::cout << "Added permission for owner ID: " << permission.ownerId
                  << " and property ID: " << permission.propertyId << std::endl;
    }

    void removePermission(const Permission& permission) {
        std::cout << "Removed permission for owner ID: " << permission.ownerId
                  << " and property ID: " << permission.propertyId << std::endl;
    }
};

class CacheManager {
public:
    void invalidatePermission(bool full, const std::string& ownerClass, long ownerId,
                              const std::string& propertyClass, long propertyId, bool add) {
        std::cout << (add ? "Invalidate added permission" : "Invalidate removed permission")
                  << " for owner ID: " << ownerId << " and property ID: " << propertyId << std::endl;
    }
};

class PermissionsResource {
private:
    Logger logger;
    PermissionsService permissionsService;
    Storage storage;
    CacheManager cacheManager;

    void checkPermissionTypes(const std::vector<std::map<std::string, long>>& entities) {
        std::set<std::string> keys;
        for (const auto& entity : entities) {
            if (!keys.empty() && keys != std::set<std::string>(entity.begin(), entity.end())) {
                throw std::runtime_error("Invalid permission type structure.");
            }
            for (const auto& [key, _] : entity) {
                keys.insert(key);
            }
        }
    }

public:
    PermissionsResource() {
        logger.info("PermissionsResource initialized.");
    }

    void add(const std::vector<std::map<std::string, long>>& entities) {
        try {
            permissionsService.checkRestriction(12345, [] { return false; });
            checkPermissionTypes(entities);

            for (const auto& entity : entities) {
                Permission permission(entity);
                permissionsService.checkPermission(permission.ownerClass, 12345, permission.ownerId);
                permissionsService.checkPermission(permission.propertyClass, 12345, permission.propertyId);
                storage.addPermission(permission);
                cacheManager.invalidatePermission(true, permission.ownerClass, permission.ownerId,
                                                  permission.propertyClass, permission.propertyId, true);
            }
            logger.info("Permissions added successfully.");
        } catch (const std::exception& e) {
            logger.error("Error adding permissions: " + std::string(e.what()));
        }
    }

    void remove(const std::vector<std::map<std::string, long>>& entities) {
        try {
            permissionsService.checkRestriction(12345, [] { return false; });
            checkPermissionTypes(entities);

            for (const auto& entity : entities) {
                Permission permission(entity);
                permissionsService.checkPermission(permission.ownerClass, 12345, permission.ownerId);
                permissionsService.checkPermission(permission.propertyClass, 12345, permission.propertyId);
                storage.removePermission(permission);
                cacheManager.invalidatePermission(true, permission.ownerClass, permission.ownerId,
                                                  permission.propertyClass, permission.propertyId, false);
            }
            logger.info("Permissions removed successfully.");
        } catch (const std::exception& e) {
            logger.error("Error removing permissions: " + std::string(e.what()));
        }
    }
};

int main() {
    PermissionsResource resource;

    // Simula la adición de permisos
    resource.add({{{"ownerId", 1}, {"propertyId", 100}}});

    // Simula la eliminación de permisos
    resource.remove({{{"ownerId", 1}, {"propertyId", 100}}});

    return 0;
}
