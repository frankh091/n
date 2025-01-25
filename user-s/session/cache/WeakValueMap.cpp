#include <iostream>
#include <unordered_map>
#include <memory>
#include <vector>
#include <algorithm>

template <typename K, typename V>
class WeakValueMap {
private:
    std::unordered_map<K, std::weak_ptr<V>> map;

public:
    // Agregar un valor al mapa
    void put(const K& key, const std::shared_ptr<V>& value) {
        map[key] = value;
    }

    // Obtener un valor del mapa
    std::shared_ptr<V> get(const K& key) {
        auto it = map.find(key);
        if (it != map.end()) {
            return it->second.lock();
        }
        return nullptr;
    }

    // Eliminar un valor del mapa
    std::shared_ptr<V> remove(const K& key) {
        auto it = map.find(key);
        if (it != map.end()) {
            auto value = it->second.lock();
            map.erase(it);
            return value;
        }
        return nullptr;
    }

    // Limpiar referencias nulas
    void clean() {
        for (auto it = map.begin(); it != map.end(); ) {
            if (it->second.expired()) {
                it = map.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Mostrar el contenido del mapa (para depuración)
    void print() {
        std::cout << "Map contents:" << std::endl;
        for (const auto& pair : map) {
            auto value = pair.second.lock();
            if (value) {
                std::cout << "Key: " << pair.first << " -> Value: " << *value << std::endl;
            } else {
                std::cout << "Key: " << pair.first << " -> Value: null" << std::endl;
            }
        }
    }
};

int main() {
    WeakValueMap<int, int> weakMap;

    auto value1 = std::make_shared<int>(10);
    auto value2 = std::make_shared<int>(20);

    weakMap.put(1, value1);
    weakMap.put(2, value2);

    weakMap.print();

    value1.reset(); // Liberar la referencia fuerte a value1

    weakMap.clean(); // Limpiar referencias nulas

    weakMap.print();

    return 0;
}
