#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
void pauseMs(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
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

void fightEnemy(Player &player, Room &room, int enemyIndex)
{
    Enemy *enemy = room.getEnemy(enemyIndex);
    if (!enemy)
    {
        std::cout << "Invalid enemy index.\n";
        return;
    }

    std::cout << "You engage " << enemy->name << " in combat.\n";
    pauseMs(2000);

    while (player.isAlive() && enemy->isAlive())
    {

        int playerDmg = player.attackD;

        playerDmg += (std::rand() % 3) - 1;
        if (playerDmg < 0)
            playerDmg = 0;

        enemy->hp -= playerDmg;
        if (enemy->hp < 0)
            enemy->hp = 0;

        std::cout << "You hit " << enemy->name << " for " << playerDmg
                  << " damage. Enemy HP: " << enemy->hp << "\n";

        pauseMs(2000);

        if (!enemy->isAlive())
            break;

        int enemyDmg = enemy->attackD;

        enemyDmg += (std::rand() % 3) - 1;
        if (enemyDmg < 0)
            enemyDmg = 0;

        player.hp -= enemyDmg;
        if (player.hp < 0)
            player.hp = 0;

        std::cout << enemy->name << " hits you for " << enemyDmg
                  << " damage. Your HP: " << player.hp << "\n";
        pauseMs(2000);
    }

    if (!player.isAlive())
    {
        std::cout << "You have been defeated.\n";
        return;
    }

    Enemy *defeated = room.removeEnemyAt(enemyIndex);
    delete defeated;

    std::cout << "You defeated the enemy.\n";
    pauseMs(2000);
}
void printHelp()
{
    std::cout
        << "Commands:\n"
        << "  help\n"
        << "  stats\n"
        << "  inv\n"
        << "  go <north|south|east|west>\n"
        << "  take <index number>\n"
        << "  use <index number>\n"
        << "  fight <index number>\n"
        << "  quit\n";
}

void printStats(const Player &p)
{
    std::cout << "Player: " << p.name
              << " | HP: " << p.hp
              << " | ATK: " << p.attackD
              << "\n";
}

bool readIntArg(int &out)
{
    if (!(std::cin >> out))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

struct World
{
    Room **rooms;
    int count;

    Room *start;

    World(int roomCount)
        : rooms(nullptr), count(roomCount), start(nullptr)
    {
        rooms = new Room *[count];
        for (int i = 0; i < count; i++)
        {
            rooms[i] = nullptr;
        }
    }

    ~World()
    {
        for (int i = 0; i < count; i++)
        {
            delete rooms[i];
        }
        delete[] rooms;
    }
};

World *createWorld()
{
    World *w = new World(3);

    w->rooms[0] = new Room("Village");
    w->rooms[1] = new Room("Forest");
    w->rooms[2] = new Room("Old Castle");

    Room *village = w->rooms[0];
    Room *forest = w->rooms[1];
    Room *castle = w->rooms[2];

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

    w->start = village;

    return w;
}

int main()
{

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::string pname;
    std::cout << "Insert Player Name: ";
    std::cin >> pname;

    Player p(pname);

    World *world = createWorld();
    Room *current = world->start;

    printHelp();
    while (p.isAlive())
    {
        pauseMs(1800);

        current->describe();
        printStats(p);

        std::cout << "\n> ";
        std::string cmd;
        std::cin >> cmd;

        if (!std::cin)
            break;

        if (cmd == "quit")
        {
            break;
        }
        else if (cmd == "help")
        {
            printHelp();
            pauseMs(1000);
        }
        else if (cmd == "stats")
        {
            printStats(p);
            pauseMs(1000);
        }
        else if (cmd == "inv")
        {
            p.inventory.list();
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
            if (!readIntArg(idx))
            {
                std::cout << "Please type: take <number>\n";
                continue;
            }

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
        else if (cmd == "use")
        {
            int idx;
            if (!readIntArg(idx))
            {
                std::cout << "Please type: use <number>\n";
                continue;
            }

            Item *item = p.inventory.get(idx);
            if (!item)
            {
                std::cout << "Invalid inventory index.\n";
                continue;
            }

            item->use(p);

            if (dynamic_cast<HealthPotion *>(item) != nullptr)
            {
                Item *consumed = p.inventory.removeAt(idx);
                delete consumed;
                std::cout << "The potion was consumed.\n";
            }
        }
        else if (cmd == "fight")
        {
            int idx;
            if (!readIntArg(idx))
            {
                std::cout << "Please type: fight <number>\n";
                continue;
            }

            fightEnemy(p, *current, idx);

            if (!p.isAlive())
            {
                std::cout << "Game Over.\n";
                break;
            }
        }
        else
        {
            std::cout << "Unknown command. Type 'help'.\n";
        }
    }

    delete world;
    return 0;
}
