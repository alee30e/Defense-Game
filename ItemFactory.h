#pragma once
#include <memory>
#include <string>
#include "Item.h"

class ItemFactory {
public:
    static std::shared_ptr<Item> createItem(const std::string& itemType) {
        if (itemType == "bomb") {
            return std::make_shared<Bomb>();
        } else if (itemType == "potion") {
            return std::make_shared<Potion>(50);
        } else if (itemType == "trap") {
            return std::make_shared<Trap>();
        } else if (itemType == "archer_tower") {
            return std::make_shared<TowerItem>(std::make_shared<ArcherTower>());
        } 
        else {
            throw std::invalid_argument("Invalid item type: " + itemType);
        }
    }
};
