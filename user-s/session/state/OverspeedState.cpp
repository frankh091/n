#include <iostream>
#include <string>
#include <optional>
#include <chrono>
#include <memory>

class Event {
private:
    std::string type;
    double speed;
    double speedLimit;
    long geofenceId;

public:
    Event(const std::string& type, double speed, double speedLimit, long geofenceId)
        : type(type), speed(speed), speedLimit(speedLimit), geofenceId(geofenceId) {}

    std::string getType() const {
        return type;
    }

    double getSpeed() const {
        return speed;
    }

    double getSpeedLimit() const {
        return speedLimit;
    }

    long getGeofenceId() const {
        return geofenceId;
    }

    void print() const {
        std::cout << "Event: " << type << ", Speed: " << speed << ", Speed Limit: " << speedLimit
                  << ", Geofence ID: " << geofenceId << std::endl;
    }
};

class Device {
private:
    bool overspeedState;
    std::optional<std::chrono::time_point<std::chrono::system_clock>> overspeedTime;
    long overspeedGeofenceId;

public:
    Device() : overspeedState(false), overspeedGeofenceId(0) {}

    bool getOverspeedState() const {
        return overspeedState;
    }

    void setOverspeedState(bool state) {
        overspeedState = state;
    }

    std::optional<std::chrono::time_point<std::chrono::system_clock>> getOverspeedTime() const {
        return overspeedTime;
    }

    void setOverspeedTime(const std::optional<std::chrono::time_point<std::chrono::system_clock>>& time) {
        overspeedTime = time;
    }

    long getOverspeedGeofenceId() const {
        return overspeedGeofenceId;
    }

    void setOverspeedGeofenceId(long id) {
        overspeedGeofenceId = id;
    }
};

class OverspeedState {
private:
    bool changed;
    bool overspeedState;
    std::optional<std::chrono::time_point<std::chrono::system_clock>> overspeedTime;
    long overspeedGeofenceId;
    std::shared_ptr<Event> event;

public:
    OverspeedState() : changed(false), overspeedState(false), overspeedGeofenceId(0) {}

    static OverspeedState fromDevice(const Device& device) {
        OverspeedState state;
        state.overspeedState = device.getOverspeedState();
        state.overspeedTime = device.getOverspeedTime();
        state.overspeedGeofenceId = device.getOverspeedGeofenceId();
        return state;
    }

    void toDevice(Device& device) const {
        device.setOverspeedState(overspeedState);
        device.setOverspeedTime(overspeedTime);
        device.setOverspeedGeofenceId(overspeedGeofenceId);
    }

    bool isChanged() const {
        return changed;
    }

    bool getOverspeedState() const {
        return overspeedState;
    }

    void setOverspeedState(bool state) {
        overspeedState = state;
        changed = true;
    }

    std::optional<std::chrono::time_point<std::chrono::system_clock>> getOverspeedTime() const {
        return overspeedTime;
    }

    void setOverspeedTime(const std::optional<std::chrono::time_point<std::chrono::system_clock>>& time) {
        overspeedTime = time;
        changed = true;
    }

    long getOverspeedGeofenceId() const {
        return overspeedGeofenceId;
    }

    void setOverspeedGeofenceId(long id) {
        overspeedGeofenceId = id;
        changed = true;
    }

    std::shared_ptr<Event> getEvent() const {
        return event;
    }

    void setEvent(const std::shared_ptr<Event>& evt) {
        event = evt;
    }
};

int main() {
    Device device;
    device.setOverspeedState(true);
    device.setOverspeedGeofenceId(42);
    auto now = std::chrono::system_clock::now();
    device.setOverspeedTime(now);

    OverspeedState state = OverspeedState::fromDevice(device);
    state.setOverspeedState(false);
    state.setOverspeedGeofenceId(0);

    state.toDevice(device);

    std::cout << "Overspeed State: " << device.getOverspeedState() << ", Geofence ID: "
              << device.getOverspeedGeofenceId() << std::endl;

    return 0;
}
