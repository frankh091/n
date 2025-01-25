#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <algorithm>

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

class Command {
public:
    long id;
    long deviceId;
    std::string type;
    bool textChannel;

    Command(long id, long deviceId, const std::string& type, bool textChannel)
        : id(id), deviceId(deviceId), type(type), textChannel(textChannel) {}
};

class PermissionsService {
public:
    void checkPermission(long userId, const std::string& resource, long resourceId) {
        if (userId != 12345) {
            throw std::runtime_error("Permission denied for resource: " + resource);
        }
    }
};

class CommandsManager {
public:
    Command sendCommand(const Command& command) {
        // Simula el envío del comando y retorna una copia
        return command;
    }
};

class CommandResource {
private:
    Logger logger;
    PermissionsService permissionsService;
    CommandsManager commandsManager;
    long userId;

public:
    CommandResource(long userId) : userId(userId) {
        logger.info("CommandResource initialized for user: " + std::to_string(userId));
    }

    std::vector<Command> getCommands(long deviceId) {
        try {
            permissionsService.checkPermission(userId, "device", deviceId);

            // Simulación de comandos obtenidos
            std::vector<Command> commands = {
                Command(1, deviceId, "TYPE_CUSTOM", true),
                Command(2, deviceId, "TYPE_DATA", false)
            };

            logger.info("Commands fetched for device ID: " + std::to_string(deviceId));
            return commands;
        } catch (const std::exception& e) {
            logger.error("Error fetching commands: " + std::string(e.what()));
            return {};
        }
    }

    Command sendCommand(const Command& command) {
        try {
            permissionsService.checkPermission(userId, "device", command.deviceId);
            Command sentCommand = commandsManager.sendCommand(command);
            logger.info("Command sent to device ID: " + std::to_string(command.deviceId));
            return sentCommand;
        } catch (const std::exception& e) {
            logger.error("Error sending command: " + std::string(e.what()));
            throw;
        }
    }

    std::vector<std::string> getCommandTypes(bool textChannel) {
        try {
            std::vector<std::string> textCommands = {"TYPE_CUSTOM", "TYPE_TEXT"};
            std::vector<std::string> dataCommands = {"TYPE_DATA", "TYPE_BINARY"};

            logger.info("Fetching command types for " + std::string(textChannel ? "text" : "data") + " channel.");
            return textChannel ? textCommands : dataCommands;
        } catch (const std::exception& e) {
            logger.error("Error fetching command types: " + std::string(e.what()));
            return {};
        }
    }
};

int main() {
    CommandResource resource(12345);

    // Obtener comandos
    auto commands = resource.getCommands(1);
    for (const auto& command : commands) {
        std::cout << "Command: " << command.type << " (Device ID: " << command.deviceId << ")" << std::endl;
    }

    // Enviar un comando
    Command commandToSend(3, 1, "TYPE_CUSTOM", true);
    resource.sendCommand(commandToSend);

    // Obtener tipos de comandos
    auto commandTypes = resource.getCommandTypes(true);
    for (const auto& type : commandTypes) {
        std::cout << "Command Type: " << type << std::endl;
    }

    return 0;
}
