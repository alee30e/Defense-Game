#include<iostream>
#include<vector>
#include<memory>
#include<random>
#include<algorithm>
#include <conio.h>
#include<chrono>
#include<thread>
#include "Enitity.h"
#include "Item.h"
#include "Inventory.h"
#include "Player.h"
#include "shop.h"
#include "GameException.h"

std::vector<std::pair<int, int>> trapPositions;
void shopPhase(Shop& shop) {
    Player& player = Player::getInstance();
    
    shop.showItems();
    
    std::string choice;
    std::cout << "Choose item to buy: ";
    std::cin >> choice;

    auto item = shop.buyItem(choice);
    if (item && player.afford(item->getPrice())) {
        player.spend(item->getPrice());
        player.getItemInventory().add(item);
    }
}

void shopMenu(std::vector<std::shared_ptr<Monster>>& monsters,
    std::vector<std::pair<int, int>>& trapPositions,
        std::vector<std::shared_ptr<Tower>>& towers,
    int rows,
    int cols) 
    {
    Shop shop;
    Player& player = Player::getInstance();

    bool running = true;
    while (running) {
        shop.showItems();
        std::cout << "Gold: " << player.getGold() << "\n";
        std::cout << "1. Cumpara item\n";
        std::cout << "2. Vezi inventarul\n";
        std::cout << "3. Foloseste item\n";
        std::cout << "0. Iesire\n";
        std::cout << "Optiune: ";
        int opt;
        std::cin >> opt;
        std::cin.ignore();
        try {

        if (opt == 1) {
            std::string itemName;
            std::cout << "Introdu numele itemului: ";
            std::cin >> itemName;
            auto item = shop.buyItem(itemName);
            if (item) {
                player.getItemInventory().add(item);
                player.spend(shop.getPrice(itemName));
                std::cout << "Ai cumparat " << item->getName() << "!\n";
            }
        } else if (opt == 2) {
            std::cout << "Inventarul tau:\n";
            player.getItemInventory().list();
        } else if (opt == 3) {
            player.getItemInventory().useItemByIndex(monsters, trapPositions, towers, rows, cols);
        } else if (opt == 0) {
            running = false;
        } else {
            throw GameException("Optiune invalida! Incearca din nou.");
        }
        }   catch (const GameException& ex) {
            std::cout << "Eroare: " << ex.what() << "\n";
        } catch (const std::exception& ex) {
            std::cout << "Eroare neasteptata: " << ex.what() << "\n";
        }
    }
}
void clearScreen() {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

const int rows = 4;
const int cols = 10;
const int monstersPerRow = 3;
const int spawnDelay = 2;

struct SpawnInfo {
    int toSpawn;
    int cooldown;
};

std::vector<SpawnInfo> spawnState(rows, {monstersPerRow, 0});
std::vector<std::shared_ptr<Monster>> monsters;

void drawMap(const std::vector<std::shared_ptr<Monster>>& monsters, const std::vector<std::shared_ptr<Tower>>& towers)
{
    const int rows = 4;
    const int cols = 10;

    std::vector<std::vector<std::string>> map(rows, std::vector<std::string>(cols, ". "));

    for (const auto& monster:monsters)
    {
        int id = 0;
        if (monster->getY() >=0 && monster->getX()>=0)
        {
            map[monster->getY()][monster->getX()] = monster->getSymbol() + " ";
        }
    }

        for (int i = 0; i< rows; i++)
        {
            if (i<towers.size() && towers[i])
            {
                if (towers[i]->isAlive())
                    map[i][0] = towers[i]->getSymbol();
                else map[i][0] = ". ";
            }
        }

        for (const auto& row : map)
        {
            for (const auto& cell:row)
                std::cout<<cell;
        std::cout<<"\n";
    }
        
}

std::vector<std::shared_ptr<Monster>> spawnWave(int cols) {
    std::vector<std::shared_ptr<Monster>> monsters;
    const int totalMonsters = 6;

    for (int i = 0; i < totalMonsters; ++i) {
        int row = rand() % 4; 
        int x = cols ;

        int type = rand() % 3;
        if (type == 0)
            {monsters.push_back(std::make_shared<Goblin>(x, row));
            
            std::cout<<"Monstrul "<<i<<" este la pozitia: "<<row<<": "<<x<<"\n";

            }
        else if (type == 1)
            monsters.push_back(std::make_shared<Tank>(x, row));
        else
            monsters.push_back(std::make_shared<Zombie>(x, row));
        }


    return monsters;
}

void afiseazaStatusTurele(const std::vector<std::shared_ptr<Tower>>& towers) {
    std::cout << "\n--- Status Turnuri ---\n";
    for (size_t i = 0; i < towers.size(); ++i) {
        if (towers[i]) {
            std::cout << "Turnul " << i + 1 << " (tip: " << towers[i]->getSymbol()
                      << ") - HP: " << towers[i]->getHP()
                      << " | Nivel: " << towers[i]->getLevel() << "\n";
        }
    }
    std::cout << "----------------------\n";
}

void towersAttack(std::vector<std::shared_ptr<Tower>>& towers, std::vector<std::shared_ptr<Monster>>& monsters, Player& player) {
    for (size_t row = 0; row < towers.size(); ++row) {
        auto& tower = towers[row];
        if (!tower) continue;

        for (auto& monster : monsters) {
            if (player.getLives()<= 0) break;
            //  std::cout<<monster->getReachedTower()<<"\n";
            if (monster->getY() == static_cast<int>(row) && monster->isAlive()) {
                int damage = tower->getDamage();
                // tower->attack(monster);
                
                // std::cout<<monster->getX()<<" "<<tower->getRange()<<"\n";
                if (monster->getX() <= tower->getRange()) {
                    tower->attack(monster);
                    std::cout << "Turnul " << row + 1 << " (" << tower->getSymbol()
                              << ") a lovit monstrul " << monster->getSymbol()
                              << " si i-a dat " << damage << " damage. ";
                    std::cout<<"Turnul are acum: "<<tower->getHP()<<"\n";
                }
                // std::cout<<monster->getReachedTower()<<"\n";
                if (monster->getX() == 1 && !monster->getReachedTower()) {
                    std::cout << "Monstrul a ajuns la turn!";
                    tower->getsAttacked(monster);
                    monster->hasReachedTower();
                }
                else
                if (monster->getX() == 1 && monster->getReachedTower()) {
                    std::cout << "Monstrul sta la turn!";
                    tower->getsAttacked(monster);
                }
                
                if (monster->isDead() && monster->getAddedGold() == false) {
                    std::cout << "Monstrul a murit! +10 gold!\n";
                    player.addGold(10);
                    monster->setAddedGold();
                } else {
                    std::cout << "Monstrul are acum " << monster->getHP() << " HP.\n";
                }
                break;
            }
        }
        if (player.getLives()<= 0) break;
    }

    for (auto& monster : monsters) {
        if (monster->getX() == 1) { 
            int row = monster->getY();
            if (row < towers.size() && (!towers[row] || towers[row]->getHP() <= 0)) {
                monster->setPosition(0, row);
            }
        }
    }
        for (auto& monster : monsters) {
        if (monster->getX() == 0) { 
            int row = monster->getY();
            if (row < towers.size() && (!towers[row] || towers[row]->getHP() <= 0))
            {
                monster->setPosition(-1, row);
                std::cout<<"Un monstru a ajuns in castel! Pierzi o viata\n";
                player.looseLife();
                std::cout<<player.getLives()<<"\n";
                if (player.getLives()<= 0) break;
            }
            }
        }
        for (auto& m : monsters) {
        for (auto it = trapPositions.begin(); it != trapPositions.end(); ) {
        if (m->getY() == it->first && m->getX() == it->second && m->isAlive()) {
            m->takeDamage(9999);
            std::cout << "Un monstru a calcat pe o capcana la (" << it->first << ", " << it->second << ")!\n";
            it = trapPositions.erase(it);
        } else {
            ++it;
        }
    }
}
    for (size_t i = 0; i < towers.size(); ++i) {
    if (towers[i] && !towers[i]->isAlive()) {
        towers[i] = nullptr;
    }
}

    monsters.erase(
        std::remove_if(monsters.begin(), monsters.end(), [](const std::shared_ptr<Monster>& m) {
            return m->isDead();
        }),
        monsters.end()
    );
    monsters.erase(
    std::remove_if(monsters.begin(), monsters.end(),
        [](const std::shared_ptr<Monster>& m) { return m->getX() == -1; }),
    monsters.end()
);
}


void upgradePhase(Player& player, std::vector<std::shared_ptr<Tower>>& towers) {
    std::cout << "\nUpgrade phase. Aur disponibil: " << player.getGold() << "\n";
    afiseazaStatusTurele(towers);

    for (size_t i = 0; i < towers.size(); ++i) {
        auto& tower = towers[i];
        if (!tower) continue;

        std::cout << "Vrei sa upgradezi turnul " << i + 1 << " (cost 20 gold)? (y/n): ";
        char raspuns;
        std::cin >> raspuns;
        

        if (raspuns == 'y' || raspuns == 'Y') {
            if (player.getGold() >= 20) {
                tower->upgrade();
                player.spend(20);
                std::cout << "Turnul " << i + 1 << " a fost upgradat.\n";
                std::cout<<"Acum ai "<<player.getGold()<<" gold!\n";
            } else {
                std::cout << "Nu ai suficient aur pentru upgrade!\n";
            }
        }
    }

    std::cin.ignore();
    std::cout << "Apasa Enter pentru a continua...";
    std::cin.get();
}
void moveMonstersInRow(std::vector<std::shared_ptr<Monster>>& monsters, int row) {
    std::vector<std::shared_ptr<Monster>> rowMonsters;
    for (auto& m : monsters)
        if (m->getY() == row && m->isAlive())
            rowMonsters.push_back(m);

    std::sort(rowMonsters.begin(), rowMonsters.end(),
              [](const std::shared_ptr<Monster>& a, const std::shared_ptr<Monster>& b) {
                  return a->getX() < b->getX();
              });

    for (auto& m : rowMonsters) {
        int x = m->getX();
        if (x == 1) continue;
        bool canMove = true;
        for (auto& other : monsters) {
            if (other == m) continue;
            if (other->getY() == row && other->getX() == x - 1 && other->isAlive()) {
                canMove = false;
                break;
            }
        }
        if (canMove) m->move();
    }
}

int main()
{
    Shop shop;
   try {
    // Player::reset();
    // Player& player = Player::getInstance();
    std::cout << "Introdu numele: ";
    std::string playerName;
    std::getline(std::cin, playerName);
    Player& player = Player::getInstance();
    player.setName(playerName);

    if (playerName.empty()) {
    throw GameException("Invalid name! Exiting.");
    return 1;}
    // if (playerName.empty()) {
    //     throw InvalidNameException(playerName);
    // }
   


    // Player& player = Player::getInstance(playerName);  // Singleton Player
    std::cout<<"hello, "<<playerName<<"!\n";
    Inventory<std::shared_ptr<Tower>>& inventory = Inventory<std::shared_ptr<Tower>>::getInstance();  // Template singleton

    std::vector<std::shared_ptr<Tower>> towers = {
        std::make_shared<ArcherTower>(),
        std::make_shared<ArcherTower>(),
        std::make_shared<ArcherTower>(),
        std::make_shared<ArcherTower>()
    };

    shopMenu(monsters, trapPositions, towers, rows, cols);
const int rows = 4;
const int cols = 10;
const int monstersPerRow = 3;
const int spawnDelay = 2; // turns between spawns in the same row
const int maxRounds = 3;
const int wavesPerRound = 2;

struct SpawnInfo {
    int toSpawn;
    int cooldown;
};
// std::vector<std::pair<int, int>> trapPositions;
std::vector<SpawnInfo> spawnState(rows, {monstersPerRow, 0});
std::vector<std::shared_ptr<Monster>> monsters;

for (int round = 1; round<=maxRounds; round++)
{
    if (player.getLives() > 0)
    {
        shopMenu(monsters, trapPositions, towers, rows, cols);
        std::cout<<"\n -----Runda "<<round<<" -----\n";

        for (int wave = 1; wave <= 3; ++wave) 
        { 
    std::cout << "-- Wave " << wave << " --\n";
    for (int row = 0; row < rows; ++row)
        spawnState[row] = {
            rand() % (spawnDelay + 3)
        };
    monsters.clear();

    while (true) {
        for (int row = 0; row < rows; ++row) {
            if (spawnState[row].toSpawn > 0 && spawnState[row].cooldown == 0) {
                int type = rand() % 3;
                if (type == 0)
                    monsters.push_back(std::make_shared<Goblin>(cols , row));
                else if (type == 1)
                    monsters.push_back(std::make_shared<Zombie>(cols , row));
                else
                    monsters.push_back(std::make_shared<Tank>(cols , row));
                spawnState[row].toSpawn--;
                spawnState[row].cooldown = rand() % 4 + 1;
            }
            if (spawnState[row].cooldown > 0)
                spawnState[row].cooldown--; 
                moveMonstersInRow(monsters, row);
        }

        // 2. Move monsters
        // for (auto& monster : monsters)
        //     monster->move();
           

        // 3. Draw map, towers attack, etc.
        clearScreen();
        std::cout << "-- Wave " << wave << " --\n";
        drawMap(monsters, towers);
        towersAttack(towers, monsters, player);
        if (player.getLives()<= 0) {
            std::cout<<"ai pierdut!\n"; break;}
        char ch;
        if (_kbhit()) {
    char ch = _getch();
    if (ch == 'i' || ch == 'I') {
        player.getItemInventory().useItemByIndex(monsters, trapPositions, towers, rows, cols);
    }
}

        // 4. Remove dead monsters
        monsters.erase(
            std::remove_if(monsters.begin(), monsters.end(),
                [](const std::shared_ptr<Monster>& m) { return m->isDead() || m->getX() < 0; }),
            monsters.end()
        );

        // 5. End wave if all monsters are dead and all have been spawned
        bool allSpawned = std::all_of(spawnState.begin(), spawnState.end(), [](const SpawnInfo& s) { return s.toSpawn == 0; });
        if (allSpawned && monsters.empty())
            break;

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    if (player.getLives() <= 0) { std::cout<<"ai pierdut\n";
        return 0;}
    upgradePhase(player, towers);
        std::cout << "Ai supravietuit valului " << wave << "!\n";
    // if (player.getLives()<=0) break;
    //  player.addGold(30 + 10 * round);
    // std::cout << "Felicitari! Ai supravietuit rundei " << round << ". Ai primit gold.\n";
}
 if (player.getLives()<=0) return 0;
     player.addGold(30 + 10 * round);
    std::cout << "Felicitari! Ai supravietuit rundei " << round << ". Ai primit gold.\n";
//     }
    }
}

// for (int wave = 1; wave <= 3; ++wave) {
//     std::cout << "-- Wave " << wave << " --\n";
//     // Randomize monsters per row and initial cooldown for each row
//     for (int row = 0; row < rows; ++row)
//         spawnState[row] = {
//             rand() % 4 + 1,        // 1 to 4 monsters per row
//             rand() % (spawnDelay + 3) // random initial delay (0..4)
//         };
//     monsters.clear();

//     while (true) {
//         // 1. Spawn monsters per row at random intervals
//         for (int row = 0; row < rows; ++row) {
//             if (spawnState[row].toSpawn > 0 && spawnState[row].cooldown == 0) {
//                 int type = rand() % 3;
//                 if (type == 0)
//                     monsters.push_back(std::make_shared<Goblin>(cols - 1, row));
//                 else if (type == 1)
//                     monsters.push_back(std::make_shared<Goblin>(cols - 1, row));
//                 else
//                     monsters.push_back(std::make_shared<Goblin>(cols - 1, row));
//                 spawnState[row].toSpawn--;
//                 spawnState[row].cooldown = rand() % 4 + 1; // random delay for next spawn (1..4)
//             }
//             if (spawnState[row].cooldown > 0)
//                 spawnState[row].cooldown--; 
//                 moveMonstersInRow(monsters, row);
//         }

//         // 2. Move monsters
//         // for (auto& monster : monsters)
//         //     monster->move();
           

//         // 3. Draw map, towers attack, etc.
//         clearScreen();
//         drawMap(monsters, towers);
//         towersAttack(towers, monsters, player);
//         char ch;
//         if (_kbhit()) {
//     char ch = _getch();
//     if (ch == 'i' || ch == 'I') {
//         player.getItemInventory().useItemByIndex(monsters, trapPositions, towers, rows, cols);
//     }
// }

//         // 4. Remove dead monsters
//         monsters.erase(
//             std::remove_if(monsters.begin(), monsters.end(),
//                 [](const std::shared_ptr<Monster>& m) { return m->isDead() || m->getX() < 0; }),
//             monsters.end()
//         );

//         // 5. End wave if all monsters are dead and all have been spawned
//         bool allSpawned = std::all_of(spawnState.begin(), spawnState.end(), [](const SpawnInfo& s) { return s.toSpawn == 0; });
//         if (allSpawned && monsters.empty())
//             break;

//         std::this_thread::sleep_for(std::chrono::seconds(2));
//     }

//     upgradePhase(player, towers);
// }
// const int maxRounds = 5;
// const int wavesPerRound = 3;
// int lives = 5;

// for (int round = 1; round <= maxRounds && lives > 0; ++round) {
//     std::cout << "\n=== ROUND " << round << " ===\n";
//     for (int wave = 1; wave <= wavesPerRound && lives > 0; ++wave) {
//         std::cout << "-- Wave " << wave << " --\n";
//         // Setup spawnState for this wave (randomize as before)
//         for (int row = 0; row < rows; ++row)
//             spawnState[row] = {
//                 rand() % 4 + 1,        // 1 to 4 monsters per row
//                 rand() % (spawnDelay + 3) // random initial delay (0..4)
//             };
//         monsters.clear();

//         // Wave logic (spawn, move, attack, use items, etc)
//         while (true) {
//                     for (int row = 0; row < rows; ++row) {
//             if (spawnState[row].toSpawn > 0 && spawnState[row].cooldown == 0) {
//                 int type = rand() % 3;
//                 if (type == 0)
//                     monsters.push_back(std::make_shared<Goblin>(cols - 1, row));
//                 else if (type == 1)
//                     monsters.push_back(std::make_shared<Goblin>(cols - 1, row));
//                 else
//                     monsters.push_back(std::make_shared<Goblin>(cols - 1, row));
//                 spawnState[row].toSpawn--;
//                 spawnState[row].cooldown = rand() % 4 + 1; // random delay for next spawn (1..4)
//             }
//             if (spawnState[row].cooldown > 0)
//                 spawnState[row].cooldown--;
//         }

//                 // 2. Move monsters
//         for (auto& monster : monsters)
//             monster->move();

//         // 3. Draw map, towers attack, etc.
//         drawMap(monsters, towers);
//         towersAttack(towers, monsters, player);
//         char ch;
//         if (_kbhit()) {
//     char ch = _getch();
//     if (ch == 'i' || ch == 'I') {
//         player.getItemInventory().useItemByIndex(monsters, trapPositions, rows, cols);
//     }
// }

//         // 4. Remove dead monsters
//         monsters.erase(
//             std::remove_if(monsters.begin(), monsters.end(),
//                 [](const std::shared_ptr<Monster>& m) { return m->isDead() || m->getX() < 0; }),
//             monsters.end()
//         );

//         // 5. End wave if all monsters are dead and all have been spawned
//         bool allSpawned = std::all_of(spawnState.begin(), spawnState.end(), [](const SpawnInfo& s) { return s.toSpawn == 0; });
//         if (allSpawned && monsters.empty())
//             break;

//         std::this_thread::sleep_for(std::chrono::seconds(2));

//     upgradePhase(player, towers);
//             // ... spawn monsters, move, attack, use items, etc ...
//             // ... update lives if monsters reach the castle ...
//             if (lives <= 0) break;
//             // ... end wave if all monsters dead and all spawned ...
//         if (lives <= 0) break;
//         std::cout << "Ai supravietuit valului " << wave << "!\n";
//     }
//     if (lives <= 0) break;

//     // Reward for surviving the round
//     player.addGold(30 + 10 * round);
//     std::cout << "Felicitari! Ai supravietuit rundei " << round << ". Ai primit gold.\n";

//     // Shop phase (buy/upgrade)
//     shopMenu(monsters, trapPositions, rows, cols);
//     upgradePhase(player, towers);
// }
// if (lives > 0)
//     std::cout << "Ai castigat jocul! Felicitari!\n";
// else
//     std::cout << "Game Over. Ai pierdut toate vietile.\n";
// }
   }catch (const GameException& ex) {
        std::cerr << "Game error: " << ex.what() << "\n";
        return 1;
    } catch (const std::exception& ex) {
        std::cerr << "Unexpected error: " << ex.what() << "\n";
        return 1;
    }

return 0;
}

