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

class Order {
public:
    long id;
    std::string description;

    Order(long id, const std::string& description) : id(id), description(description) {}
};

class Storage {
private:
    std::map<long, Order> database;

public:
    void addOrder(const Order& order) {
        database[order.id] = order;
    }

    Order getOrder(long id) {
        if (database.find(id) != database.end()) {
            return database[id];
        } else {
            throw std::runtime_error("Order not found with ID: " + std::to_string(id));
        }
    }

    std::vector<Order> getAllOrders() {
        std::vector<Order> result;
        for (const auto& entry : database) {
            result.push_back(entry.second);
        }
        return result;
    }
};

class OrderResource {
private:
    Logger logger;
    Storage storage;

public:
    OrderResource() {
        logger.info("OrderResource initialized.");
    }

    void addOrder(long id, const std::string& description) {
        try {
            storage.addOrder(Order(id, description));
            logger.info("Order added with ID: " + std::to_string(id));
        } catch (const std::exception& e) {
            logger.error("Error adding order: " + std::string(e.what()));
        }
    }

    void getOrder(long id) {
        try {
            Order order = storage.getOrder(id);
            logger.info("Order fetched with ID: " + std::to_string(id));
            std::cout << "Order Description: " << order.description << std::endl;
        } catch (const std::exception& e) {
            logger.error("Error fetching order: " + std::string(e.what()));
        }
    }

    void listOrders() {
        try {
            auto orders = storage.getAllOrders();
            logger.info("Listing all orders.");
            for (const auto& order : orders) {
                std::cout << "Order ID: " << order.id << ", Description: " << order.description << std::endl;
            }
        } catch (const std::exception& e) {
            logger.error("Error listing orders: " + std::string(e.what()));
        }
    }
};

int main() {
    OrderResource resource;

    // Simula operaciones CRUD sobre órdenes
    resource.addOrder(1, "First Order");
    resource.addOrder(2, "Second Order");

    resource.getOrder(1);

    resource.listOrders();

    return 0;
}
