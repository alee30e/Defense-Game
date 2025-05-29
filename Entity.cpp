#include "Enitity.h"

void Tower::setPosition(int x, int y) 
        {
            this->x = x;
            this->y = y;
        }
std::string Tower::getSymbol() const {
    return symbol + std::to_string(level);
}

int Tower:: getX(){
    return x;
}

int Tower:: getY(){
    return y;
}
int Tower:: getHP(){
    return HP;
}
void Tower::heal(int amount)
{
    HP+=amount;
}

int Tower:: getLevel(){
    return level;
}
int Tower::getDamage() const{
    return damage;
}
void ArcherTower::attack(std::shared_ptr<Monster>& monster) {
        if (monster && monster->isAlive()) {
            monster->takeDamage(damage);
            std::cout << "[LOG] Tower attacked monster, monster HP: " << monster->getHP() << "\n";
        }
    }

void ArcherTower::getsAttacked(std::shared_ptr<Monster>& monster)
{
    if (monster->getX()==1)
    {
        HP = HP - monster->getDamage();
    }
}
int ArcherTower:: getRange(){
    return 5;
}
int ArcherTower:: getDamage() const
{return damage;}

void ArcherTower::upgrade() {
    level++;
    damage += 5;
    HP += 10;
    std::cout << "Tower upgraded to level " << level << "!\n";
}
void ArcherTower::update(){
    std::cout<<"Turn updatat!\n";
}

void ArcherTower::takeDamage(int dmg) {
    HP -= dmg;
    if (HP < 0) HP = 0;
}

bool ArcherTower::isAlive() const {
    return HP > 0;
}
bool ArcherTower::isDead() const
{
    return !HP;
}
std::string ArcherTower::getSymbol() const {
    return symbol;
}

int Monster::getHP() const
{
    return hp;
}
int Monster::getDamage() const
{
    return damage;
}
void Monster::move() {
    y--;  // Assume movement is left to right on the map
}
void Monster::setAddedGold()
{
    addedGold = true;
}
bool Monster::getAddedGold()
{
    return addedGold;
}

bool Monster::hasReachedTower()
{
                if (y>=1 && reachedTower == 0) 
            { 
                reachedTower = 1;
                return 1;
            }
            return 0; 
}
bool Monster::getReachedTower(){
    return reachedTower;
}
void Monster::takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
}
bool Monster::isAlive() const
{
    return hp;
}

int Monster::getX() const{
    return x;
}
int Monster::getY() const{
    return y;
}
std::string Monster::getSymbol() const
{
    return symbol;
}
void Monster::update()
{
    hp+=10;
}
void Monster::setPosition(int y, int x) 
        {
            this->x = y;
            this->y = x;
        }
bool Monster::isDead()
{
    return !hp;
}
void Goblin::update()
{
    damage+=10;
}

void Goblin::move()
{
    if (reachedTower==0)
    x--;
}
Monster::Monster(int startX, int startY, int health, int dmg, const std::string& sym)
    : Entity(startX, startY), hp(health), damage(dmg), symbol(sym) {}

Tower::Tower(int baseDamage, const std::string& sym)
    : Entity(), level(1), damage(baseDamage), symbol(sym), HP(100) {}

ArcherTower::ArcherTower() : Tower(20, "A") {}

void Zombie::update()
{
    damage+=5;
    hp+=5;
}
void Zombie::move()
{
    if (reachedTower == 0 && x >3)
    x-=2;
    else
    if (reachedTower == 0 && x<=3)
    x-=1; 
}


void Tank::update()
{
    damage+=15;
    hp+=15;
}
void Tank::move()
{
    if (reachedTower == 0)
    x--;
}