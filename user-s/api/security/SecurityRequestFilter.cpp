#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <stdexcept>
#include <ctime>

class Logger {
public:
    void warn(const std::string& message) {
        std::cerr << "WARN: " << message << std::endl;
    }

    void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }
};

class User {
public:
    long id;
    bool disabled;

    User(long id, bool disabled) : id(id), disabled(disabled) {}

    void checkDisabled() {
        if (disabled) {
            throw std::runtime_error("User account is disabled.");
        }
    }
};

class LoginResult {
public:
    std::shared_ptr<User> user;
    std::time_t expiration;

    LoginResult(std::shared_ptr<User> user, std::time_t expiration)
        : user(std::move(user)), expiration(expiration) {}

    std::shared_ptr<User> getUser() const {
        return user;
    }

    std::time_t getExpiration() const {
        return expiration;
    }
};

class LoginService {
public:
    LoginResult login(const std::string& scheme, const std::string& credentials) {
        if (scheme == "Bearer" && credentials == "valid_token") {
            auto user = std::make_shared<User>(12345, false);
            return LoginResult(user, std::time(nullptr) + 3600);
        } else {
            throw std::runtime_error("Invalid credentials");
        }
    }
};

class StatisticsManager {
public:
    void registerRequest(long userId) {
        std::cout << "Request registered for user ID: " << userId << std::endl;
    }
};

class SecurityRequestFilter {
private:
    Logger logger;
    LoginService loginService;
    StatisticsManager statisticsManager;
    std::map<std::string, std::string> headers;

public:
    void setHeader(const std::string& key, const std::string& value) {
        headers[key] = value;
    }

    void filter() {
        try {
            auto authHeaderIt = headers.find("Authorization");
            if (authHeaderIt != headers.end()) {
                auto authHeader = authHeaderIt->second;
                auto spacePos = authHeader.find(' ');
                if (spacePos != std::string::npos) {
                    std::string scheme = authHeader.substr(0, spacePos);
                    std::string credentials = authHeader.substr(spacePos + 1);

                    auto loginResult = loginService.login(scheme, credentials);
                    auto user = loginResult.getUser();
                    user->checkDisabled();

                    statisticsManager.registerRequest(user->id);
                    logger.info("Authentication successful for user ID: " + std::to_string(user->id));
                } else {
                    throw std::runtime_error("Invalid Authorization header format.");
                }
            } else {
                logger.warn("No Authorization header provided.");
                throw std::runtime_error("Unauthorized");
            }
        } catch (const std::exception& e) {
            logger.warn("Authentication error: " + std::string(e.what()));
            throw;
        }
    }
};

int main() {
    SecurityRequestFilter filter;

    // Simula una solicitud con cabecera de autorización válida
    try {
        filter.setHeader("Authorization", "Bearer valid_token");
        filter.filter();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Simula una solicitud sin cabecera de autorización
    try {
        SecurityRequestFilter invalidFilter;
        invalidFilter.filter();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
