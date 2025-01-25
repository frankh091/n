#include <iostream>
#include <string>
#include <memory>
#include <optional>
#include <chrono>

class Event {
public:
    static constexpr const char* TYPE_DEVICE_MOVING = "DEVICE_MOVING";
    static constexpr const char* TYPE_DEVICE_STOPPED = "DEVICE_STOPPED";

    Event(const std::string& type) : type(type) {}

    std::string getType() const {
        return type;
    }

private:
    std::string type;
};

class Position {
private:
    std::chrono::time_point<std::chrono::system_clock> fixTime;
    double totalDistance;
    bool ignition;

public:
    Position(const std::chrono::time_point<std::chrono::system_clock>& fixTime, double totalDistance, bool ignition)
        : fixTime(fixTime), totalDistance(totalDistance), ignition(ignition) {}

    std::chrono::time_point<std::chrono::system_clock> getFixTime() const {
        return fixTime;
    }

    double getTotalDistance() const {
        return totalDistance;
    }

    bool hasIgnition() const {
        return ignition;
    }
};

class MotionState {
private:
    bool motionState;
    bool motionStreak;
    std::optional<std::chrono::time_point<std::chrono::system_clock>> motionTime;
    double motionDistance;
    std::optional<Event> event;

public:
    MotionState() : motionState(false), motionStreak(false), motionDistance(0.0) {}

    void setMotionState(bool state) {
        motionState = state;
    }

    bool getMotionState() const {
        return motionState;
    }

    void setMotionStreak(bool streak) {
        motionStreak = streak;
    }

    bool getMotionStreak() const {
        return motionStreak;
    }

    void setMotionTime(const std::optional<std::chrono::time_point<std::chrono::system_clock>>& time) {
        motionTime = time;
    }

    std::optional<std::chrono::time_point<std::chrono::system_clock>> getMotionTime() const {
        return motionTime;
    }

    void setMotionDistance(double distance) {
        motionDistance = distance;
    }

    double getMotionDistance() const {
        return motionDistance;
    }

    void setEvent(const Event& evt) {
        event = evt;
    }

    std::optional<Event> getEvent() const {
        return event;
    }
};

class TripsConfig {
private:
    bool useIgnition;
    long minimalTripDuration;
    double minimalTripDistance;
    long minimalParkingDuration;

public:
    TripsConfig(bool useIgnition, long tripDuration, double tripDistance, long parkingDuration)
        : useIgnition(useIgnition), minimalTripDuration(tripDuration), minimalTripDistance(tripDistance), minimalParkingDuration(parkingDuration) {}

    bool getUseIgnition() const {
        return useIgnition;
    }

    long getMinimalTripDuration() const {
        return minimalTripDuration;
    }

    double getMinimalTripDistance() const {
        return minimalTripDistance;
    }

    long getMinimalParkingDuration() const {
        return minimalParkingDuration;
    }
};

class MotionProcessor {
public:
    static void updateState(
        MotionState& state, const Position& position, bool newState, const TripsConfig& tripsConfig) {

        state.setEvent(std::nullopt);

        bool oldState = state.getMotionState();
        if (oldState == newState) {
            if (state.getMotionTime().has_value()) {
                auto oldTime = state.getMotionTime().value();
                auto newTime = position.getFixTime();

                double distance = position.getTotalDistance() - state.getMotionDistance();

                bool generateEvent = false;
                if (newState) {
                    if (std::chrono::duration_cast<std::chrono::seconds>(newTime - oldTime).count() >= tripsConfig.getMinimalTripDuration()
                        || distance >= tripsConfig.getMinimalTripDistance()) {
                        generateEvent = true;
                    }
                } else {
                    if (std::chrono::duration_cast<std::chrono::seconds>(newTime - oldTime).count() >= tripsConfig.getMinimalParkingDuration()) {
                        generateEvent = true;
                    }
                }

                if (generateEvent) {
                    std::string eventType = newState ? Event::TYPE_DEVICE_MOVING : Event::TYPE_DEVICE_STOPPED;
                    Event event(eventType);

                    state.setMotionStreak(newState);
                    state.setMotionTime(std::nullopt);
                    state.setMotionDistance(0.0);
                    state.setEvent(event);
                }
            }
        } else {
            state.setMotionState(newState);
            if (state.getMotionStreak() == newState) {
                state.setMotionTime(std::nullopt);
                state.setMotionDistance(0.0);
            } else {
                state.setMotionTime(position.getFixTime());
                state.setMotionDistance(position.getTotalDistance());
            }
        }
    }
};

int main() {
    TripsConfig config(true, 600, 1000.0, 300);
    MotionState state;

    auto now = std::chrono::system_clock::now();
    Position position(now, 1500.0, true);

    MotionProcessor::updateState(state, position, true, config);

    if (state.getEvent().has_value()) {
        std::cout << "Generated event: " << state.getEvent()->getType() << std::endl;
    } else {
        std::cout << "No event generated." << std::endl;
    }

    return 0;
}