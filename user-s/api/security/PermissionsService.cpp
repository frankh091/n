#include <iostream>
#include <string>
#include <stdexcept>
#include <map>
#include <memory>
#include <functional>
#include <optional>

class Logger {
public:
    void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }

    void error(const std::string& message) {
        std::cerr << "ERROR: " << message << std::endl;
    }
};

class User {
public:
    long id;
    std::string name;
    bool administrator;
    int deviceLimit;
    int userLimit;

    User(long id, const std::string& name, bool administrator, int deviceLimit, int userLimit)
        : id(id), name(name), administrator(administrator), deviceLimit(deviceLimit), userLimit(userLimit) {}

    bool isAdmin() const {
        return administrator;
    }
};

class Storage {
private:
    std::map<long, std::shared_ptr<User>> users;

public:
    Storage() {
        // Simula datos de usuarios
        users[1] = std::make_shared<User>(1, "Admin", true, 10, 5);
        users[2] = std::make_shared<User>(2, "User", false, 2, 0);
    }

    std::shared_ptr<User> getUser(long userId) {
        if (users.find(userId) != users.end()) {
            return users[userId];
        }
        throw std::runtime_error("User not found with ID: " + std::to_string(userId));
    }
};

class PermissionsService {
private:
    Logger logger;
    Storage storage;

public:
    PermissionsService() {
        logger.info("PermissionsService initialized.");
    }

    bool notAdmin(long userId) {
        auto user = storage.getUser(userId);
        return !user->isAdmin();
    }

    void checkAdmin(long userId) {
        if (notAdmin(userId)) {
            throw std::runtime_error("Administrator access required.");
        }
    }

    void checkUser(long userId, long targetUserId) {
        if (userId != targetUserId && notAdmin(userId)) {
            throw std::runtime_error("Access denied to user: " + std::to_string(targetUserId));
        }
    }

    template <typename T>
    void checkPermission(long userId, long objectId) {
        if (notAdmin(userId)) {
            throw std::runtime_error("Access denied to object with ID: " + std::to_string(objectId));
        }
    }
};

int main() {
    PermissionsService permissionsService;

    try {
        permissionsService.checkAdmin(2);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    try {
        permissionsService.checkUser(2, 1);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    try {
        permissionsService.checkPermission<int>(2, 100);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
