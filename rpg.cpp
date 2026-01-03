#include <iostream>
#include <string>
class Player;


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

class Inventory {
private:
    Item** items;     
    int count;        
    int capacity;     

    void resize(int newCapacity);

public:
    Inventory();
    ~Inventory();

    void add(Item* itemPtr);
    void list() const;
};
class Player {
public:
    std::string name;
    int hp, attackD;
    Inventory inventory;   

    Player(const std::string& n) : name(n), hp(25), attackD(5), inventory() {}
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


Inventory::Inventory() : items(nullptr), count(0), capacity(0) {
    capacity = 4;
    items = new Item*[capacity];
}

Inventory::~Inventory() {
    for (int i = 0; i < count; i++) {
        delete items[i];
    }
    delete[] items;
}


void Inventory::resize(int newCapacity) {
    Item** newArr = new Item*[newCapacity];

  
    for (int i = 0; i < count; i++) {
        newArr[i] = items[i];
    }

    delete[] items;   
    items = newArr;
    capacity = newCapacity;
}

void Inventory::add(Item* itemPtr) {
    if (!itemPtr) return;

    if (count == capacity) {
        resize(capacity * 2);
    }

    items[count++] = itemPtr;
}

void Inventory::list() const {
    if (count == 0) {
        std::cout << "Inventory is empty.\n";
        return;
    }

    std::cout << "Inventory:\n";
    for (int i = 0; i < count; i++) {
        std::cout << "  [" << i << "] " << items[i]->name
                  << " - " << items[i]->description << "\n";
    }
}



int main() { 
    std::string pname;
    std::cout << "Insert Player Name: ";
    std::cin >> pname;

    Player p(pname);

    //std::cout << "Player " << p.name << " created with " << p.hp << "HP and " << p.attackD << " Attack Damage"<< "\n";


    p.inventory.add(new HealthPotion(6));
    p.inventory.list();
}
