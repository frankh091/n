#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>
#include <string>
#include <typeindex>

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

class CacheNode {
private:
    std::shared_ptr<BaseModel> value;
    std::unordered_map<std::type_index, std::unordered_set<std::shared_ptr<CacheNode>>> links;
    std::unordered_map<std::type_index, std::unordered_set<std::shared_ptr<CacheNode>>> backlinks;

public:
    explicit CacheNode(std::shared_ptr<BaseModel> value) : value(std::move(value)) {}

    std::shared_ptr<BaseModel> getValue() const {
        return value;
    }

    void setValue(std::shared_ptr<BaseModel> newValue) {
        value = std::move(newValue);
    }

    std::unordered_set<std::shared_ptr<CacheNode>>& getLinks(std::type_index clazz, bool forward) {
        auto& map = forward ? links : backlinks;
        return map[clazz];
    }

    std::vector<std::shared_ptr<CacheNode>> getAllLinks(bool forward) const {
        const auto& map = forward ? links : backlinks;
        std::vector<std::shared_ptr<CacheNode>> result;

        for (const auto& [_, nodes] : map) {
            result.insert(result.end(), nodes.begin(), nodes.end());
        }

        return result;
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
    auto device1 = std::make_shared<Device>(1);
    auto device2 = std::make_shared<Device>(2);
    auto device3 = std::make_shared<Device>(3);

    auto node1 = std::make_shared<CacheNode>(device1);
    auto node2 = std::make_shared<CacheNode>(device2);
    auto node3 = std::make_shared<CacheNode>(device3);

    // Crear enlaces
    node1->getLinks(typeid(Device), true).insert(node2);
    node1->getLinks(typeid(Device), true).insert(node3);

    // Obtener enlaces
    auto links = node1->getAllLinks(true);
    std::cout << "Node 1 links:" << std::endl;
    for (const auto& link : links) {
        std::cout << "Linked to Device ID: " << link->getValue()->getId() << std::endl;
    }

    return 0;
}
