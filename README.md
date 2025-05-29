# Tower Defense POO – Apără Regatul!

## Introducere: O Poveste Fantasy

Într-o țară îndepărtată, unde cerurile sunt pictate în nuanțele magiei și pământul tremură sub pașii hoardelor monstruoase, un regat stă pe marginea distrugerii. Regatul, cândva pașnic, este sub asediu din partea unor valuri de creaturi înspăimântătoare—zombi, orci și goblini. Singura speranță se află în mâinile unui jucător curajos, care trebuie să plaseze strategic și să îmbunătățească turnuri pentru a apăra ultima fortăreață a regatului.

**Tu, jucătorul, ești eroul ales.** Înarmat cu aur și curaj, trebuie să construiești și să gestionezi o serie de turnuri—Turnuri de Arcași, Turnuri cu Tunuri, Turnuri Magice și Turnuri de Gheață—pentru a respinge valurile neîncetate de monștri. Vei face față provocării și vei salva regatul, sau monștrii vor copleși apărarea ta?

---

## Cum Funcționează Jocul

- **Obiectiv:** Apără-ți turnurile de valurile de monștri. Fiecare val aduce inamici mai puternici. Supraviețuiește prin mai multe runde pentru a câștiga jocul.
- **Turnuri:** Plasează și îmbunătățește turnuri pentru a ataca monștri. Fiecare tip de turn are abilități și raze de atac unice.
- **Monștri:** Monștrii se îndreaptă spre turnurile tale. Dacă ajung la un turn, îl atacă. Dacă HP-ul unui turn scade la zero, acesta este distrus.
- **Jucător:** Gestionează-ți cu înțelepciune aurul pentru a îmbunătăți turnurile și a-ți păstra apărarea puternică. Dacă prea mulți monștri trec prin apărare, pierzi vieți—și în cele din urmă, jocul.

---

## Caracteristici Cheie și Implementare

### Structura Codului

Codul este organizat în fișiere separate de antet (.h) și sursă (.cpp) pentru claritate și modularitate.  
**Exemple:**
- `Player.h` / `Player.cpp` definesc clasa Player.
- `Enitity.h` / `Entity.cpp` definesc clasele Tower, Monster și derivatele lor.
- `Item.h` / `Item.cpp` definesc itemele și comportamentul lor.
- `Inventory.h` / `Inventory.cpp` – template pentru gestionarea inventarului.
- `shop.h` / `Shop.cpp` – logica magazinului.
- `GameException.h` – ierarhie de excepții personalizate.

### Moștenire și Polimorfism

Jocul utilizează o ierarhie robustă de moștenire:
- **Clasa de bază:**  
  - `Entity` pentru toate entitățile de pe hartă.
  - `Tower` și `Monster` moștenesc din `Entity`.
- **Clase derivate:**  
  - `ArcherTower : public Tower`
  - `Goblin : public Monster`
- **Exemplu din cod:**
    ```cpp
    class Tower : public Entity { ... };
    class ArcherTower : public Tower { ... };
    class Monster : public Entity { ... };
    class Goblin : public Monster { ... };
    ```

### Funcții Virtuale

- **Funcții virtuale pure:**  
  - `virtual void update() = 0;` în `Entity`
  - `virtual void attack(std::shared_ptr<Monster>&) = 0;` în `Tower`
  - `virtual void use() = 0;` în `Item`
- **Apelare prin pointer la clasa de bază:**  
    ```cpp
    for (auto& tower : towers) {
        if (tower) tower->attack(monster);
    }
    ```

### Constructori Virtuali (Clone)

- **Notă:** În codul actual, funcții de tip `clone()` nu sunt implementate, dar pot fi adăugate ușor pentru a permite copierea polimorfă a turnurilor sau monștrilor.

### Excepții Personalizate

- **Ierarhie proprie derivată din `std::exception`:**
    ```cpp
    class GameException : public std::runtime_error { ... };
    class InvalidNameException : public GameException { ... };
    class NotEnoughGoldException : public GameException { ... };
    ```
- **Utilizare cu sens:**
    ```cpp
    if (playerName.empty()) {
        throw GameException("Invalid name! Exiting.");
    }
    ```
    Blocuri `try/catch` în [main.cpp](http://_vscodecontentref_/0):
    ```cpp
    try {
        // ...
    } catch (const GameException& ex) {
        std::cerr << "Game error: " << ex.what() << "\n";
    }
    ```

### Smart Pointers

- **Folosire extensivă:**  
  - `std::shared_ptr<Tower>`, `std::shared_ptr<Monster>`, `std::shared_ptr<Item>`
  - Vectori de smart poineri pentru gestionarea memoriei.

### Funcții și Atribute Statice, Template

- **Clasă template:**  
    ```cpp
    template <typename T>
    class Inventory { ... };
    ```
- **Funcție statică (singleton):**
    ```cpp
    static Player& getInstance();
    static Inventory<T>& getInstance();
    ```
- **Funcție template:**
    ```cpp
    template<typename Predicate> int countIf(Predicate pred) const;
    ```

### Utilizarea STL

- **Vectori și algoritmi:**  
    ```cpp
    std::vector<std::shared_ptr<Monster>> monsters;
    monsters.erase(
        std::remove_if(monsters.begin(), monsters.end(),
            [](const std::shared_ptr<Monster>& m) { return m->isDead() || m->getX() < 0; }),
        monsters.end()
    );
    ```

### Design Patterns

- **Singleton:**  
  - [Player](http://_vscodecontentref_/1) și [Inventory<T>](http://_vscodecontentref_/2) folosesc singleton pentru a asigura o singură instanță globală.
- **Factory:**  
  - `ItemFactory` creează iteme pe baza unui string.
    ```cpp
    static std::shared_ptr<Item> createItem(const std::string& itemType);
    ```
  ### Template:
  - [Inventory<T>](http://_vscodecontentref_/3) permite gestionarea generică a oricărui tip de obiect.

---
### Apelare funcții virtuale prin pointer la clasa de bază

```cpp
for (auto& tower : towers) {
    if (tower) tower->attack(monster);
}
```
Bibliografie:
https://pisaucer.github.io/book-c-plus-plus/Beginning_Cpp_Through_Game_Programming.pdf
https://gameprogrammingpatterns.com/contents.html
https://www.geeksforgeeks.org/shared_ptr-in-cpp/
https://www.geeksforgeeks.org/unique_ptr-in-cpp/
https://www.geeksforgeeks.org/how-to-clear-console-in-cpp/

