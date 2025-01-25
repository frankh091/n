#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <functional>

class BaseModel {
protected:
    long id;

public:
    BaseModel(long id) : id(id) {}
    virtual ~BaseModel() = default;

    long getId() const {
        return id;
    }

    virtual std::string getClassName() const = 0;
};

class CacheManager {
private:
    std::unordered_map<long, std::shared_ptr<BaseModel>> cache;
    std::mutex cacheMutex;

public:
    // Agregar un objeto al caché
    void addObject(const std::shared_ptr<BaseModel>& object) {
        std::lock_guard<std::mutex> lock(cacheMutex);
        if (cache.find(object->getId()) != cache.end()) {
            throw std::runtime_error("Object with ID " + std::to_string(object->getId()) + " already exists in cache.");
        }
        cache[object->getId()] = object;
        std::cout << "Added object with ID: " << object->getId() << std::endl;
    }

    // Obtener un objeto del caché
    std::shared_ptr<BaseModel> getObject(long id) {
        std::lock_guard<std::mutex> lock(cacheMutex);
        auto it = cache.find(id);
        if (it != cache.end()) {
            return it->second;
        }
        throw std::runtime_error("Object with ID " + std::to_string(id) + " not found in cache.");
    }

    // Eliminar un objeto del caché
    void removeObject(long id) {
        std::lock_guard<std::mutex> lock(cacheMutex);
        if (cache.erase(id) == 0) {
            throw std::runtime_error("Object with ID " + std::to_string(id) + " not found in cache.");
        }
        std::cout << "Removed object with ID: " << id << std::endl;
    }

    // Actualizar un objeto en el caché
    void updateObject(const std::shared_ptr<BaseModel>& object) {
        std::lock_guard<std::mutex> lock(cacheMutex);
        auto it = cache.find(object->getId());
        if (it != cache.end()) {
            it->second = object;
            std::cout << "Updated object with ID: " << object->getId() << std::endl;
        } else {
            throw std::runtime_error("Object with ID " + std::to_string(object->getId()) + " not found in cache.");
        }
    }

    // Obtener todos los objetos del caché
    std::vector<std::shared_ptr<BaseModel>> getAllObjects() {
        std::lock_guard<std::mutex> lock(cacheMutex);
        std::vector<std::shared_ptr<BaseModel>> objects;
        for (const auto& entry : cache) {
            objects.push_back(entry.second);
        }
        return objects;
    }
};

class Device : public BaseModel {
private:
    std::string name;

public:
    Device(long id, const std::string& name) : BaseModel(id), name(name) {}

    std::string getClassName() const override {
        return "Device";
    }

    std::string getName() const {
        return name;
    }
};

int main() {
    try {
        CacheManager cacheManager;

        // Crear y agregar dispositivos
        auto device1 = std::make_shared<Device>(1, "Device A");
        auto device2 = std::make_shared<Device>(2, "Device B");

        cacheManager.addObject(device1);
        cacheManager.addObject(device2);

        // Obtener un dispositivo
        auto retrievedDevice = cacheManager.getObject(1);
        std::cout << "Retrieved: " << retrievedDevice->getClassName() << " ID: " << retrievedDevice->getId() << std::endl;

        // Actualizar un dispositivo
        auto updatedDevice = std::make_shared<Device>(1, "Updated Device A");
        cacheManager.updateObject(updatedDevice);

        // Eliminar un dispositivo
        cacheManager.removeObject(2);

        // Obtener todos los dispositivos restantes
        auto allObjects = cacheManager.getAllObjects();
        std::cout << "Remaining objects in cache:" << std::endl;
        for (const auto& obj : allObjects) {
            std::cout << "ID: " << obj->getId() << " - " << obj->getClassName() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
