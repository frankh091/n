#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

class StorageException : public std::runtime_error {
public:
    explicit StorageException(const std::string& message) : std::runtime_error(message) {}
};

class CryptoManager {
private:
    EC_KEY* privateKey;
    EC_KEY* publicKey;

    void initializeKeys() {
        privateKey = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
        if (!privateKey) {
            throw std::runtime_error("Failed to initialize private key.");
        }

        if (!EC_KEY_generate_key(privateKey)) {
            EC_KEY_free(privateKey);
            throw std::runtime_error("Failed to generate key pair.");
        }

        publicKey = EC_KEY_dup(privateKey);
        if (!publicKey) {
            EC_KEY_free(privateKey);
            throw std::runtime_error("Failed to duplicate public key.");
        }
    }

public:
    CryptoManager() : privateKey(nullptr), publicKey(nullptr) {
        initializeKeys();
    }

    ~CryptoManager() {
        if (privateKey) EC_KEY_free(privateKey);
        if (publicKey) EC_KEY_free(publicKey);
    }

    std::vector<unsigned char> sign(const std::vector<unsigned char>& data) {
        unsigned int sigLen = 0;
        std::vector<unsigned char> signature(ECDSA_size(privateKey));

        if (!ECDSA_sign(0, data.data(), data.size(), signature.data(), &sigLen, privateKey)) {
            throw std::runtime_error("Failed to sign data.");
        }

        signature.resize(sigLen);
        return signature;
    }

    bool verify(const std::vector<unsigned char>& data, const std::vector<unsigned char>& signature) {
        int result = ECDSA_verify(0, data.data(), data.size(), signature.data(), signature.size(), publicKey);
        if (result < 0) {
            throw std::runtime_error("Error during signature verification.");
        }
        return result == 1;
    }
};

int main() {
    try {
        CryptoManager cryptoManager;

        // Datos de prueba
        std::vector<unsigned char> data = {'T', 'e', 's', 't', ' ', 'd', 'a', 't', 'a'};

        // Firmar datos
        auto signature = cryptoManager.sign(data);
        std::cout << "Data signed successfully." << std::endl;

        // Verificar firma
        if (cryptoManager.verify(data, signature)) {
            std::cout << "Signature verified successfully." << std::endl;
        } else {
            std::cout << "Signature verification failed." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}