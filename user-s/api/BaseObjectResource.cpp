#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <stdexcept>

// Simulaciones de dependencias externas
class StorageException : public std::runtime_error {
public:
    explicit StorageException(const std::string& message) : std::runtime_error(message) {}
};

class PermissionsService {
public:
    template <typename T>
    void checkPermission(long userId, long objectId) {
        // Simula la validación de permisos
        if (userId != 1) {
            throw std::runtime_error("Permission denied");
        }
    }

    template <typename T>
    void checkEdit(long userId, bool isNew, bool skipReadonly) {
        // Simula la validación de permisos de edición
        if (userId != 1) {
            throw std::runtime_error("Edit permission denied");
        }
    }
};

class CacheManager {
public:
    template <typename T>
    void invalidateObject(long objectId, const std::string& operation) {
        std::cout << "Cache invalidated for object ID: " << objectId << ", operation: " << operation << std::endl;
    }
};

// Clase base genérica
template <typename T>
class BaseObjectResource {
private:
    PermissionsService permissionsService;
    CacheManager cacheManager;
    std::unordered_map<long, T> storage;

public:
    T getSingle(long userId, long id) {
        permissionsService.checkPermission<T>(userId, id);
        if (storage.find(id) != storage.end()) {
            return storage[id];
        } else {
            throw StorageException("Object not found");
        }
    }

    void add(long userId, const T& entity) {
        permissionsService.checkEdit<T>(userId, true, false);
        long id = storage.size() + 1;
        T newEntity = entity;
        newEntity.setId(id);
        storage[id] = newEntity;
        cacheManager.invalidateObject<T>(id, "CREATE");
        std::cout << "Object created with ID: " << id << std::endl;
    }

    void update(long userId, const T& entity) {
        permissionsService.checkPermission<T>(userId, entity.getId());
        permissionsService.checkEdit<T>(userId, false, false);
        if (storage.find(entity.getId()) != storage.end()) {
            storage[entity.getId()] = entity;
            cacheManager.invalidateObject<T>(entity.getId(), "UPDATE");
            std::cout << "Object updated with ID: " << entity.getId() << std::endl;
        } else {
            throw StorageException("Object not found");
        }
    }

    void remove(long userId, long id) {
        permissionsService.checkPermission<T>(userId, id);
        permissionsService.checkEdit<T>(userId, false, false);
        if (storage.find(id) != storage.end()) {
            storage.erase(id);
            cacheManager.invalidateObject<T>(id, "DELETE");
            std::cout << "Object removed with ID: " << id << std::endl;
        } else {
            throw StorageException("Object not found");
        }
    }
};

// Ejemplo de modelo
class BaseModel {
private:
    long id;

public:
    long getId() const {
        return id;
    }

    void setId(long newId) {
        id = newId;
    }
};

int main() {
    try {
        BaseObjectResource<BaseModel> resource;
        BaseModel obj;

        // Agregar un objeto
        resource.add(1, obj);

        // Obtener un objeto
        auto fetched = resource.getSingle(1, 1);
        std::cout << "Fetched object ID: " << fetched.getId() << std::endl;

        // Actualizar un objeto
        resource.update(1, fetched);

        // Eliminar un objeto
        resource.remove(1, 1);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}