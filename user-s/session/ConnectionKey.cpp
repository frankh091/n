#include <iostream>
#include <utility>
#include <string>

class SocketAddress {
private:
    std::string address;
    int port;

public:
    SocketAddress(const std::string& address, int port) : address(address), port(port) {}

    std::string getAddress() const {
        return address;
    }

    int getPort() const {
        return port;
    }

    std::string toString() const {
        return address + ":" + std::to_string(port);
    }
};

class ConnectionKey {
private:
    SocketAddress localAddress;
    SocketAddress remoteAddress;

public:
    ConnectionKey(const SocketAddress& localAddress, const SocketAddress& remoteAddress)
        : localAddress(localAddress), remoteAddress(remoteAddress) {}

    ConnectionKey(const std::pair<SocketAddress, SocketAddress>& channel)
        : localAddress(channel.first), remoteAddress(channel.second) {}

    SocketAddress getLocalAddress() const {
        return localAddress;
    }

    SocketAddress getRemoteAddress() const {
        return remoteAddress;
    }

    std::string toString() const {
        return "Local: " + localAddress.toString() + ", Remote: " + remoteAddress.toString();
    }
};

int main() {
    SocketAddress local("192.168.1.1", 8080);
    SocketAddress remote("192.168.1.2", 9090);

    ConnectionKey connection(local, remote);

    std::cout << connection.toString() << std::endl;

    return 0;
}
