#include "Player.h"
#include "Enitity.h"
#include "Inventory.h"
#include "Item.h"

bool Player::afford(int amount) const
 {return gold >= amount;}

void Player::spend(int amount) { gold -= amount;}

void Player::buyTower (std::shared_ptr<Tower> tower, int cost)
{
    spend(cost);
    towerInventory.add(tower);
}
int Player::getGold() const {return gold;}
void Player::setName(const std::string name)
{
    this->name = name;
}
     void Player::looseLife()
        {
            if (lifes > 0) lifes--;
        }
int Player::getLives() const
        {
            return lifes;
        }

        bool Player::isAlive() const
        {
            return lifes > 0;
        }
        std::string Player::getName()
        {
            return name;
        }
        
        void Player::addGold(int add)
        {
            gold +=add;
        }

Inventory<std::shared_ptr<Item>>&  Player::getItemInventory() {
            return itemInventory;
        }
        
        Inventory<std::shared_ptr<Tower>>& Player::getTowerInventory() {
            return towerInventory;
        }