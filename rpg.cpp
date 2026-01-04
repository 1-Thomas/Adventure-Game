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

    int size() const;
    Item* get(int index) const;
    Item* removeAt(int index);
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
    HealthPotion(int amt) : Item("Healing Potion", "Adds more HP."), healAmount(amt) {}
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

int Inventory::size() const {
    return count;
}

Item* Inventory::get(int index) const {
    if (index < 0 || index >= count) {
        return nullptr;
    }
    return items[index];
}

Item* Inventory::removeAt(int index) {
    if (index < 0 || index >= count) {
        return nullptr;
    }

    Item* removed = items[index];

    for (int i = index; i < count - 1; i++) {
        items[i] = items[i + 1];
    }

    count--;
    return removed;
}




int main() { 
    std::string pname;
    std::cout << "Insert Player Name: ";
    std::cin >> pname;

    Player p(pname);

    //std::cout << "Player " << p.name << " created with " << p.hp << "HP and " << p.attackD << " Attack Damage"<< "\n";


    p.inventory.add(new HealthPotion(6));
    p.inventory.list();

    std::cout << "Enter index of item to use: ";
    int idx;
    std::cin >> idx;

    Item* it = p.inventory.get(idx);
    if (!it) {
        std::cout << "Invalid inventory index.\n";
    } else {
        it->use(p);

        Item* consumed = p.inventory.removeAt(idx);
        delete consumed;

        std::cout << "Item consumed and removed from inventory.\n";
    }

    std::cout << "Player HP is now: " << p.hp << "\n";
    p.inventory.list();
}
