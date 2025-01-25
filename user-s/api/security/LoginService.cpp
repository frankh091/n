#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include <map>
#include <vector>
#include <ctime>
#include <optional>
#include <utility>

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
    std::string name;
    bool administrator;
    bool disabled;

    User(long id, const std::string& email, const std::string& name, bool administrator)
        : id(id), email(email), name(name), administrator(administrator), disabled(false) {}

    void checkDisabled() {
        if (disabled) {
            throw std::runtime_error("User account is disabled.");
        }
    }
};

class LoginResult {
public:
    std::shared_ptr<User> user;
    std::optional<std::time_t> expiration;

    LoginResult(std::shared_ptr<User> user, std::optional<std::time_t> expiration = std::nullopt)
        : user(user), expiration(expiration) {}
};

class TokenManager {
public:
    struct TokenData {
        long userId;
        std::time_t expiration;
    };

    TokenData verifyToken(const std::string& token) {
        if (token == "valid_token") {
            return {12345, std::time(nullptr) + 3600};
        } else {
            throw std::runtime_error("Invalid token.");
        }
    }
};

class LoginService {
private:
    Logger logger;
    TokenManager tokenManager;
    std::map<std::string, std::shared_ptr<User>> storage;

    void checkUserEnabled(std::shared_ptr<User> user) {
        if (!user) {
            throw std::runtime_error("Unknown account");
        }
        user->checkDisabled();
    }

public:
    LoginService() {
        logger.info("LoginService initialized.");
        // Simula una base de datos de usuarios
        storage["user@example.com"] = std::make_shared<User>(12345, "user@example.com", "John Doe", false);
    }

    LoginResult login(const std::string& scheme, const std::string& credentials) {
        if (scheme == "bearer") {
            return login(credentials);
        } else if (scheme == "basic") {
            auto delimiterPos = credentials.find(':');
            if (delimiterPos == std::string::npos) {
                throw std::runtime_error("Invalid Basic credentials format.");
            }
            std::string email = credentials.substr(0, delimiterPos);
            std::string password = credentials.substr(delimiterPos + 1);
            return login(email, password);
        } else {
            throw std::runtime_error("Unsupported authorization scheme.");
        }
    }

    LoginResult login(const std::string& token) {
        auto tokenData = tokenManager.verifyToken(token);
        auto userIt = std::find_if(storage.begin(), storage.end(), [&](const auto& pair) {
            return pair.second->id == tokenData.userId;
        });

        if (userIt != storage.end()) {
            checkUserEnabled(userIt->second);
            return LoginResult(userIt->second, tokenData.expiration);
        }
        throw std::runtime_error("User not found for token.");
    }

    LoginResult login(const std::string& email, const std::string& password) {
        auto it = storage.find(email);
        if (it != storage.end() && password == "password") {
            checkUserEnabled(it->second);
            return LoginResult(it->second);
        }
        throw std::runtime_error("Invalid email or password.");
    }
};

int main() {
    LoginService service;

    try {
        auto result = service.login("bearer", "valid_token");
        std::cout << "Logged in user: " << result.user->name << ", Expiration: " << result.expiration.value_or(0) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    try {
        auto result = service.login("basic", "user@example.com:password");
        std::cout << "Logged in user: " << result.user->name << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
