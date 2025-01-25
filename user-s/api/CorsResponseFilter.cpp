#include <iostream>
#include <string>
#include <map>
#include <set>
#include <optional>

class Config {
public:
    std::optional<std::string> getWebOrigin() const {
        return "*"; // Simula la configuración para orígenes permitidos
    }
};

class CorsResponseFilter {
private:
    std::string allowed;

    const std::string ORIGIN_ALL = "*";
    const std::string HEADERS_ALL = "origin, content-type, accept, authorization";
    const std::string METHODS_ALL = "GET, POST, PUT, DELETE, OPTIONS";

public:
    CorsResponseFilter(const Config& config) {
        auto origin = config.getWebOrigin();
        allowed = origin.value_or(ORIGIN_ALL);
    }

    void filter(const std::map<std::string, std::string>& requestHeaders, std::map<std::string, std::string>& responseHeaders) {
        if (responseHeaders.find("Access-Control-Allow-Headers") == responseHeaders.end()) {
            responseHeaders["Access-Control-Allow-Headers"] = HEADERS_ALL;
        }

        if (responseHeaders.find("Access-Control-Allow-Credentials") == responseHeaders.end()) {
            responseHeaders["Access-Control-Allow-Credentials"] = "true";
        }

        if (responseHeaders.find("Access-Control-Allow-Methods") == responseHeaders.end()) {
            responseHeaders["Access-Control-Allow-Methods"] = METHODS_ALL;
        }

        if (responseHeaders.find("Access-Control-Allow-Origin") == responseHeaders.end()) {
            auto originIt = requestHeaders.find("Origin");
            if (originIt == requestHeaders.end()) {
                responseHeaders["Access-Control-Allow-Origin"] = ORIGIN_ALL;
            } else {
                const std::string& origin = originIt->second;
                if (allowed == ORIGIN_ALL || allowed.find(origin) != std::string::npos) {
                    responseHeaders["Access-Control-Allow-Origin"] = origin;
                }
            }
        }
    }
};

int main() {
    Config config;
    CorsResponseFilter corsFilter(config);

    // Simulación de una solicitud HTTP
    std::map<std::string, std::string> requestHeaders = {
        {"Origin", "http://example.com"}
    };

    std::map<std::string, std::string> responseHeaders;

    // Aplicar el filtro CORS
    corsFilter.filter(requestHeaders, responseHeaders);

    // Imprimir encabezados de respuesta
    for (const auto& header : responseHeaders) {
        std::cout << header.first << ": " << header.second << std::endl;
    }

    return 0;
}
