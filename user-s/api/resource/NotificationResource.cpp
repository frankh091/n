#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <set>

class Logger {
public:
    void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }

    void warn(const std::string& message) {
        std::cerr << "WARN: " << message << std::endl;
    }

    void error(const std::string& message) {
        std::cerr << "ERROR: " << message << std::endl;
    }
};

class Notification {
public:
    long id;
    std::string description;

    Notification(long id, const std::string& description) : id(id), description(description) {}
};

class Typed {
public:
    std::string type;

    Typed(const std::string& type) : type(type) {}
};

class NotificatorManager {
public:
    std::vector<Typed> getAllNotificatorTypes() {
        return {Typed("email"), Typed("sms"), Typed("push")};
    }

    void sendNotification(const std::string& notificator, const std::string& message) {
        std::cout << "Sending notification via " << notificator << ": " << message << std::endl;
    }
};

class NotificationResource {
private:
    Logger logger;
    NotificatorManager notificatorManager;

public:
    NotificationResource() {
        logger.info("NotificationResource initialized.");
    }

    void listNotificators(bool announcement) {
        auto notificators = notificatorManager.getAllNotificatorTypes();
        std::set<std::string> unsupported = {"command", "web"};
        logger.info("Listing notificators.");
        for (const auto& notificator : notificators) {
            if (announcement && unsupported.find(notificator.type) != unsupported.end()) {
                continue;
            }
            std::cout << "Notificator Type: " << notificator.type << std::endl;
        }
    }

    void sendTestMessage(const std::string& notificator) {
        try {
            notificatorManager.sendNotification(notificator, "This is a test message.");
            logger.info("Test message sent via " + notificator);
        } catch (const std::exception& e) {
            logger.error("Error sending test message: " + std::string(e.what()));
        }
    }

    void sendCustomMessage(const std::string& notificator, const std::string& message) {
        try {
            notificatorManager.sendNotification(notificator, message);
            logger.info("Custom message sent via " + notificator);
        } catch (const std::exception& e) {
            logger.error("Error sending custom message: " + std::string(e.what()));
        }
    }
};

int main() {
    NotificationResource resource;

    // Listar tipos de notificadores
    resource.listNotificators(false);

    // Enviar un mensaje de prueba
    resource.sendTestMessage("email");

    // Enviar un mensaje personalizado
    resource.sendCustomMessage("sms", "Hello, this is a custom notification!");

    return 0;
}
