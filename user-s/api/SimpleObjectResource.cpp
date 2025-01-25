#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <stdexcept>

class Condition {
public:
    static bool checkPermission(long userId, long objectId) {
        // Simula la validación de permisos
        return userId == 1 || objectId % 2 == 0;
    }
};

class Storage {
public:
    template <typename T>
    std::vector<T> getObjects(const std::string& sortField) {
        std::vector<T> objects = {T(1, "Object A"), T(2, "Object B"), T(3, "Object C")};

        if (!sortField.empty()) {
            std::sort(objects.begin(), objects.end(), [&](const T& a, const T& b) {
                return a.getField(sortField) < b.getField(sortField);
            });
        }

        return objects;
    }
};

class BaseModel {
protected:
    long id;
    std::string name;

public:
    BaseModel(long id, const std::string& name) : id(id), name(name) {}

    long getId() const {
        return id;
    }

    std::string getField(const std::string& field) const {
        if (field == "name") {
            return name;
        }
        throw std::runtime_error("Field not found");
    }
};

template <typename T>
class SimpleObjectResource {
private:
    std::string sortField;
    Storage storage;
    long userId;

public:
    SimpleObjectResource(long userId, const std::string& sortField) : userId(userId), sortField(sortField) {}

    std::vector<T> get(bool all, long filterUserId) {
        std::vector<T> results;

        if (all) {
            if (!Condition::checkPermission(userId, 0)) {
                throw std::runtime_error("Admin permissions required for 'all' access");
            }
        } else {
            if (filterUserId == 0) {
                filterUserId = userId;
            } else if (!Condition::checkPermission(userId, filterUserId)) {
                throw std::runtime_error("Permission denied for user ID: " + std::to_string(filterUserId));
            }
        }

        results = storage.getObjects<T>(sortField);
        return results;
    }
};

class Device : public BaseModel {
public:
    Device(long id, const std::string& name) : BaseModel(id, name) {}
};

int main() {
    try {
        SimpleObjectResource<Device> resource(1, "name"); // Usuario admin con ID 1

        auto results = resource.get(true, 0);

        for (const auto& obj : results) {
            std::cout << "ID: " << obj.getId() << ", Name: " << obj.getField("name") << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
