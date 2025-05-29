#pragma once
#include<iostream>
#include<vector>
#include<memory>
#include "Enitity.h"
#include "Inventory.h"
#include "Item.h"

class Player
{
    private:
        std::string name;
        int gold;
        int lifes;
        Inventory<std::shared_ptr<Item>> itemInventory;
        Inventory<std::shared_ptr<Tower>> towerInventory;

        Player():gold(100), lifes(3){}

        Player(const Player&) = delete;
        Player& operator=(const Player&) = delete;

    public:
            static Player& getInstance(){
            static Player instance;
            return instance;
        }
        bool afford(int amount) const;
        void spend(int amount);
        void buyTower (std::shared_ptr<Tower> tower, int cost);
        int getGold() const;
        void setName(const std::string name);
        void looseLife();
        int getLives() const;
        bool isAlive() const;
        std::string getName();
    
Inventory<std::shared_ptr<Item>>&  getItemInventory()  ;
        
        Inventory<std::shared_ptr<Tower>>& getTowerInventory() ;
        void addGold(int add);
};