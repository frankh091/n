#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>

class Logger {
public:
    void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }

    void error(const std::string& message) {
        std::cerr << "ERROR: " << message << std::endl;
    }
};

class Group {
public:
    long id;
    std::string name;

    Group(long id, const std::string& name) : id(id), name(name) {}
};

class Storage {
private:
    std::map<long, Group> database;

public:
    void addGroup(const Group& group) {
        database[group.id] = group;
    }

    Group getGroup(long id) {
        if (database.find(id) != database.end()) {
            return database[id];
        } else {
            throw std::runtime_error("Group not found with ID: " + std::to_string(id));
        }
    }

    std::vector<Group> getAllGroups() {
        std::vector<Group> result;
        for (const auto& entry : database) {
            result.push_back(entry.second);
        }
        return result;
    }
};

class GroupResource {
private:
    Logger logger;
    Storage storage;

public:
    GroupResource() {
        logger.info("GroupResource initialized.");
    }

    void addGroup(long id, const std::string& name) {
        try {
            storage.addGroup(Group(id, name));
            logger.info("Group added with ID: " + std::to_string(id));
        } catch (const std::exception& e) {
            logger.error("Error adding group: " + std::string(e.what()));
        }
    }

    void getGroup(long id) {
        try {
            Group group = storage.getGroup(id);
            logger.info("Group fetched with ID: " + std::to_string(id));
            std::cout << "Group Name: " << group.name << std::endl;
        } catch (const std::exception& e) {
            logger.error("Error fetching group: " + std::string(e.what()));
        }
    }

    void listGroups() {
        try {
            auto groups = storage.getAllGroups();
            logger.info("Listing all groups.");
            for (const auto& group : groups) {
                std::cout << "Group ID: " << group.id << ", Name: " << group.name << std::endl;
            }
        } catch (const std::exception& e) {
            logger.error("Error listing groups: " + std::string(e.what()));
        }
    }
};

int main() {
    GroupResource resource;

    // Simula operaciones CRUD sobre grupos
    resource.addGroup(1, "Admin Group");
    resource.addGroup(2, "User Group");

    resource.getGroup(1);

    resource.listGroups();

    return 0;
}
