#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <vector>

class Logger {
public:
    void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }

    void error(const std::string& message) {
        std::cerr << "ERROR: " << message << std::endl;
    }
};

class Storage {
private:
    std::map<long, std::map<std::string, std::string>> database;

public:
    void add(long id, const std::map<std::string, std::string>& calendar) {
        database[id] = calendar;
    }

    std::map<std::string, std::string> get(long id) {
        if (database.find(id) != database.end()) {
            return database[id];
        } else {
            throw std::runtime_error("Calendar not found with ID: " + std::to_string(id));
        }
    }

    void update(long id, const std::map<std::string, std::string>& calendar) {
        if (database.find(id) != database.end()) {
            database[id] = calendar;
        } else {
            throw std::runtime_error("Cannot update non-existing calendar with ID: " + std::to_string(id));
        }
    }

    void remove(long id) {
        if (database.find(id) != database.end()) {
            database.erase(id);
        } else {
            throw std::runtime_error("Cannot remove non-existing calendar with ID: " + std::to_string(id));
        }
    }

    std::vector<std::map<std::string, std::string>> getAll() {
        std::vector<std::map<std::string, std::string>> result;
        for (const auto& entry : database) {
            result.push_back(entry.second);
        }
        return result;
    }
};

class CalendarResource {
private:
    Logger logger;
    Storage storage;

public:
    CalendarResource() {
        logger.info("CalendarResource initialized.");
    }

    void addCalendar(long id, const std::map<std::string, std::string>& calendar) {
        try {
            storage.add(id, calendar);
            logger.info("Calendar added with ID: " + std::to_string(id));
        } catch (const std::exception& e) {
            logger.error("Error adding calendar: " + std::string(e.what()));
        }
    }

    void getCalendar(long id) {
        try {
            auto calendar = storage.get(id);
            logger.info("Calendar fetched for ID: " + std::to_string(id));
            for (const auto& [key, value] : calendar) {
                std::cout << key << ": " << value << std::endl;
            }
        } catch (const std::exception& e) {
            logger.error("Error fetching calendar: " + std::string(e.what()));
        }
    }

    void updateCalendar(long id, const std::map<std::string, std::string>& calendar) {
        try {
            storage.update(id, calendar);
            logger.info("Calendar updated for ID: " + std::to_string(id));
        } catch (const std::exception& e) {
            logger.error("Error updating calendar: " + std::string(e.what()));
        }
    }

    void removeCalendar(long id) {
        try {
            storage.remove(id);
            logger.info("Calendar removed with ID: " + std::to_string(id));
        } catch (const std::exception& e) {
            logger.error("Error removing calendar: " + std::string(e.what()));
        }
    }

    void listCalendars() {
        try {
            auto calendars = storage.getAll();
            logger.info("Listing all calendars.");
            for (const auto& calendar : calendars) {
                for (const auto& [key, value] : calendar) {
                    std::cout << key << ": " << value << std::endl;
                }
                std::cout << "-------------------" << std::endl;
            }
        } catch (const std::exception& e) {
            logger.error("Error listing calendars: " + std::string(e.what()));
        }
    }
};

int main() {
    CalendarResource resource;

    // Simula operaciones CRUD sobre calendarios
    resource.addCalendar(1, {{"name", "Work Calendar"}, {"description", "Events for work."}});
    resource.getCalendar(1);
    resource.updateCalendar(1, {{"name", "Updated Work Calendar"}});
    resource.listCalendars();
    resource.removeCalendar(1);

    return 0;
}
