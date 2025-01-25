#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <memory>
#include <ctime>
#include <sstream>

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

class Device {
public:
    long id;
    std::string name;
    std::string uniqueId;

    Device(long id, const std::string& name, const std::string& uniqueId)
        : id(id), name(name), uniqueId(uniqueId) {}
};

class PermissionsService {
public:
    void checkPermission(long userId, const std::string& resource, long resourceId) {
        if (userId != 12345) {
            throw std::runtime_error("Permission denied for resource: " + resource);
        }
    }
};

class MediaManager {
public:
    std::ofstream createFileStream(const std::string& uniqueId, const std::string& name, const std::string& extension) {
        std::string filePath = uniqueId + "_" + name + "." + extension;
        std::ofstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filePath);
        }
        return file;
    }
};

class DeviceResource {
private:
    Logger logger;
    PermissionsService permissionsService;
    MediaManager mediaManager;

    const long IMAGE_SIZE_LIMIT = 500000;

    std::string imageExtension(const std::string& type) {
        if (type == "image/jpeg") return "jpg";
        if (type == "image/png") return "png";
        if (type == "image/gif") return "gif";
        if (type == "image/webp") return "webp";
        if (type == "image/svg+xml") return "svg";
        throw std::invalid_argument("Unsupported image type");
    }

public:
    DeviceResource() {
        logger.info("DeviceResource initialized.");
    }

    void uploadImage(long deviceId, const std::string& filePath, const std::string& type) {
        try {
            permissionsService.checkPermission(12345, "device", deviceId);

            std::string extension = imageExtension(type);
            std::ifstream inputFile(filePath, std::ios::binary);
            if (!inputFile.is_open()) {
                throw std::runtime_error("Failed to open input file: " + filePath);
            }

            std::ofstream outputFile = mediaManager.createFileStream(std::to_string(deviceId), "device", extension);

            char buffer[8192];
            size_t transferred = 0;
            while (inputFile.read(buffer, sizeof(buffer)) || inputFile.gcount() > 0) {
                outputFile.write(buffer, inputFile.gcount());
                transferred += inputFile.gcount();
                if (transferred > IMAGE_SIZE_LIMIT) {
                    throw std::invalid_argument("Image size limit exceeded");
                }
            }

            logger.info("Image uploaded successfully for device ID: " + std::to_string(deviceId));
        } catch (const std::exception& e) {
            logger.error("Error uploading image: " + std::string(e.what()));
        }
    }
};

int main() {
    DeviceResource resource;

    // Simula la carga de una imagen para un dispositivo
    resource.uploadImage(1, "sample_image.jpg", "image/jpeg");

    return 0;
}
