#include <iostream>
class Player {
public:
    std::string name;
    int hp, attackD;

    Player(const std::string& n) : name(n), hp(25), attackD(5) {}
};

class Enemy {
public:
    std::string name;
    int hp, attackD;

    Enemy(const std::string& n, int h, int a) : name(n), hp(h), attackD(a) {}
};

int main() { 
    std::string pname;
    std::cout << "Insert Player Name: ";
    std::cin >> pname;
}
