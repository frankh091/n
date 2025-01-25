#include <iostream>
#include <vector>

class KeystoreModel {
private:
    std::vector<unsigned char> publicKey;
    std::vector<unsigned char> privateKey;

public:
    // Obtener clave pública
    std::vector<unsigned char> getPublicKey() const {
        return publicKey;
    }

    // Establecer clave pública
    void setPublicKey(const std::vector<unsigned char>& key) {
        publicKey = key;
    }

    // Obtener clave privada
    std::vector<unsigned char> getPrivateKey() const {
        return privateKey;
    }

    // Establecer clave privada
    void setPrivateKey(const std::vector<unsigned char>& key) {
        privateKey = key;
    }
};

int main() {
    KeystoreModel keystore;

    // Simula claves pública y privada
    std::vector<unsigned char> pubKey = {0x01, 0x02, 0x03, 0x04};
    std::vector<unsigned char> privKey = {0x0A, 0x0B, 0x0C, 0x0D};

    keystore.setPublicKey(pubKey);
    keystore.setPrivateKey(privKey);

    // Obtener y mostrar las claves
    std::cout << "Public Key: ";
    for (auto byte : keystore.getPublicKey()) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    std::cout << "Private Key: ";
    for (auto byte : keystore.getPrivateKey()) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    return 0;
}
