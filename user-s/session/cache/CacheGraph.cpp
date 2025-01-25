#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <stdexcept>

// Clase genérica para representar un nodo en el grafo
class Node {
public:
    long id;
    std::string data;

    Node(long id, const std::string& data) : id(id), data(data) {}
};

class CacheGraph {
private:
    std::unordered_map<long, Node> nodes;
    std::unordered_map<long, std::unordered_set<long>> links;

public:
    // Agregar un nodo al grafo
    void addNode(long id, const std::string& data) {
        if (nodes.find(id) != nodes.end()) {
            throw std::runtime_error("Node with ID " + std::to_string(id) + " already exists.");
        }
        nodes.emplace(id, Node(id, data));
    }

    // Eliminar un nodo del grafo
    void removeNode(long id) {
        if (nodes.erase(id) == 0) {
            throw std::runtime_error("Node with ID " + std::to_string(id) + " not found.");
        }
        links.erase(id);
        for (auto& [_, neighbors] : links) {
            neighbors.erase(id);
        }
    }

    // Agregar un enlace entre nodos
    void addLink(long fromId, long toId) {
        if (nodes.find(fromId) == nodes.end() || nodes.find(toId) == nodes.end()) {
            throw std::runtime_error("Both nodes must exist to create a link.");
        }
        links[fromId].insert(toId);
    }

    // Eliminar un enlace entre nodos
    void removeLink(long fromId, long toId) {
        if (links.find(fromId) == links.end() || links[fromId].erase(toId) == 0) {
            throw std::runtime_error("Link does not exist.");
        }
    }

    // Obtener un nodo por ID
    Node getNode(long id) const {
        auto it = nodes.find(id);
        if (it == nodes.end()) {
            throw std::runtime_error("Node with ID " + std::to_string(id) + " not found.");
        }
        return it->second;
    }

    // Obtener todos los nodos conectados a un nodo dado
    std::vector<Node> getLinkedNodes(long id) const {
        std::vector<Node> result;
        auto it = links.find(id);
        if (it != links.end()) {
            for (long neighborId : it->second) {
                result.push_back(nodes.at(neighborId));
            }
        }
        return result;
    }

    // Recorrer el grafo desde un nodo dado
    std::vector<Node> traverseFrom(long rootId) const {
        std::vector<Node> result;
        std::unordered_set<long> visited;
        std::queue<long> toVisit;

        toVisit.push(rootId);
        while (!toVisit.empty()) {
            long currentId = toVisit.front();
            toVisit.pop();

            if (visited.count(currentId) > 0) {
                continue;
            }

            visited.insert(currentId);
            result.push_back(nodes.at(currentId));

            auto it = links.find(currentId);
            if (it != links.end()) {
                for (long neighborId : it->second) {
                    if (visited.count(neighborId) == 0) {
                        toVisit.push(neighborId);
                    }
                }
            }
        }

        return result;
    }
};

int main() {
    try {
        CacheGraph graph;

        // Agregar nodos
        graph.addNode(1, "Node 1");
        graph.addNode(2, "Node 2");
        graph.addNode(3, "Node 3");

        // Crear enlaces
        graph.addLink(1, 2);
        graph.addLink(2, 3);

        // Obtener nodos conectados
        auto linkedNodes = graph.getLinkedNodes(1);
        std::cout << "Nodes linked to 1:" << std::endl;
        for (const auto& node : linkedNodes) {
            std::cout << "ID: " << node.id << ", Data: " << node.data << std::endl;
        }

        // Recorrer el grafo desde el nodo 1
        auto traversal = graph.traverseFrom(1);
        std::cout << "Traversal from 1:" << std::endl;
        for (const auto& node : traversal) {
            std::cout << "ID: " << node.id << ", Data: " << node.data << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
