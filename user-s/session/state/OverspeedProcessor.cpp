#include <iostream>
#include <string>
#include <optional>
#include <chrono>
#include <memory>

class Event {
public:
    static constexpr const char* TYPE_DEVICE_OVERSPEED = "DEVICE_OVERSPEED";

    Event(const std::string& type, double speed, double speedLimit, long geofenceId)
        : type(type), speed(speed), speedLimit(speedLimit), geofenceId(geofenceId) {}

    void print() const {
        std::cout << "Event: " << type << ", Speed: " << speed << ", Speed Limit: "
                  << speedLimit << ", Geofence ID: " << geofenceId << std::endl;
    }

private:
    std::string type;
    double speed;
    double speedLimit;
    long geofenceId;
};

class Position {
private:
    std::chrono::time_point<std::chrono::system_clock> fixTime;
    double speed;

public:
    Position(const std::chrono::time_point<std::chrono::system_clock>& fixTime, double speed)
        : fixTime(fixTime), speed(speed) {}

    std::chrono::time_point<std::chrono::system_clock> getFixTime() const {
        return fixTime;
    }

    double getSpeed() const {
        return speed;
    }
};

class OverspeedState {
private:
    bool overspeedState;
    std::optional<std::chrono::time_point<std::chrono::system_clock>> overspeedTime;
    long overspeedGeofenceId;
    std::optional<std::shared_ptr<Event>> event;

public:
    OverspeedState() : overspeedState(false), overspeedGeofenceId(0) {}

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

    void setEvent(const std::shared_ptr<Event>& evt) {
        event = evt;
    }

    std::optional<std::shared_ptr<Event>> getEvent() const {
        return event;
    }
};

class OverspeedProcessor {
public:
    static void updateState(
        OverspeedState& state, const Position& position,
        double speedLimit, double multiplier, long minimalDuration, long geofenceId) {

        state.setEvent(nullptr);

        bool oldState = state.getOverspeedState();
        if (oldState) {
            bool newState = position.getSpeed() > speedLimit * multiplier;
            if (newState) {
                checkEvent(state, position, speedLimit, minimalDuration);
            } else {
                state.setOverspeedState(false);
                state.setOverspeedTime(std::nullopt);
                state.setOverspeedGeofenceId(0);
            }
        } else if (position.getSpeed() > speedLimit * multiplier) {
            state.setOverspeedState(true);
            state.setOverspeedTime(position.getFixTime());
            state.setOverspeedGeofenceId(geofenceId);

            checkEvent(state, position, speedLimit, minimalDuration);
        }
    }

private:
    static void checkEvent(OverspeedState& state, const Position& position, double speedLimit, long minimalDuration) {
        if (state.getOverspeedTime().has_value()) {
            auto oldTime = state.getOverspeedTime().value();
            auto newTime = position.getFixTime();
            if (std::chrono::duration_cast<std::chrono::seconds>(newTime - oldTime).count() >= minimalDuration) {

                auto event = std::make_shared<Event>(Event::TYPE_DEVICE_OVERSPEED,
                                                     position.getSpeed(), speedLimit,
                                                     state.getOverspeedGeofenceId());

                state.setOverspeedTime(std::nullopt);
                state.setOverspeedGeofenceId(0);
                state.setEvent(event);
            }
        }
    }
};

int main() {
    OverspeedState state;
    auto now = std::chrono::system_clock::now();
    Position position(now, 120.0); // Velocidad actual: 120

    OverspeedProcessor::updateState(state, position, 100.0, 1.0, 10, 42); // Límite: 100

    if (state.getEvent().has_value()) {
        state.getEvent().value()->print();
    } else {
        std::cout << "No overspeed event generated." << std::endl;
    }

    return 0;
}
