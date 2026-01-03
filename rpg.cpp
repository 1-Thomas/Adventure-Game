#include <iostream>
class Player {
public:
    std::string name;
    int hp, attackD;

    Player(const std::string& n) : name(n), hp(25), attackD(5) {}
};

int main() { 
    std::string pname;
    std::cout << "Insert Player Name: ";
    std::cin >> pname;
}
