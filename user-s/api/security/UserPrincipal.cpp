#include <iostream>
#include <string>
#include <ctime>
#include <optional>

class UserPrincipal {
private:
    long userId;
    std::optional<std::time_t> expiration;

public:
    UserPrincipal(long userId, std::time_t expiration)
        : userId(userId), expiration(expiration) {}

    long getUserId() const {
        return userId;
    }

    std::optional<std::time_t> getExpiration() const {
        return expiration;
    }

    std::string getName() const {
        return ""; // En este caso, el nombre no se utiliza, como en el archivo original
    }
};

int main() {
    std::time_t now = std::time(nullptr);
    UserPrincipal userPrincipal(12345, now + 3600); // Usuario con expiración en una hora

    std::cout << "User ID: " << userPrincipal.getUserId() << std::endl;

    if (userPrincipal.getExpiration().has_value()) {
        std::cout << "Expiration: " << std::ctime(&userPrincipal.getExpiration().value());
    } else {
        std::cout << "No expiration set." << std::endl;
    }

    std::cout << "Name: " << userPrincipal.getName() << std::endl;

    return 0;
}
