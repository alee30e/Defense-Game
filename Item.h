#pragma once
#include<iostream>
#include<vector>
#include<memory>
#include "Enitity.h"
class Item
{
    public:
        virtual void use() = 0;
        virtual std::string getName() const = 0;
        virtual int getPrice() const = 0;
        virtual ~Item() = default;
};

class Bomb: public Item
{
    public:
        void use() override;
        virtual int getPrice()const override;

        std::string getName() const override;
};

class Potion:public Item
{
    int healAmount;

    public:
        Potion(int heal) : healAmount(heal){}

        void use() override ;
        virtual int getPrice()const override;

        std::string getName() const override;
};

class Trap:public Item{
    public:
        void use() override;

        std::string getName() const override;
        virtual int getPrice()const override;
};

class TowerItem: public Item 
{
    private:
        std::shared_ptr<Tower> tower;

    public:    
        TowerItem(std::shared_ptr<Tower> t) : tower(std::move(t)){}

        virtual int getPrice() const override;

        void use() override;
        std::string getName() const override;

        std::shared_ptr<Tower> getTower() const;
            
};
