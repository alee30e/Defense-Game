#include "Item.h"


void Bomb::use()
{
    std::cout<<"Bomba\n";
}

int Bomb::getPrice()const
{
            return 20;
}
std::string Bomb::getName() const {
            return "Bomba";
}

void Potion::use()  {
            std::cout<<"Turn heal at: "<<healAmount<<"\n";
        }
        int Potion::getPrice()const{
            return 20;
        }

        std::string Potion:: getName() const {
            return "Potion\n";
        }

        void Trap::use() 
        {
            std::cout<<"Trap\n";
        }

        std::string Trap::getName() const
        {
            return  "Trap";
        }
         int Trap::getPrice()const
        {
            return 20;
        }



int TowerItem::getPrice() const
        {
            return 20;
        }

        void TowerItem::use() 
        {
            std::cout<<"Aseaza turnul: "<<tower->getSymbol()<<"\n";
        }
        std::string TowerItem::getName() const 
        {
            return "Tower: " + tower->getSymbol();
        }

        std::shared_ptr<Tower> TowerItem::getTower() const{
            return tower;
        }