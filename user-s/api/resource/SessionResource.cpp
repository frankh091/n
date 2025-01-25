#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <ctime>
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

    User(long id, const std::string& email) : id(id), email(email) {}
};

class LoginService {
public:
    std::unique_ptr<User> login(const std::string& token) {
        if (token == "valid_token") {
            return std::make_unique<User>(12345, "user@example.com");
        } else {
            throw std::runtime_error("Invalid token");
        }
    }

    std::unique_ptr<User> login(const std::string& email, const std::string& password) {
        if (email == "user@example.com" && password == "password") {
            return std::make_unique<User>(12345, email);
        } else {
            throw std::runtime_error("Invalid credentials");
        }
    }
};

class TokenManager {
public:
    std::string generateToken(long userId, const std::time_t& expiration) {
        return "generated_token_for_user_" + std::to_string(userId);
    }
};

class SessionResource {
private:
    Logger logger;
    LoginService loginService;
    TokenManager tokenManager;
    std::map<long, std::unique_ptr<User>> sessions;

public:
    SessionResource() {
        logger.info("SessionResource initialized.");
    }

    User getSession(const std::string& token) {
        try {
            auto user = loginService.login(token);
            sessions[user->id] = std::move(user);
            logger.info("Session created for token.");
            return *sessions[12345];
        } catch (const std::exception& e) {
            logger.error("Error getting session: " + std::string(e.what()));
            throw;
        }
    }

    User loginUser(const std::string& email, const std::string& password) {
        try {
            auto user = loginService.login(email, password);
            sessions[user->id] = std::move(user);
            logger.info("User logged in successfully.");
            return *sessions[12345];
        } catch (const std::exception& e) {
            logger.error("Error logging in user: " + std::string(e.what()));
            throw;
        }
    }

    void removeSession(long userId) {
        try {
            sessions.erase(userId);
            logger.info("Session removed for user ID: " + std::to_string(userId));
        } catch (const std::exception& e) {
            logger.error("Error removing session: " + std::string(e.what()));
        }
    }

    std::string requestToken(long userId, const std::time_t& expiration) {
        try {
            return tokenManager.generateToken(userId, expiration);
        } catch (const std::exception& e) {
            logger.error("Error generating token: " + std::string(e.what()));
            throw;
        }
    }
};

int main() {
    SessionResource resource;

    // Simula inicio de sesión mediante token
    try {
        User user = resource.getSession("valid_token");
        std::cout << "User ID: " << user.id << ", Email: " << user.email << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    // Simula inicio de sesión mediante credenciales
    try {
        User user = resource.loginUser("user@example.com", "password");
        std::cout << "User ID: " << user.id << ", Email: " << user.email << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    // Simula la generación de un token
    try {
        std::string token = resource.requestToken(12345, std::time(nullptr) + 3600);
        std::cout << "Generated Token: " << token << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
