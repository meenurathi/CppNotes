#include<iostream>
#include<iterator>
#include<algorithm>
#include<vector>
#include<random>
#include<ctime>
#include<typeinfo>
using list_type = std::vector<int>;
namespace config{
    constexpr int min{2};
    constexpr int max{4};
    constexpr int maxWrongAnswer{4};
};
auto getRandomInt(int min, int max){
    std::mt19937 rseed{static_cast<std::mt19937::result_type>(std::time(nullptr))};
    std::uniform_int_distribution rrand{min, max};
    return rrand(rseed);
}
void printNumbers(list_type numbers){
    std::cout<<"printNumbers\n";
    for(auto const &num : numbers){
         std::cout<<num<<" ";
    }
    std::cout<<"\n";
}
list_type generateNumbers(int start, int count, int multiplier){
    list_type numbers(static_cast<list_type::size_type>(count));
    int i{start};
    for( auto &number : numbers){
        number = ((i *i) * multiplier);
        i++;
    }
  //  printNumbers(numbers);
    return numbers;
}

list_type  generateUserNumbers(int multiplier){
    int n{}, count{};
    std::cout<<"Start Where?\n";
    std::cin>>n;
    std::cout<<"How many?\n";
    std::cin>>count;
    return generateNumbers(n, count, multiplier);
}
int getUserGuess(){
    int guess{};
    std::cout<<"> ";
    std::cin>>guess;
    return guess;
}
bool findAndRemove(list_type &numbers, int guess){
   auto found{ std::find(numbers.begin(), numbers.end(), guess)};
   if(found == numbers.end()){
        return false;
    }else{
      //  std::cout<<"found = "<<*found<<"\n";
        numbers.erase(found);
        return true;
    }
}
void printSuccess(list_type::size_type numbersLeft){
    if(numbersLeft){
        std::cout << numbersLeft << " number(s) left.\n";
    }else{
        std::cout << "You found all numbers, good job!\n";
    }
}
int getClosest(list_type& numbers, int guess){
    return *std::min_element(numbers.begin(), numbers.end(), [=](int a, int b){
       // std::cout<<"a="<<a<<" b="<<b<<" g="<<guess<<"\n";
        return (std::abs(a - guess) < std::abs(b - guess));
    });
}
void printFailure(list_type& numbers, int guess){
    auto closest = getClosest(numbers, guess);
     std::cout << guess << " is wrong!";
     //std::cout<<closest<<" "<<std::abs(closest - guess)<< " " << config::maxWrongAnswer<<"\n";
    if(std::abs(closest - guess) <= config::maxWrongAnswer){
        std::cout << " Try " << closest << " next time.\n";
    }else{
          std::cout << '\n';
    }
}
void printTask(int count, int multiplier){
    std::cout<<"I generated "<<count<<" square numbers."
             " Do you know what each number is after multiplying it by "<<multiplier<<"\n?";
}
bool playRound(list_type & numbers){
    int guess{getUserGuess()};
    if(findAndRemove(numbers, guess)){
        printSuccess(numbers.size());
        return !numbers.empty();
    }else{
        printFailure(numbers, guess);
        return false;
    }
}
int main(){

    int multiplier{getRandomInt(config::min,config::max)};
    auto numbers{generateUserNumbers(multiplier)};
    std::cout<<"\n size= "<<numbers.size()<<"\n";
    printNumbers(numbers);
    printTask(numbers.size(), multiplier);
    while(playRound(numbers));
    return 0;
}