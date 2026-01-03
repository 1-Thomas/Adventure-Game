#include <iostream>
#include <string>
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

class Item {
public:
    std::string name, description;

    Item(const std::string& n, const std::string& d) : name(n), description(d) {}
    virtual ~Item() = default;
    virtual void use(Player& player) = 0;
};

class HealthPotion : public Item {
    int healAmount;
public:
    HealthPotion(int amt) : Item("Healing Potion", "Restores health."), healAmount(amt) {}
    void use(Player& player) override {
        player.hp += healAmount;
        std::cout << "You gained " << healAmount << "HP.\n";
    }
};


int main() { 
    std::string pname;
    std::cout << "Insert Player Name: ";
    std::cin >> pname;

    Player p(pname);

    std::cout << "Player " << p.name << " created with " << p.hp << "HP and " << p.attackD << " Attack Damage"<< "\n";
    Item* potion = new HealthPotion(6);
    potion->use(p);
    delete potion;
}
