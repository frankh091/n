#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <stdexcept>
#include <memory>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

class TokenManager {
private:
    static constexpr int DEFAULT_EXPIRATION_DAYS = 7;

    class TokenData {
    public:
        long userId;
        std::time_t expiration;

        TokenData(long userId, std::time_t expiration)
            : userId(userId), expiration(expiration) {}
    };

    std::string base64Encode(const std::vector<unsigned char>& data) {
        BIO* bio = BIO_new(BIO_s_mem());
        BIO* b64 = BIO_new(BIO_f_base64());
        bio = BIO_push(b64, bio);

        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
        BIO_write(bio, data.data(), data.size());
        BIO_flush(bio);

        BUF_MEM* buffer;
        BIO_get_mem_ptr(bio, &buffer);
        std::string encoded(buffer->data, buffer->length);

        BIO_free_all(bio);
        return encoded;
    }

    std::vector<unsigned char> base64Decode(const std::string& encoded) {
        BIO* bio = BIO_new_mem_buf(encoded.data(), encoded.size());
        BIO* b64 = BIO_new(BIO_f_base64());
        bio = BIO_push(b64, bio);

        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
        std::vector<unsigned char> decoded(encoded.size());
        int decodedLength = BIO_read(bio, decoded.data(), decoded.size());

        BIO_free_all(bio);
        decoded.resize(decodedLength);
        return decoded;
    }

public:
    std::string generateToken(long userId, std::time_t expiration = 0) {
        if (expiration == 0) {
            expiration = std::time(nullptr) + DEFAULT_EXPIRATION_DAYS * 24 * 60 * 60;
        }

        TokenData data(userId, expiration);

        std::string payload = std::to_string(data.userId) + "," + std::to_string(data.expiration);
        std::vector<unsigned char> payloadBytes(payload.begin(), payload.end());

        // Simulación de firma
        std::vector<unsigned char> signedData = payloadBytes; // Aquí agregaríamos una firma real

        return base64Encode(signedData);
    }

    TokenData verifyToken(const std::string& token) {
        std::vector<unsigned char> decoded = base64Decode(token);

        // Verificación de firma simulada
        std::string payload(decoded.begin(), decoded.end());
        auto delimiterPos = payload.find(',');
        if (delimiterPos == std::string::npos) {
            throw std::runtime_error("Invalid token format");
        }

        long userId = std::stol(payload.substr(0, delimiterPos));
        std::time_t expiration = std::stol(payload.substr(delimiterPos + 1));

        if (std::time(nullptr) > expiration) {
            throw std::runtime_error("Token has expired");
        }

        return TokenData(userId, expiration);
    }
};

int main() {
    TokenManager tokenManager;

    try {
        // Generar token
        std::string token = tokenManager.generateToken(12345);
        std::cout << "Generated Token: " << token << std::endl;

        // Verificar token
        auto data = tokenManager.verifyToken(token);
        std::cout << "Token Verified: UserID=" << data.userId << ", Expiration=" << data.expiration << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
