#include "shop.h"

void Shop::showItems() const {
        std::cout << "\n=== Shop Menu ===\n";
        for (const auto& entry : priceList) {
            std::cout << entry.first << " - " << entry.second << " coins\n";
        }
        std::cout << "=================\n";
    }

    int Shop::getPrice(const std::string& itemName) const 
{
    auto it = priceList.find(itemName);
    if (it != priceList.end())
        return it->second;
    return -1; // or throw, or handle as you wish
}

    void Shop::buy(const std::string& itemName) {
        auto& player = Player::getInstance();

        auto it = priceList.find(itemName);
        if (it == priceList.end()) {
            std::cout << "Item not found.\n";
            return;
        }

        int price = it->second;
        if (!player.afford(price)) {
            std::cout << "You don't have enough coins.\n";
            return;
        }

        try {
            std::shared_ptr<Item> item = ItemFactory::createItem(itemName);
            player.getItemInventory().add(item);
            player.spend(price);
            std::cout << "You bought a " << item->getName() << "!\n";
        } catch (const std::exception& e) {
            std::cout << "Error creating item: " << e.what() << "\n";
        }
    }

        std::shared_ptr<Item> Shop::buyItem(const std::string& itemName) {
    auto it = priceList.find(itemName);
    if (it == priceList.end()) {
        std::cout << "Item not found.\n";
        return nullptr;
    }

    int price = it->second;
    auto& player = Player::getInstance();

    if (!player.afford(price)) {
        std::cout << "You don't have enough coins.\n";
        return nullptr;
    }

    try {
        std::shared_ptr<Item> item = ItemFactory::createItem(itemName);
        return item; // let calling code handle adding and spending
    } catch (const std::exception& e) {
        std::cout << "Error creating item: " << e.what() << "\n";
        return nullptr;
    }
}