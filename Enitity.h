#pragma once
#include<iostream>
#include<vector>
#include<memory>
class Monster;
class Entity
{
    protected:
    int x;
    int y;
    public:
        Entity() : x(0), y(0){};
        Entity(int x, int y):x(x), y(y){}
        virtual void update() = 0;
        virtual ~Entity(){}
};

class Tower : public Entity
{
    protected:
    int level;
    int damage;
    int HP;
    int price;
    std::string symbol;

    public:
        Tower(int baseDamage, const std::string& sym);        
        virtual void attack(std::shared_ptr<Monster>& monster) = 0;
        virtual void getsAttacked(std::shared_ptr<Monster>& monster) =0;
        virtual void upgrade()=0;
        virtual void takeDamage(int)=0;
        virtual bool isAlive() const=0;
        virtual bool isDead() const = 0;
        virtual std::string getSymbol() const=0;
        virtual int getRange()=0;
        virtual int getDamage() const = 0;
        virtual void heal(int amount);

        void setPosition(int x, int y) ;
        int getX();
        int getY();
        int getHP();
        int getLevel();
        
        virtual ~Tower() = default;

};


class ArcherTower : public Tower
{
    public:
        ArcherTower();
        virtual void attack(std::shared_ptr<Monster>& monster) ;
        virtual void getsAttacked(std::shared_ptr<Monster>& monster);
        virtual void upgrade()override;
        virtual void takeDamage(int)override;
        virtual bool isAlive() const override;
        virtual bool isDead() const override;
        virtual std::string getSymbol() const override;
        virtual void update() override;
        virtual int getRange() override;
        virtual int getDamage() const override ;
        // virtual void heal() override;
 
    //     {
    //     // Example logic: maybe regenerate HP or check for something
    //     std::cout << "ArcherTower updated.\n";
    // }

};

class Monster : public Entity
{
    protected:
        int hp;
        int step;
        int damage;
        std::string symbol;
        mutable bool addedGold = false;
        mutable bool reachedTower = false;
    public:
        Monster(int startX, int startY, int hp, int damage, const std::string& symbol);

        virtual void move();
        virtual void takeDamage(int dmg);      // Reduce HP
        virtual bool isAlive() const;          // Check if still alive
        virtual int getX() const;       // Return position on the map
        virtual int getY() const; 
        // virtual void attack() const;
        virtual std::string getSymbol() const; // Symbol for display

        virtual int getHP() const;
        virtual int getDamage() const;
        virtual void update() override;
        virtual void setAddedGold();
        virtual bool getAddedGold();
        bool hasReachedTower() ;
        bool getReachedTower();
        void setPosition(int x, int y) ;
        bool isDead();

        virtual ~Monster() = default;
};

class Goblin : public Monster
{
    public:
    Goblin(int x, int y) : Monster(x, y, 100, 15, "G"){};
    void move() override;
    void update() override;
};

class Zombie: public Monster
{
    public:
        Zombie(int x, int y): Monster(x, y, 75, 10, "Z"){};
        void move() override;
        void update() override;
};

class Tank: public Monster
{
        public:
        Tank(int x, int y): Monster(x, y, 110, 10, "T"){};
        void move() override;
        void update() override;
};