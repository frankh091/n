#include <iostream>
#include <stdexcept>
#include <string>

class CodeRequiredException : public std::runtime_error {
public:
    CodeRequiredException() : std::runtime_error("Code not provided") {}
};

int main() {
    try {
        throw CodeRequiredException();
    } catch (const CodeRequiredException& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
    return 0;
}
