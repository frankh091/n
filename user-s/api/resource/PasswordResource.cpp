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

class User {
public:
    long id;
    std::string email;
    std::string password;

    User(long id, const std::string& email, const std::string& password)
        : id(id), email(email), password(password) {}
};

class Storage {
private:
    std::map<long, User> database;

public:
    void addUser(const User& user) {
        database[user.id] = user;
    }

    User* getUserByEmail(const std::string& email) {
        for (auto& [id, user] : database) {
            if (user.email == email) {
                return &user;
            }
        }
        return nullptr;
    }

    User* getUserById(long id) {
        if (database.find(id) != database.end()) {
            return &database[id];
        } else {
            return nullptr;
        }
    }

    void updateUserPassword(long id, const std::string& newPassword) {
        if (database.find(id) != database.end()) {
            database[id].password = newPassword;
        } else {
            throw std::runtime_error("User not found with ID: " + std::to_string(id));
        }
    }
};

class TokenManager {
public:
    long verifyToken(const std::string& token) {
        // Simula la verificación del token
        if (token == "valid_token") {
            return 12345; // Retorna un ID de usuario simulado
        } else {
            throw std::runtime_error("Invalid token");
        }
    }
};

class MailManager {
public:
    void sendResetEmail(const User& user) {
        std::cout << "Sending password reset email to: " << user.email << std::endl;
    }
};

class PasswordResource {
private:
    Logger logger;
    Storage storage;
    TokenManager tokenManager;
    MailManager mailManager;

public:
    PasswordResource() {
        logger.info("PasswordResource initialized.");
    }

    void resetPassword(const std::string& email) {
        try {
            User* user = storage.getUserByEmail(email);
            if (user) {
                mailManager.sendResetEmail(*user);
                logger.info("Password reset email sent to " + email);
            } else {
                logger.error("User not found with email: " + email);
            }
        } catch (const std::exception& e) {
            logger.error("Error resetting password: " + std::string(e.what()));
        }
    }

    void updatePassword(const std::string& token, const std::string& newPassword) {
        try {
            long userId = tokenManager.verifyToken(token);
            storage.updateUserPassword(userId, newPassword);
            logger.info("Password updated for user ID: " + std::to_string(userId));
        } catch (const std::exception& e) {
            logger.error("Error updating password: " + std::string(e.what()));
        }
    }
};

int main() {
    PasswordResource resource;

    // Simula el restablecimiento de contraseña
    resource.resetPassword("user@example.com");

    // Simula la actualización de contraseña
    resource.updatePassword("valid_token", "new_secure_password");

    return 0;
}
