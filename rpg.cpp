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

class Room {
public:
    std::string name;

    Room* north;
    Room* south;
    Room* east;
    Room* west;

    Room(const std::string& n)
        : name(n), north(nullptr), south(nullptr), east(nullptr), west(nullptr) {}

    void describe() const {
        std::cout << "\n== " << name << " ==\n";

        std::cout << "Exits: ";
        bool any = false;
        if (north) { std::cout << "north "; any = true; }
        if (south) { std::cout << "south "; any = true; }
        if (east)  { std::cout << "east ";  any = true; }
        if (west)  { std::cout << "west ";  any = true; }
        if (!any) std::cout << "none";
        std::cout << "\n";
    }
};



Room* moveRoom(Room* current, const std::string& dir) {
    if (!current) return nullptr;

    if (dir == "north") return current->north ? current->north : current;
    if (dir == "south") return current->south ? current->south : current;
    if (dir == "east")  return current->east  ? current->east  : current;
    if (dir == "west")  return current->west  ? current->west  : current;

    return current;
}





int main() { 
    std::string pname;
    std::cout << "Insert Player Name: ";
    std::cin >> pname;

    Player p(pname);

    // std::cout << "Player " << p.name << " created with " << p.hp << "HP and " << p.attackD << " Attack Damage"<< "\n";


    // p.inventory.add(new HealthPotion(6));
    // p.inventory.list();

    // std::cout << "Enter index of item to use: ";
    // int idx;
    // std::cin >> idx;

    // Item* it = p.inventory.get(idx);
    // if (!it) {
    //     std::cout << "Invalid inventory index.\n";
    // } else {
    //     it->use(p);

    //     Item* consumed = p.inventory.removeAt(idx);
    //     delete consumed;

    //     std::cout << "Item consumed and removed from inventory.\n";
    // }

    // std::cout << "Player HP is now: " << p.hp << "\n";
    // p.inventory.list();

    Room* village = new Room("Village");
    Room* forest  = new Room("Forest");
    Room* castle   = new Room("Old Castle");

    village->north = forest;
    forest->south  = village;

    forest->east   = castle;
    castle->west    = forest;

    Room* current = village;

    std::cout << "Type: go north/south/east/west or quit\n";

    while (true) {
        current->describe();

        std::cout << "\n> ";
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "quit") {
            break;
        }
        else if (cmd == "go") {
            std::string dir;
            std::cin >> dir;

            Room* next = moveRoom(current, dir);
            if (next == current) {
                std::cout << "You can't go that way.\n";
            } else {
                current = next;
                std::cout << "You move " << dir << ".\n";
            }
        }
        else {
            std::cout << "Unknown command.\n";
        }
    }


    
    delete village;
    delete forest;
    delete castle;
    return 0;
}
