#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>

using json = nlohmann::json;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;

class AsyncSocket {
private:
    websocket::stream<beast::tcp_stream> ws;
    long userId;
    bool includeLogs;

    void sendData(const json& data) {
        try {
            ws.text(true);
            ws.write(net::buffer(data.dump()));
        } catch (const std::exception& e) {
            std::cerr << "Error sending data: " << e.what() << std::endl;
        }
    }

public:
    AsyncSocket(net::ip::tcp::socket&& socket, long userId)
        : ws(std::move(socket)), userId(userId), includeLogs(false) {}

    void onWebSocketConnect() {
        json initialData;
        initialData["positions"] = { { "id", 1 }, { "latitude", 40.7128 }, { "longitude", -74.0060 } };
        sendData(initialData);
    }

    void onWebSocketClose() {
        std::cout << "WebSocket closed for user: " << userId << std::endl;
    }

    void onWebSocketText(const std::string& message) {
        try {
            auto parsed = json::parse(message);
            if (parsed.contains("logs")) {
                includeLogs = parsed["logs"].get<bool>();
            }
        } catch (const json::exception& e) {
            std::cerr << "JSON parsing error: " << e.what() << std::endl;
        }
    }

    void onUpdateDevice(const json& device) {
        sendData({ { "devices", { device } } });
    }

    void onUpdatePosition(const json& position) {
        sendData({ { "positions", { position } } });
    }

    void onUpdateEvent(const json& event) {
        sendData({ { "events", { event } } });
    }

    void onUpdateLog(const json& log) {
        if (includeLogs) {
            sendData({ { "logs", { log } } });
        }
    }
};

int main() {
    try {
        net::io_context ioc;
        net::ip::tcp::endpoint endpoint(net::ip::tcp::v4(), 8080);
        net::ip::tcp::acceptor acceptor(ioc, endpoint);

        std::cout << "WebSocket server running on port 8080..." << std::endl;

        acceptor.async_accept([
            &ioc
        ](boost::system::error_code ec, net::ip::tcp::socket socket) {
            if (!ec) {
                std::make_shared<AsyncSocket>(std::move(socket), 12345)->onWebSocketConnect();
            } else {
                std::cerr << "Accept error: " << ec.message() << std::endl;
            }
        });

        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
