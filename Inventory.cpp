#include "Inventory.h"
#include "Item.h"
template <typename T>
    template<typename Predicate> int Inventory<T>::countIf(Predicate pred) const 
    {
        int count = 0;
        for (const auto& item : items) {
            if (pred(item)) ++count;
        }
        return count;
    }


template <typename T>
void Inventory<T>::add(const T& item) {
    items.push_back(item);
}
template <typename T>
    void Inventory<T>::remove(int index) {
        if (index >= 0 && index < items.size()) {
            items.erase(items.begin() + index);
        } else {
            throw std::out_of_range("Invalid inventory index");
        }
    }

template <typename T>
    T& Inventory<T>:: get(int index) {
        if (index >= 0 && index < items.size()) {
            return items[index];
        } else {
            throw std::out_of_range("Invalid inventory index");
        }
    }

template <typename T>
    int Inventory<T>::size() const {
        return items.size();
    }

    template <typename T>
    void Inventory<T>::list() const {
    std::cout << "Numar total de iteme: " << items.size() << "\n";

    std::map<std::string, int> typeCounts;
    for (const auto& item : items) {
        std::string name = item->getName();
        typeCounts[name]++;
    }
    for (const auto& [name, count] : typeCounts) {
        std::cout << name << ": " << count << "\n";
    }
    }

    template <typename T>
    void Inventory<T>::useItemByType(    std::vector<std::shared_ptr<Monster>>& monsters,
    std::vector<std::pair<int, int>>& trapPositions,
    int rows,
    int cols) {

    std::map<std::string, int> typeCounts;
    for (const auto& item : items) {
        typeCounts[item->getName()]++;
    }

    std::cout << "Alege tipul de item de folosit:\n";
    int idx = 1;
    std::vector<std::string> typeList;
    for (const auto& [name, count] : typeCounts) {
        std::cout << idx << ". " << name << " - " << count << "\n";
        typeList.push_back(name);
        idx++;
    }
    if (typeList.empty()) {
        std::cout << "Inventarul este gol!\n";
        return;
    }

    int choice;
    std::cout << "Introdu numarul tipului de item: ";
    std::cin >> choice;
    if (choice < 1 || choice > typeList.size()) {
        std::cout << "Optiune invalida!\n";
        return;
    }
    std::string chosenType = typeList[choice - 1];

    for (int i = 0; i < items.size(); ++i) {
    if (items[i]->getName().find("Bomb") != std::string::npos) {
    useBomb(monsters, rows, cols);
    items.erase(items.begin() + i);
    std::cout << "Ai folosit o bomba.\n";
    return;
}



if (items[i]->getName().find("Trap") != std::string::npos) {
    useTrap(trapPositions, rows, cols);
    items.erase(items.begin() + i);
    std::cout << "Ai plasat o capcana.\n";
    return;
}
}
    std::cout << "Nu ai niciun item de acest tip!\n";
}

    template <typename T>
    void Inventory<T>:: useItemByIndex(
    std::vector<std::shared_ptr<Monster>>& monsters,
    std::vector<std::pair<int, int>>& trapPositions,
     std::vector<std::shared_ptr<Tower>>& towers,
    int rows,
    int cols
) {
    if (items.empty()) {
        std::cout << "Inventarul este gol!\n";
        return;
    }

    std::cout << "Alege indexul itemului de folosit:\n";
    for (int i = 0; i < items.size(); ++i) {
        std::cout << i << ". " << items[i]->getName() << "\n";
    }

    int idx;
    std::cout << "Introdu indexul: ";
    std::cin >> idx;
    if (idx < 0 || idx >= items.size()) {
        std::cout << "Index invalid!\n";
        return;
    }
    std::string name = items[idx]->getName();
    if (name.find("Bomb") != std::string::npos) {
        useBomb(monsters, rows, cols);
        std::cout << "Ai folosit o bomba.\n";
         items.erase(items.begin() + idx);
    } else if (name.find("Trap") != std::string::npos) {
        useTrap(trapPositions, rows, cols);
        std::cout << "Ai plasat o capcana.\n";
        items.erase(items.begin() + idx);
    } else if (name.find("Tower") != std::string::npos) 
    {
    useTower(towers, rows);
    std::cout << "Ai plasat un turn.\n";
    }
    else if (name.find("Potion") != std::string::npos) {
    usePotion(towers);
    std::cout << "Ai folosit o potiune.\n";
    items.erase(items.begin() + idx);
}
    else {
        items[idx]->use();
        std::cout << "Ai folosit " << name << ".\n";
    }
}

template <typename T>
    void Inventory<T>::useBomb(std::vector<std::shared_ptr<Monster>>& monsters, int rows, int cols) {
    int row, col;
    std::cout << "Plaseaza bomba! Introdu randul (0-" << rows-1 << ") si coloana (0-" << cols-1 << "): ";
    std::cin >> row >> col;
    int radius = 1; // area of effect

    int killed = 0;
    for (auto& m : monsters) {
        if (m->isAlive() &&
            std::abs(m->getY() - row) <= radius &&
            std::abs(m->getX() - col) <= radius) {
            m->takeDamage(9999); // or m->setHP(0);
            killed++;
        }
    }
    std::cout << "Bomba a ucis " << killed << " monstri in zona!\n";
}





template <typename T>
    void Inventory<T>::useTrap(std::vector<std::pair<int, int>>& trapPositions, int rows, int cols) {
    int row, col;
    std::cout << "Plaseaza capcana! Introdu randul (0-" << rows-1 << ") si coloana (0-" << cols-1 << "): ";
    std::cin >> row >> col;
    trapPositions.emplace_back(row, col);
    std::cout << "Capcana plasata la (" << row << ", " << col << ")!\n";
}



template <typename T>
    void Inventory<T>::useTower(std::vector<std::shared_ptr<Tower>>& towers, int rows) {
    int row;
    
    while (true) {std::cout << "Alege randul (0-" << rows-1 << ") unde vrei sa plasezi turnul: ";
    std::cin >> row;
    if (row < 0 || row >= rows) {
        std::cout << "Rand invalid!\n";
        return;
    }
    if (towers[row]) {
        std::cout << "Exista deja un turn pe acest rand!\n";
        return;
    }
    for (int i = 0; i < items.size(); ++i) {
        auto towerItem = std::dynamic_pointer_cast<TowerItem>(items[i]);
        if (towerItem) {
            towers[row] = towerItem->getTower();
            std::cout << "Ai plasat " << towerItem->getTower()->getSymbol() << " pe randul " << row << "!\n";
            items.erase(items.begin() + i);
            return;
        }
    }}
    std::cout << "Nu ai niciun turn in inventar!\n";
}



template <typename T>
    void Inventory<T>::usePotion(std::vector<std::shared_ptr<Tower>>& towers) {
    if (towers.empty()) {
        std::cout << "Nu exista turnuri de vindecat!\n";
        return;
    }
    std::cout << "Alege indexul turnului pe care vrei sa il vindeci:\n";
    for (size_t i = 0; i < towers.size(); ++i) {
        if (towers[i])
            std::cout << i << ". " << towers[i]->getSymbol() << " (HP: " << towers[i]->getHP() << ")\n";
    }
    int idx;
    std::cout << "Introdu indexul turnului: ";
    std::cin >> idx;
    if (idx < 0 || idx >= towers.size() || !towers[idx]) {
        std::cout << "Index invalid sau turn inexistent!\n";
        return;
    }
    // Heal the tower (you can adjust the heal amount)
    int healAmount = 50;
    towers[idx]->heal(healAmount);
    std::cout << "Ai vindecat turnul " << idx << " cu " << healAmount << " HP!\n";
}
