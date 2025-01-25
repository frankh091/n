#include <iostream>
#include <string>
#include <memory>

class UserPrincipal {
private:
    long userId;

public:
    UserPrincipal(long userId) : userId(userId) {}

    long getUserId() const {
        return userId;
    }
};

class UserSecurityContext {
private:
    std::shared_ptr<UserPrincipal> principal;

public:
    UserSecurityContext(std::shared_ptr<UserPrincipal> principal) : principal(principal) {}

    std::shared_ptr<UserPrincipal> getUserPrincipal() const {
        return principal;
    }

    bool isUserInRole(const std::string& role) const {
        return true; // Simplificación; siempre retorna true
    }

    bool isSecure() const {
        return false; // Simplificación; siempre retorna false
    }

    std::string getAuthenticationScheme() const {
        return "BASIC_AUTH"; // Simula el esquema de autenticación
    }
};

int main() {
    auto principal = std::make_shared<UserPrincipal>(12345);
    UserSecurityContext context(principal);

    // Imprimir detalles del contexto de seguridad
    std::cout << "User ID: " << context.getUserPrincipal()->getUserId() << std::endl;
    std::cout << "Is User in Role: " << (context.isUserInRole("admin") ? "Yes" : "No") << std::endl;
    std::cout << "Is Secure: " << (context.isSecure() ? "Yes" : "No") << std::endl;
    std::cout << "Authentication Scheme: " << context.getAuthenticationScheme() << std::endl;

    return 0;
}
