#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>
#include <optional>
#include <functional>

class Device {
public:
    enum class Status { UNKNOWN, ONLINE, OFFLINE };

private:
    long id;
    std::string uniqueId;
    Status status;

public:
    Device(long id, const std::string& uniqueId) : id(id), uniqueId(uniqueId), status(Status::UNKNOWN) {}

    long getId() const { return id; }
    std::string getUniqueId() const { return uniqueId; }
    Status getStatus() const { return status; }

    void setStatus(Status newStatus) { status = newStatus; }
};

class DeviceSession {
private:
    std::string sessionId;
    std::shared_ptr<Device> device;

public:
    DeviceSession(const std::string& sessionId, const std::shared_ptr<Device>& device)
        : sessionId(sessionId), device(device) {}

    std::string getSessionId() const { return sessionId; }
    std::shared_ptr<Device> getDevice() const { return device; }
};

class ConnectionManager {
private:
    std::unordered_map<std::string, std::shared_ptr<DeviceSession>> sessions;
    std::unordered_map<std::string, std::shared_ptr<Device>> devices;

public:
    void addDevice(const std::shared_ptr<Device>& device) {
        devices[device->getUniqueId()] = device;
    }

    std::optional<std::shared_ptr<DeviceSession>> getSession(const std::string& uniqueId) const {
        auto it = sessions.find(uniqueId);
        if (it != sessions.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    std::shared_ptr<DeviceSession> createSession(const std::string& sessionId, const std::string& uniqueId) {
        auto it = devices.find(uniqueId);
        if (it == devices.end()) {
            throw std::runtime_error("Device not found: " + uniqueId);
        }

        auto session = std::make_shared<DeviceSession>(sessionId, it->second);
        sessions[uniqueId] = session;
        updateDeviceStatus(uniqueId, Device::Status::ONLINE);
        return session;
    }

    void removeSession(const std::string& uniqueId) {
        sessions.erase(uniqueId);
        updateDeviceStatus(uniqueId, Device::Status::OFFLINE);
    }

    void updateDeviceStatus(const std::string& uniqueId, Device::Status newStatus) {
        auto it = devices.find(uniqueId);
        if (it != devices.end()) {
            it->second->setStatus(newStatus);
            notifyListeners(it->second);
        }
    }

    void notifyListeners(const std::shared_ptr<Device>& device) {
        std::cout << "Device " << device->getUniqueId() << " is now "
                  << (device->getStatus() == Device::Status::ONLINE ? "ONLINE" : "OFFLINE") << std::endl;
    }
};

int main() {
    ConnectionManager manager;

    auto device1 = std::make_shared<Device>(1, "device_1");
    manager.addDevice(device1);

    auto session = manager.createSession("session_1", "device_1");
    std::cout << "Session created for device: " << session->getDevice()->getUniqueId() << std::endl;

    manager.removeSession("device_1");

    return 0;
}
