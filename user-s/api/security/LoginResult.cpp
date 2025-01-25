#include <iostream>
#include <memory>
#include <string>
#include <optional>
#include <ctime>

class User {
public:
    long id;
    std::string name;

    User(long id, const std::string& name) : id(id), name(name) {}
};

class LoginResult {
private:
    std::shared_ptr<User> user;
    std::optional<std::time_t> expiration;

public:
    LoginResult(std::shared_ptr<User> user) : user(user), expiration(std::nullopt) {}

    LoginResult(std::shared_ptr<User> user, std::time_t expiration)
        : user(user), expiration(expiration) {}

    std::shared_ptr<User> getUser() const {
        return user;
    }

    std::optional<std::time_t> getExpiration() const {
        return expiration;
    }
};

int main() {
    auto user = std::make_shared<User>(12345, "John Doe");

    // Crear un LoginResult sin fecha de expiración
    LoginResult resultWithoutExpiration(user);
    std::cout << "User ID: " << resultWithoutExpiration.getUser()->id
              << ", Name: " << resultWithoutExpiration.getUser()->name << std::endl;

    // Crear un LoginResult con fecha de expiración
    std::time_t now = std::time(nullptr);
    LoginResult resultWithExpiration(user, now);
    std::cout << "User ID: " << resultWithExpiration.getUser()->id
              << ", Expiration: " << (resultWithExpiration.getExpiration().has_value() ? std::ctime(&now) : "None")
              << std::endl;

    return 0;
}
