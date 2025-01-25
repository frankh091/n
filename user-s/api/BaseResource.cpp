#include <iostream>
#include <string>
#include <memory>

class SecurityContext {
private:
    long userId;

public:
    SecurityContext(long userId) : userId(userId) {}

    long getUserId() const {
        return userId;
    }
};

class PermissionsService {
public:
    void checkPermission(long userId, const std::string& operation) {
        if (userId != 1) {
            throw std::runtime_error("Permission denied for operation: " + operation);
        }
    }
};

class BaseResource {
protected:
    SecurityContext securityContext;
    PermissionsService permissionsService;

public:
    BaseResource(long userId) : securityContext(userId) {}

    long getUserId() const {
        return securityContext.getUserId();
    }

    void checkAccess(const std::string& operation) {
        permissionsService.checkPermission(securityContext.getUserId(), operation);
    }
};

int main() {
    try {
        BaseResource resource(1); // Usuario con ID 1

        std::cout << "User ID: " << resource.getUserId() << std::endl;

        // Comprobación de permisos
        resource.checkAccess("read");

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
