#include<iostream>
#include<string>
#include<string_view>
#include<array>
#include<cstdlib> //rand, srand
#include<ctime>
int getRandomNumber(int min, int max){
        constexpr static double fraction{1.0/(RAND_MAX +1.0)};
        return {min+ static_cast<int>((std::rand() * fraction * (max - min +1)) )};
}
class Creature{
protected:
    std::string m_name{nullptr};
    char m_symbol{};
    int m_health{};
    int m_damage{};
    int m_gold{};
public:
    Creature( std::string_view name,  char symbol=' ', int health=0, int damage=0, int gold =0)
        :m_name(name), m_symbol(symbol), m_health(health), m_damage(damage), m_gold(gold){}
    Creature(const Creature & src)= default;
    const std::string& getName(){ return m_name;}
    auto getSymbol(){ return m_symbol;}
    auto getHealth(){return m_health;}
    auto getDamage(){return m_damage;}
    auto getGold(){ return m_gold;}

    void reduceHealth(int amt){ m_health -= amt;}
    bool isDead(){ return m_health <= 0;}
    void addGold(int amt){ m_gold +=amt;}
};

class Player: public Creature{
    int m_level{1};
    static constexpr int maxPlayerLevel{20};
public:
    Player(std::string_view name) : Creature{name, '@', 10, 1, 0}, m_level{1}{}
    int getLevel(){ return m_level;}
    void levelUp(){ ++m_level; ++m_damage;}
    bool hasWon(){ return m_level >= maxPlayerLevel;}
};
class Monster: public Creature{
public:
    enum class Type{ dragon, orc, slime , max_types    };
    Monster(Type type) : Creature{getDefaultCreature(type)}{};
    static const Monster getRandomMonster(){//return by value
        int idx{ getRandomNumber(0, static_cast<int>(Type::max_types)-1)};
        return Monster{static_cast<Type>(idx)};//temp monster object
    }
private:
    static const Creature & getDefaultCreature(Type type){
        static const std::array<Creature, static_cast<std::size_t>(Type::max_types)> monsterData{
            {
                { "dragon", 'D', 20, 4, 100},
                { "orc",    'o', 4, 2 , 25},
                { "slime",  's', 1, 1, 10} }   
        };
        return monsterData.at(static_cast<std::size_t>(type));
    }
};
void attackMonster(Player & player, Monster & monster) {
    // Reduce the monster's health by the player's damage
    monster.reduceHealth(player.getDamage());
    std::cout<<"You hit the "<<monster.getName()<<" for "<<player.getDamage()<<".\n";
      // If the monster is now dead, level the player up
    if(monster.isDead()){
        std::cout<<"You have killed the "<<monster.getName()<<"\n";
        player.addGold(monster.getGold());
        player.levelUp();
        std::cout<<"You are now level "<<player.getLevel()<<"\n";
        std::cout<<"You found "<<monster.getGold()<<".\n";
    }
}
void attackPlayer(Player & player, Monster & monster){
    player.reduceHealth(monster.getDamage());
    std::cout<<"The "<<monster.getName()<<" hit you for "<<monster.getDamage()<<".\n";
}
void fightMonster(Player & player){
    //First randomly generate a monster
    Monster m{ Monster::getRandomMonster() };
    std::cout << "A " << m.getName() << " (" << m.getSymbol() << ") was created.\n";
    std::cout<< "You have encountered a "<<m.getName()<<" ("<<m.getSymbol()<<")";
    char choice{};
    while(!player.isDead()  && !m.isDead()){
        std::cout<<"(R)un or (F)ight: ";
        std::cin>>choice;
        if(choice == 'r' || choice == 'R'){
            // 50% chance of fleeing successfully
           if(getRandomNumber(0,1)){
                std::cout<<"You successfully fled.\n";
                return;// success ends the encounter
            }else{
                // Failure to flee gives the monster a free attack on the player
                std::cout << "You failed to flee.\n";
                attackPlayer(player, m);
                continue;
            }
        }else{
            // Player attacks first, monster attacks second
             attackMonster(player, m);
             attackPlayer(player, m);
        }
    }

 }
int main()
{
	std::string name{};
    std::cout<<"Enter your name: ";
    std::cin>>name;
    Player player{name};
    std::cout << "Welcome, "<<player.getName()<<".";
    std::cout<<"\nYou have "<<player.getHealth()<<" and are carrying "<<player.getGold()<<" gold.\n";

   /*  Monster m{ Monster::Type::orc };
	std::cout << "A " << m.getName() << " (" << m.getSymbol() << ") was created.\n"; */
 
    std::srand(static_cast<unsigned int>(time(nullptr)));     // set initial seed value to system clock
    std::rand();//// get rid of first resul

    while (!(player.isDead() || player.hasWon()))
	{
        fightMonster(player);       
	}
    if(player.hasWon()){
        std::cout<<"You won the game with "<<player.getGold()<<" gold!\n";
    }else{
        std::cout<<"You died at level "<<player.getLevel()<<"  and with "<<player.getGold()<<" gold.\n";
        std::cout<<"Too bad you can't take it with you!\n";
    }
	return 0;
}
