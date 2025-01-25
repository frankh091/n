#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <optional>
#include <stdexcept>

class Protocol {
public:
    void sendDataCommand(const std::string& channel, const std::string& remoteAddress, const std::string& command) {
        std::cout << "Sending command: " << command << " to " << remoteAddress << " via channel " << channel << std::endl;
    }
};

class DeviceSession {
private:
    long deviceId;
    std::string uniqueId;
    std::string model;
    std::shared_ptr<Protocol> protocol;
    std::string channel;
    std::string remoteAddress;
    std::unordered_map<std::string, std::string> locals;

public:
    DeviceSession(long deviceId, const std::string& uniqueId, const std::string& model,
                  const std::shared_ptr<Protocol>& protocol, const std::string& channel, const std::string& remoteAddress)
        : deviceId(deviceId), uniqueId(uniqueId), model(model), protocol(protocol), channel(channel), remoteAddress(remoteAddress) {}

    long getDeviceId() const {
        return deviceId;
    }

    std::string getUniqueId() const {
        return uniqueId;
    }

    std::string getModel() const {
        return model;
    }

    std::string getChannel() const {
        return channel;
    }

    std::string getConnectionKey() const {
        return "Channel: " + channel + ", Remote Address: " + remoteAddress;
    }

    bool supportsLiveCommands() const {
        // Simulated check for live command support
        return channel.find("HttpRequestDecoder") == std::string::npos;
    }

    void sendCommand(const std::string& command) {
        if (protocol) {
            protocol->sendDataCommand(channel, remoteAddress, command);
        } else {
            throw std::runtime_error("Protocol not defined.");
        }
    }

    bool contains(const std::string& key) const {
        return locals.find(key) != locals.end();
    }

    void set(const std::string& key, const std::string& value) {
        if (!value.empty()) {
            locals[key] = value;
        } else {
            locals.erase(key);
        }
    }

    std::optional<std::string> get(const std::string& key) const {
        auto it = locals.find(key);
        if (it != locals.end()) {
            return it->second;
        }
        return std::nullopt;
    }
};

int main() {
    auto protocol = std::make_shared<Protocol>();

    DeviceSession session(1, "device_123", "ModelX", protocol, "ChannelA", "192.168.1.1");

    std::cout << "Device ID: " << session.getDeviceId() << std::endl;
    std::cout << "Unique ID: " << session.getUniqueId() << std::endl;
    std::cout << "Model: " << session.getModel() << std::endl;
    std::cout << "Connection Key: " << session.getConnectionKey() << std::endl;

    if (session.supportsLiveCommands()) {
        session.sendCommand("Activate");
    } else {
        std::cout << "Live commands not supported." << std::endl;
    }

    session.set("timezone", "UTC");
    if (session.contains("timezone")) {
        std::cout << "Timezone: " << session.get("timezone").value() << std::endl;
    }

    return 0;
}
