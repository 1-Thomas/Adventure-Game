#include <iostream>
#include <string>
class Player;

class Enemy
{
public:
    std::string name;
    int hp, attackD;

    Enemy(const std::string &n, int h, int a) : name(n), hp(h), attackD(a) {}
    bool isAlive() const { return hp > 0; }
};

class Item
{
public:
    std::string name, description;

    Item(const std::string &n, const std::string &d) : name(n), description(d) {}
    virtual ~Item() = default;
    virtual void use(Player &player) = 0;
};

class Inventory
{
private:
    Item **items;
    int count;
    int capacity;

    void resize(int newCapacity);

public:
    Inventory();
    ~Inventory();

    void add(Item *itemPtr);
    void list() const;

    int size() const;
    Item *get(int index) const;
    Item *removeAt(int index);
};
class Player
{
public:
    std::string name;
    int hp, attackD;
    Inventory inventory;

    Player(const std::string &n) : name(n), hp(25), attackD(5), inventory() {}
    bool isAlive() const { return hp > 0; }
};
class HealthPotion : public Item
{
    int healAmount;

public:
    HealthPotion(int amt) : Item("Healing Potion", "Adds more HP."), healAmount(amt) {}
    void use(Player &player) override
    {
        player.hp += healAmount;
        std::cout << "You gained " << healAmount << "HP.\n";
    }
};
class Sword : public Item
{
    int bonusAD;

public:
    Sword(int bonus)
        : Item("Sword", "Increases attack damage."), bonusAD(bonus) {}

    void use(Player &player) override
    {
        player.attackD += bonusAD;
        std::cout << "You equipped the " << name << "! Attack increased by "
                  << bonusAD << ". New ATK: " << player.attackD << "\n";
    }
};

Inventory::Inventory() : items(nullptr), count(0), capacity(0)
{
    capacity = 4;
    items = new Item *[capacity];
}

Inventory::~Inventory()
{
    for (int i = 0; i < count; i++)
    {
        delete items[i];
    }
    delete[] items;
}

void Inventory::resize(int newCapacity)
{
    Item **newArr = new Item *[newCapacity];

    for (int i = 0; i < count; i++)
    {
        newArr[i] = items[i];
    }

    delete[] items;
    items = newArr;
    capacity = newCapacity;
}

void Inventory::add(Item *itemPtr)
{
    if (!itemPtr)
        return;

    if (count == capacity)
    {
        resize(capacity * 2);
    }

    items[count++] = itemPtr;
}

void Inventory::list() const
{
    if (count == 0)
    {
        std::cout << "Inventory is empty.\n";
        return;
    }

    std::cout << "Inventory:\n";
    for (int i = 0; i < count; i++)
    {
        std::cout << "  [" << i << "] " << items[i]->name
                  << " - " << items[i]->description << "\n";
    }
}

int Inventory::size() const
{
    return count;
}

Item *Inventory::get(int index) const
{
    if (index < 0 || index >= count)
    {
        return nullptr;
    }
    return items[index];
}

Item *Inventory::removeAt(int index)
{
    if (index < 0 || index >= count)
    {
        return nullptr;
    }

    Item *removed = items[index];

    for (int i = index; i < count - 1; i++)
    {
        items[i] = items[i + 1];
    }

    count--;
    return removed;
}

class Room
{
public:
    std::string name;

    Room *north;
    Room *south;
    Room *east;
    Room *west;

    Room(const std::string &n)
        : name(n),
          north(nullptr), south(nullptr), east(nullptr), west(nullptr),
          enemies(nullptr), enemyCount(0), enemyCapacity(0),
          groundItems(nullptr), itemCount(0), itemCapacity(0)
    {
        enemyCapacity = 3;
        enemies = new Enemy *[enemyCapacity];

        itemCapacity = 3;
        groundItems = new Item *[itemCapacity];
    }

    ~Room()
    {
        for (int i = 0; i < enemyCount; i++)
        {
            delete enemies[i];
        }
        delete[] enemies;

        for (int i = 0; i < itemCount; i++)
        {
            delete groundItems[i];
        }
        delete[] groundItems;
    }

    void addItem(Item *it);
    void listItems() const;
    Item *removeItemAt(int index);

    void addEnemy(Enemy *e)
    {
        if (!e)
            return;
        if (enemyCount == enemyCapacity)
        {
            resizeEnemies(enemyCapacity * 2);
        }
        enemies[enemyCount++] = e;
    }

    void listEnemies() const
    {
        if (enemyCount == 0)
        {
            std::cout << "Enemies: none\n";
            return;
        }

        std::cout << "Enemies:\n";
        for (int i = 0; i < enemyCount; i++)
        {
            std::cout << "  [" << i << "] " << enemies[i]->name
                      << " (HP " << enemies[i]->hp
                      << ", ATK " << enemies[i]->attackD << ")\n";
        }
    }

    int getEnemyCount() const { return enemyCount; }

    Enemy *getEnemy(int index) const
    {
        if (index < 0 || index >= enemyCount)
            return nullptr;
        return enemies[index];
    }

    Enemy *removeEnemyAt(int index)
    {
        if (index < 0 || index >= enemyCount)
            return nullptr;

        Enemy *removed = enemies[index];
        for (int i = index; i < enemyCount - 1; i++)
        {
            enemies[i] = enemies[i + 1];
        }
        enemyCount--;
        return removed;
    }

    void describe() const
    {
        std::cout << "\n== " << name << " ==\n";
        listEnemies();
        listItems();

        std::cout << "Exits: ";
        bool any = false;
        if (north)
        {
            std::cout << "north ";
            any = true;
        }
        if (south)
        {
            std::cout << "south ";
            any = true;
        }
        if (east)
        {
            std::cout << "east ";
            any = true;
        }
        if (west)
        {
            std::cout << "west ";
            any = true;
        }
        if (!any)
            std::cout << "none";
        std::cout << "\n";
    }

private:
    Enemy **enemies;
    int enemyCount;
    int enemyCapacity;

    Item **groundItems;
    int itemCount;
    int itemCapacity;

    void resizeEnemies(int newCapacity)
    {
        Enemy **newArr = new Enemy *[newCapacity];
        for (int i = 0; i < enemyCount; i++)
        {
            newArr[i] = enemies[i];
        }
        delete[] enemies;
        enemies = newArr;
        enemyCapacity = newCapacity;
    }

    void resizeItems(int newCapacity);
};

Room *moveRoom(Room *current, const std::string &dir)
{
    if (!current)
        return nullptr;

    if (dir == "north")
        return current->north ? current->north : current;
    if (dir == "south")
        return current->south ? current->south : current;
    if (dir == "east")
        return current->east ? current->east : current;
    if (dir == "west")
        return current->west ? current->west : current;

    return current;
}

void Room::resizeItems(int newCapacity)
{
    Item **newArr = new Item *[newCapacity];

    for (int i = 0; i < itemCount; i++)
    {
        newArr[i] = groundItems[i];
    }

    delete[] groundItems;
    groundItems = newArr;
    itemCapacity = newCapacity;
}

void Room::addItem(Item *it)
{
    if (!it)
        return;

    if (itemCount == itemCapacity)
    {
        resizeItems(itemCapacity * 2);
    }

    groundItems[itemCount++] = it;
}

void Room::listItems() const
{
    if (itemCount == 0)
    {
        std::cout << "Items on ground: none\n";
        return;
    }

    std::cout << "Items on ground:\n";
    for (int i = 0; i < itemCount; i++)
    {
        std::cout << "  [" << i << "] " << groundItems[i]->name
                  << " - " << groundItems[i]->description << "\n";
    }
}

Item *Room::removeItemAt(int index)
{
    if (index < 0 || index >= itemCount)
        return nullptr;

    Item *removed = groundItems[index];

    for (int i = index; i < itemCount - 1; i++)
    {
        groundItems[i] = groundItems[i + 1];
    }

    itemCount--;
    return removed;
}

int main()
{
    std::string pname;
    std::cout << "Insert Player Name: ";
    std::cin >> pname;

    Player p(pname);

    Room *village = new Room("Village");
    Room *forest = new Room("Forest");
    Room *castle = new Room("Old Castle");

    village->north = forest;
    forest->south = village;

    forest->east = castle;
    castle->west = forest;

    forest->addEnemy(new Enemy("Bandit", 12, 4));
    forest->addEnemy(new Enemy("Troll", 14, 5));

    castle->addEnemy(new Enemy("Skeleton Guard", 18, 6));
    castle->addEnemy(new Enemy("Armored Knight", 22, 7));

    village->addItem(new HealthPotion(6));
    forest->addItem(new Sword(3));
    castle->addItem(new HealthPotion(10));

    Room *current = village;

    std::cout << "Commands: go <north|south|east|west>, take <index>, inv, quit\n";

    while (true)
    {
        current->describe();

        std::cout << "\n> ";
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "quit")
        {
            break;
        }
        else if (cmd == "go")
        {
            std::string dir;
            std::cin >> dir;

            Room *next = moveRoom(current, dir);
            if (next == current)
            {
                std::cout << "You can't go that way.\n";
            }
            else
            {
                current = next;
                std::cout << "You move " << dir << ".\n";
            }
        }
        else if (cmd == "take")
        {
            int idx;
            std::cin >> idx;

            Item *picked = current->removeItemAt(idx);
            if (!picked)
            {
                std::cout << "Invalid item index.\n";
            }
            else
            {
                std::cout << "You picked up: " << picked->name << "\n";
                p.inventory.add(picked); 
            }
        }
        else if (cmd == "inv")
        {
            p.inventory.list();
        }
        else
        {
            std::cout << "Unknown command.\n";
        }
    }
    delete village;
    delete forest;
    delete castle;

    return 0;
}
