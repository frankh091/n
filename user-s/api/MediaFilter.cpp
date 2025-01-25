#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;

class Device {
private:
    long id;
    std::string uniqueId;

public:
    Device(long id, const std::string& uniqueId) : id(id), uniqueId(uniqueId) {}

    long getId() const { return id; }
    std::string getUniqueId() const { return uniqueId; }
};

class Storage {
public:
    std::shared_ptr<Device> getObject(const std::string& uniqueId) {
        if (uniqueId == "validDevice") {
            return std::make_shared<Device>(1, uniqueId);
        }
        throw std::runtime_error("Device not found");
    }
};

class StatisticsManager {
public:
    void registerRequest(long userId) {
        std::cout << "Request registered for user ID: " << userId << std::endl;
    }
};

class PermissionsService {
public:
    void checkPermission(long userId, long deviceId) {
        if (userId != 1) {
            throw std::runtime_error("Permission denied for user ID: " + std::to_string(userId));
        }
    }
};

class MediaFilter {
private:
    Storage storage;
    StatisticsManager statisticsManager;
    PermissionsService permissionsService;

public:
    void doFilter(http::request<http::string_body>& req, http::response<http::string_body>& res, long userId) {
        try {
            if (userId == 0) {
                res.result(http::status::unauthorized);
                res.body() = "Unauthorized";
                return;
            }

            statisticsManager.registerRequest(userId);

            std::string path = req.target().to_string();
            std::istringstream pathStream(path);
            std::vector<std::string> parts;
            std::string segment;

            while (std::getline(pathStream, segment, '/')) {
                if (!segment.empty()) {
                    parts.push_back(segment);
                }
            }

            if (parts.size() >= 2) {
                auto device = storage.getObject(parts[1]);
                permissionsService.checkPermission(userId, device->getId());
                res.result(http::status::ok);
                res.body() = "Access granted";
                return;
            }

            res.result(http::status::forbidden);
            res.body() = "Forbidden: Invalid path";
        } catch (const std::exception& e) {
            res.result(http::status::forbidden);
            res.body() = "Forbidden: " + std::string(e.what());
        }
    }
};

int main() {
    try {
        net::io_context ioc;

        // Simulación de una solicitud HTTP
        http::request<http::string_body> req{http::verb::get, "/media/validDevice", 11};
        http::response<http::string_body> res;

        MediaFilter filter;
        long userId = 1; // Simula un usuario autenticado

        filter.doFilter(req, res, userId);

        std::cout << "Response: " << res.result_int() << " - " << res.body() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}