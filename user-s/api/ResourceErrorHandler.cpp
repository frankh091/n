#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;

class Log {
public:
    static std::string exceptionStack(const std::exception& e) {
        std::ostringstream oss;
        oss << "Exception: " << e.what();
        return oss.str();
    }
};

class WebApplicationException : public std::runtime_error {
private:
    http::response<http::string_body> response;

public:
    explicit WebApplicationException(const std::string& message, http::status status)
        : std::runtime_error(message), response(status, 11) {
        response.body() = message;
        response.prepare_payload();
    }

    http::response<http::string_body> getResponse() const {
        return response;
    }
};

class ResourceErrorHandler {
public:
    http::response<http::string_body> toResponse(const std::exception& e) {
        if (const auto* webException = dynamic_cast<const WebApplicationException*>(&e)) {
            auto response = webException->getResponse();
            response.body() += "\n" + Log::exceptionStack(*webException);
            response.prepare_payload();
            return response;
        } else {
            http::response<http::string_body> response(http::status::bad_request, 11);
            response.body() = Log::exceptionStack(e);
            response.prepare_payload();
            return response;
        }
    }
};

int main() {
    ResourceErrorHandler errorHandler;

    try {
        throw WebApplicationException("Not Found", http::status::not_found);
    } catch (const std::exception& e) {
        auto response = errorHandler.toResponse(e);
        std::cout << "Response: " << response.result_int() << " - " << response.body() << std::endl;
    }

    try {
        throw std::runtime_error("General error occurred");
    } catch (const std::exception& e) {
        auto response = errorHandler.toResponse(e);
        std::cout << "Response: " << response.result_int() << " - " << response.body() << std::endl;
    }

    return 0;
}
