#include "crow.h"
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <string>

class UserController {
private:
    std::unordered_map<std::string, std::string> users; // Simulating a database

public:
    std::string createUser(const std::string& username, const std::string& password) {
        if (users.find(username) != users.end()) {
            throw std::runtime_error("User already exists.");
        }
        users[username] = password;
        return "User created: " + username;
    }

    bool loginUser(const std::string& username, const std::string& password) {
        auto it = users.find(username);
        if (it != users.end() && it->second == password) {
            return true;
        }
        return false;
    }

    std::optional<std::string> getUser(const std::string& username) const {
        auto it = users.find(username);
        if (it != users.end()) {
            return it->second;
        }
        return std::nullopt;
    }
};

int main() {
    crow::SimpleApp app;
    UserController userController;

    // Endpoint to create a user
    CROW_ROUTE(app, "/users").methods(crow::HTTPMethod::Post)([&userController](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password")) {
            return crow::response(400, "Invalid JSON or missing fields");
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        try {
            std::string result = userController.createUser(username, password);
            return crow::response(201, result);
        } catch (const std::exception& e) {
            return crow::response(400, e.what());
        }
    });

    // Endpoint to log in a user
    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::Post)([&userController](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password")) {
            return crow::response(400, "Invalid JSON or missing fields");
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        if (userController.loginUser(username, password)) {
            return crow::response(200, "Login successful");
        }
        return crow::response(401, "Invalid credentials");
    });

    // Endpoint to get user details
    CROW_ROUTE(app, "/users/<string>").methods(crow::HTTPMethod::Get)([&userController](const crow::request&, const std::string& username) {
        auto user = userController.getUser(username);
        if (user.has_value()) {
            crow::json::wvalue result;
            result["username"] = username;
            result["password"] = user.value(); // Simulated data; in real applications, don't return passwords
            return crow::response(result);
        }
        return crow::response(404, "User not found");
    });

    app.port(18080).multithreaded().run();
}
