#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <stdexcept>

class Condition {
public:
    // Simulación de condiciones de permisos
    static bool checkPermission(long userId, long objectId) {
        // Simula la validación de permisos
        return userId == 1 || objectId % 2 == 0; // Ejemplo: acceso para admin o objetos pares
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

class BaseObject {
protected:
    long id;
    std::string name;

public:
    BaseObject(long id, const std::string& name) : id(id), name(name) {}

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
class ExtendedObjectResource {
private:
    std::string sortField;
    Storage storage;
    long userId;

public:
    ExtendedObjectResource(long userId, const std::string& sortField) : userId(userId), sortField(sortField) {}

    std::vector<T> get(bool all, long filterUserId, long groupId, long deviceId) {
        std::vector<T> results;

        if (all) {
            if (!Condition::checkPermission(userId, 0)) {
                throw std::runtime_error("Admin permissions required for 'all' access");
            }
        }

        // Filtrar por grupo
        if (groupId > 0) {
            if (!Condition::checkPermission(userId, groupId)) {
                throw std::runtime_error("Permission denied for group ID: " + std::to_string(groupId));
            }
        }

        // Filtrar por dispositivo
        if (deviceId > 0) {
            if (!Condition::checkPermission(userId, deviceId)) {
                throw std::runtime_error("Permission denied for device ID: " + std::to_string(deviceId));
            }
        }

        results = storage.getObjects<T>(sortField);
        return results;
    }
};

class Device : public BaseObject {
public:
    Device(long id, const std::string& name) : BaseObject(id, name) {}
};

int main() {
    try {
        ExtendedObjectResource<Device> resource(1, "name"); // Usuario con permisos admin

        auto results = resource.get(true, 0, 0, 0);

        for (const auto& obj : results) {
            std::cout << "ID: " << obj.getId() << ", Name: " << obj.getField("name") << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
