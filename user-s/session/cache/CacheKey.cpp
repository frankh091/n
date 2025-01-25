#include <iostream>
#include <string>
#include <typeinfo>
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

class CacheKey {
private:
    std::string className;
    long id;

public:
    CacheKey(const std::string& className, long id) : className(className), id(id) {}

    CacheKey(const BaseModel& object) : className(object.getClassName()), id(object.getId()) {}

    bool operator==(const CacheKey& other) const {
        return className == other.className && id == other.id;
    }

    std::size_t hash() const {
        return std::hash<std::string>{}(className) ^ std::hash<long>{}(id);
    }

    friend std::ostream& operator<<(std::ostream& os, const CacheKey& key) {
        os << "CacheKey(Class: " << key.className << ", ID: " << key.id << ")";
        return os;
    }
};

class Device : public BaseModel {
public:
    Device(long id) : BaseModel(id) {}

    std::string getClassName() const override {
        return "Device";
    }
};

int main() {
    Device device(42);
    CacheKey key(device);

    std::cout << key << std::endl;

    return 0;
}
