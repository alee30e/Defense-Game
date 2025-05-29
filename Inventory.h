#pragma once
#include <vector>
#include <memory>
#include<map>
#include <iostream>
#include "Item.h"

template <typename T>
class Inventory {
private:
    std::vector<T> items;

public:
    Inventory() = default;
    static Inventory<T>& getInstance() {
        static Inventory<T> instance;
        return instance;
    }
        
    template<typename Predicate> int countIf(Predicate pred) const;
    void add(const T& item);

    void remove(int index);

    T& get(int index);

    int size() const;
    void list() const;
    std::vector<T>& getAll() {
        return items;
    }
    void useItemByType(    std::vector<std::shared_ptr<Monster>>& monsters,
    std::vector<std::pair<int, int>>& trapPositions,
    int rows,
    int cols);

void useItemByIndex(
    std::vector<std::shared_ptr<Monster>>& monsters,
    std::vector<std::pair<int, int>>& trapPositions,
     std::vector<std::shared_ptr<Tower>>& towers,
    int rows,
    int cols
);
void useBomb(std::vector<std::shared_ptr<Monster>>& monsters, int rows, int cols);
void useTrap(std::vector<std::pair<int, int>>& trapPositions, int rows, int cols);

void useTower(std::vector<std::shared_ptr<Tower>>& towers, int rows) ;
void usePotion(std::vector<std::shared_ptr<Tower>>& towers);

};
#include "Inventory.cpp"