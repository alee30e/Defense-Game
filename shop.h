#pragma once
#include <iostream>
#include <map>
#include "ItemFactory.h"
#include "Player.h"

class Shop {
private:
    // Maps item names to their price
    std::map<std::string, int> priceList = {
        {"bomb", 30},
        {"potion", 25},
        {"trap", 20},
        {"archer_tower", 50},
    };

public:
    void showItems() const;
int getPrice(const std::string& itemName) const ;
    void buy(const std::string& itemName);

    std::shared_ptr<Item> buyItem(const std::string& itemName);

};
