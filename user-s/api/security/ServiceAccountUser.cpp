#include <iostream>
#include <string>

class User {
protected:
    long id;
    std::string name;
    std::string email;
    bool administrator;

public:
    User() : id(0), administrator(false) {}

    void setId(long id) {
        this->id = id;
    }

    void setName(const std::string& name) {
        this->name = name;
    }

    void setEmail(const std::string& email) {
        this->email = email;
    }

    void setAdministrator(bool administrator) {
        this->administrator = administrator;
    }

    long getId() const {
        return id;
    }

    std::string getName() const {
        return name;
    }

    std::string getEmail() const {
        return email;
    }

    bool isAdministrator() const {
        return administrator;
    }
};

class ServiceAccountUser : public User {
public:
    static const long ID = 9000000000000000000L;

    ServiceAccountUser() {
        setId(ID);
        setName("Service Account");
        setEmail("none");
        setAdministrator(true);
    }
};

int main() {
    ServiceAccountUser serviceAccount;

    std::cout << "ID: " << serviceAccount.getId() << std::endl;
    std::cout << "Name: " << serviceAccount.getName() << std::endl;
    std::cout << "Email: " << serviceAccount.getEmail() << std::endl;
    std::cout << "Is Administrator: " << (serviceAccount.isAdministrator() ? "Yes" : "No") << std::endl;

    return 0;
}
