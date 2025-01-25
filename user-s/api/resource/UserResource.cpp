#include <iostream>
#include <string>
#include <vector>
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

class User {
public:
    long id;
    std::string name;
    bool administrator;

    User(long id, const std::string& name, bool administrator)
        : id(id), name(name), administrator(administrator) {}
};

class PermissionsService {
public:
    void checkUser(long requesterId, long targetId) {
        if (requesterId != targetId && requesterId != 1) {
            throw std::runtime_error("Permission denied.");
        }
    }

    bool isAdmin(long userId) {
        return userId == 1;
    }
};

class Storage {
private:
    std::vector<User> database;

public:
    Storage() {
        // Simula una base de datos con usuarios
        database.emplace_back(1, "Admin", true);
        database.emplace_back(2, "User1", false);
    }

    std::vector<User> getUsers() {
        return database;
    }

    void addUser(const User& user) {
        database.push_back(user);
    }

    void removeUser(long id) {
        auto it = std::remove_if(database.begin(), database.end(), [id](const User& user) {
            return user.id == id;
        });
        if (it == database.end()) {
            throw std::runtime_error("User not found.");
        }
        database.erase(it, database.end());
    }
};

class UserResource {
private:
    Logger logger;
    PermissionsService permissionsService;
    Storage storage;

public:
    UserResource() {
        logger.info("UserResource initialized.");
    }

    std::vector<User> getUsers(long requesterId) {
        try {
            if (!permissionsService.isAdmin(requesterId)) {
                throw std::runtime_error("Only administrators can access user list.");
            }
            auto users = storage.getUsers();
            logger.info("User list fetched.");
            return users;
        } catch (const std::exception& e) {
            logger.error("Error fetching users: " + std::string(e.what()));
            return {};
        }
    }

    void addUser(long requesterId, const std::string& name, bool isAdmin) {
        try {
            if (!permissionsService.isAdmin(requesterId)) {
                throw std::runtime_error("Only administrators can add users.");
            }
            storage.addUser(User(storage.getUsers().size() + 1, name, isAdmin));
            logger.info("User added: " + name);
        } catch (const std::exception& e) {
            logger.error("Error adding user: " + std::string(e.what()));
        }
    }

    void removeUser(long requesterId, long targetId) {
        try {
            permissionsService.checkUser(requesterId, targetId);
            storage.removeUser(targetId);
            logger.info("User removed with ID: " + std::to_string(targetId));
        } catch (const std::exception& e) {
            logger.error("Error removing user: " + std::string(e.what()));
        }
    }
};

int main() {
    UserResource resource;

    // Obtener lista de usuarios
    try {
        auto users = resource.getUsers(1);
        for (const auto& user : users) {
            std::cout << "ID: " << user.id << ", Name: " << user.name << ", Admin: " << user.administrator << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    // Agregar un nuevo usuario
    resource.addUser(1, "NewUser", false);

    // Eliminar un usuario
    resource.removeUser(1, 2);

    return 0;
}
