#include<iostream>
#include<string>
#include<string_view>
#include<ctime>
#include<array>
#include<cstdlib>
using std::cout;
using std::cin;

 class Monster{
public:
    enum class Type{
        dragon,
        goblin,
        ogre,
        orc,
        skeleton,
        troll,
        vampire,
        zombie, 
        max_monster_types
    };
    Monster(Type type = Type::zombie, std::string name ="zombie", std::string roar= "", int hitPoints=0)
            :m_type{type}, m_name{name}, m_roar{roar}, m_hitPoints{hitPoints}{}
    std::string_view getTypeString(Type type)const {
        switch(type){
        case Type::dragon: return "dragon";
        case Type::goblin: return "goblin";
        case Type::ogre: return "ogre";
        case Type::orc: return "orc";
        case Type::skeleton: return "skeleton";
        case Type::troll: return "troll";
        case Type::vampire: return "vampire";
        case Type::zombie: return "zombie";
        default:
            return "?????";
        }
    }
    void print()const {
        cout<<m_name<<" the "<<getTypeString(m_type)<<" has "<<m_hitPoints<<" hit points and says *"<<m_roar<<"*\n";
    }
private:
     Type m_type{};
     std::string m_name{};
     std::string m_roar{};
     int m_hitPoints{};
 };
 class MonsterGenerator{
     
	// Generate a random number between min and max (inclusive)
	// Assumes std::srand() has already been called
	// Assumes max - min <= RAND_MAX
     static int getRandomNumber(int min, int max){
        static constexpr double fraction{1.0/(static_cast<double>(RAND_MAX) + 1.0)};
        return static_cast<double>(std::rand() * fraction *(max - min +1) +min);
     }
public: 
    static Monster generateMonster(){
        //return {Monster::Type::skeleton, "Bones", "*rattle*", 4};
        auto type {static_cast<Monster::Type>(getRandomNumber(0,static_cast<int>( Monster::Type::max_monster_types)-1))};
        auto hits { getRandomNumber(1,100)};

        static constexpr std::array s_names{"Blarg", "Moog", "Pksh", "Tyrn", "Mort", "Hans"};
        auto name {s_names[static_cast<unsigned int>(getRandomNumber(0, 5))]};
        
        static constexpr std::array s_roars{"*ROAR*", "*peep*", "*squeal*", "*whine*", "*hum*", "*burp*"};
        auto roar {s_roars[static_cast<unsigned int>(getRandomNumber(0, 5))]};
        
        return {type, name, roar, hits};
    }
 };
int main(){
    std::srand(static_cast<unsigned int>(std::time(nullptr)));//// set initial seed value to system clock
    std::rand(); //discard first random value
   	Monster skeleton{ Monster::Type::skeleton, "Bones", "*rattle*", 4 };
	skeleton.print();

    Monster m{ MonsterGenerator::generateMonster() };
	m.print();
    return 0;
}