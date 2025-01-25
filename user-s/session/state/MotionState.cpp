#include <iostream>
#include <string>
#include <chrono>
#include <optional>
#include <memory>

class Event {
private:
    std::string type;

public:
    Event(const std::string& type) : type(type) {}

    std::string getType() const {
        return type;
    }
};

class Device {
private:
    bool motionStreak;
    bool motionState;
    std::optional<std::chrono::system_clock::time_point> motionTime;
    double motionDistance;

public:
    Device() : motionStreak(false), motionState(false), motionDistance(0.0) {}

    bool getMotionStreak() const {
        return motionStreak;
    }

    void setMotionStreak(bool value) {
        motionStreak = value;
    }

    bool getMotionState() const {
        return motionState;
    }

    void setMotionState(bool value) {
        motionState = value;
    }

    std::optional<std::chrono::system_clock::time_point> getMotionTime() const {
        return motionTime;
    }

    void setMotionTime(const std::optional<std::chrono::system_clock::time_point>& time) {
        motionTime = time;
    }

    double getMotionDistance() const {
        return motionDistance;
    }

    void setMotionDistance(double distance) {
        motionDistance = distance;
    }
};

class MotionState {
private:
    bool changed;
    bool motionStreak;
    bool motionState;
    std::optional<std::chrono::system_clock::time_point> motionTime;
    double motionDistance;
    std::shared_ptr<Event> event;

public:
    MotionState() : changed(false), motionStreak(false), motionState(false), motionDistance(0.0) {}

    static MotionState fromDevice(const Device& device) {
        MotionState state;
        state.motionStreak = device.getMotionStreak();
        state.motionState = device.getMotionState();
        state.motionTime = device.getMotionTime();
        state.motionDistance = device.getMotionDistance();
        return state;
    }

    void toDevice(Device& device) const {
        device.setMotionStreak(motionStreak);
        device.setMotionState(motionState);
        device.setMotionTime(motionTime);
        device.setMotionDistance(motionDistance);
    }

    bool isChanged() const {
        return changed;
    }

    bool getMotionStreak() const {
        return motionStreak;
    }

    void setMotionStreak(bool value) {
        motionStreak = value;
        changed = true;
    }

    bool getMotionState() const {
        return motionState;
    }

    void setMotionState(bool value) {
        motionState = value;
        changed = true;
    }

    std::optional<std::chrono::system_clock::time_point> getMotionTime() const {
        return motionTime;
    }

    void setMotionTime(const std::optional<std::chrono::system_clock::time_point>& time) {
        motionTime = time;
        changed = true;
    }

    double getMotionDistance() const {
        return motionDistance;
    }

    void setMotionDistance(double distance) {
        motionDistance = distance;
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
    device.setMotionStreak(true);
    device.setMotionState(false);
    device.setMotionDistance(150.0);

    auto now = std::chrono::system_clock::now();
    device.setMotionTime(now);

    MotionState state = MotionState::fromDevice(device);
    state.setMotionState(true);

    std::cout << "Motion state changed: " << std::boolalpha << state.isChanged() << std::endl;

    state.toDevice(device);

    std::cout << "Device motion state: " << std::boolalpha << device.getMotionState() << std::endl;

    return 0;
}
