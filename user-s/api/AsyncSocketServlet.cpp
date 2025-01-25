#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;

class LoginService {
public:
    std::optional<long> login(const std::string& token) {
        if (token == "valid_token") {
            return 12345; // Simulación del ID de usuario autenticado
        }
        return std::nullopt;
    }
};

class AsyncSocket {
private:
    websocket::stream<beast::tcp_stream> ws;
    long userId;

public:
    AsyncSocket(net::ip::tcp::socket&& socket, long userId)
        : ws(std::move(socket)), userId(userId) {}

    void start() {
        std::cout << "WebSocket started for user ID: " << userId << std::endl;
    }
};

class AsyncSocketServlet {
private:
    LoginService loginService;

public:
    void handleConnection(net::ip::tcp::socket&& socket, const std::map<std::string, std::string>& params) {
        std::optional<long> userId;

        auto tokenIt = params.find("token");
        if (tokenIt != params.end()) {
            userId = loginService.login(tokenIt->second);
        }

        if (userId.has_value()) {
            auto asyncSocket = std::make_shared<AsyncSocket>(std::move(socket), userId.value());
            asyncSocket->start();
        } else {
            std::cerr << "Authentication failed. Connection rejected." << std::endl;
        }
    }
};

int main() {
    try {
        net::io_context ioc;
        net::ip::tcp::endpoint endpoint(net::ip::tcp::v4(), 8080);
        net::ip::tcp::acceptor acceptor(ioc, endpoint);

        AsyncSocketServlet servlet;

        acceptor.async_accept([
            &servlet
        ](boost::system::error_code ec, net::ip::tcp::socket socket) {
            if (!ec) {
                std::map<std::string, std::string> params = { {"token", "valid_token"} };
                servlet.handleConnection(std::move(socket), params);
            } else {
                std::cerr << "Accept error: " << ec.message() << std::endl;
            }
        });

        std::cout << "WebSocket server running on port 8080..." << std::endl;
        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
