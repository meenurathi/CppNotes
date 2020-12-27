 #include <algorithm>
#include <array>
#include <cassert>
#include <ctime>
#include <iostream>
#include <random>

// Maximum score before losing.
static constexpr int maximumScore{ 21 };
 
// Minimum score that the dealer has to have.
constexpr int minimumDealerScore{ 17 };
class Card{
public:
    enum class Suit
    {
    SUIT_CLUB,
    SUIT_DIAMOND,
    SUIT_HEART,
    SUIT_SPADE,
    
    MAX_SUITS
    };
    
    enum class Rank
    {
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
    RANK_9,
    RANK_10,
    RANK_JACK,
    RANK_QUEEN,
    RANK_KING,
    RANK_ACE,
    
    MAX_RANKS
    };
    Card()= default;
    Card(Rank r, Suit s):m_rank{r}, m_suit{s}{}
    void print() const{
    switch (m_rank)
    {
        case Rank::RANK_2:
            std::cout << '2';
            break;
        case Rank::RANK_3:
            std::cout << '3';
            break;
        case Rank::RANK_4:
            std::cout << '4';
            break;
        case Rank::RANK_5:
            std::cout << '5';
            break;
        case Rank::RANK_6:
            std::cout << '6';
            break;
        case Rank::RANK_7:
            std::cout << '7';
            break;
        case Rank::RANK_8:
            std::cout << '8';
            break;
        case Rank::RANK_9:
            std::cout << '9';
            break;
        case Rank::RANK_10:
            std::cout << 'T';
            break;
        case Rank::RANK_JACK:
            std::cout << 'J';
            break;
        case Rank::RANK_QUEEN:
            std::cout << 'Q';
            break;
        case Rank::RANK_KING:
            std::cout << 'K';
            break;
        case Rank::RANK_ACE:
            std::cout << 'A';
            break;
        default:
            std::cout << '?';
            break;
        }
        
        switch (m_suit)
        {
        case Suit::SUIT_CLUB:
            std::cout << 'C';
            break;
        case Suit::SUIT_DIAMOND:
            std::cout << 'D';
            break;
        case Suit::SUIT_HEART:
            std::cout << 'H';
            break;
        case Suit::SUIT_SPADE:
            std::cout << 'S';
            break;
        default:
            std::cout << '?';
            break;
        }
    }
    int value() const{
        if (m_rank <= Rank::RANK_10)
        {
            return (static_cast<int>(m_rank) + 2);
        }
        
        switch (m_rank)
        {
        case Rank::RANK_JACK:
        case Rank::RANK_QUEEN:
        case Rank::RANK_KING:
            return 10;
        case Rank::RANK_ACE:
            return 11;
        default:
            assert(false && "should never happen");
            return 0;
        }

    }
private:
    Rank m_rank{};
    Suit m_suit{};
};
class Deck{
  public: 
    using Deck_Type= std::array<Card, 52>;
    using Index_Type = Deck_Type::size_type;

    Deck(): m_cardIndex{}{
        auto maxSuit{static_cast<Index_Type>(Card::Suit::MAX_SUITS)};
        auto maxRank{static_cast<Index_Type>(Card::Rank::MAX_RANKS)};
        auto pos{0};
        for(Index_Type s{0}; s< maxSuit; s++){
            for (Index_Type r{0}; r< maxRank; r++){
              m_deck[pos++] = {static_cast<Card::Rank>(r),static_cast<Card::Suit>(s)};
            }
        }
    }
    void print() const{
        for(const auto & card : m_deck){
            card.print();
            std::cout<<" ";
        }
        std::cout<<"\n";
    }
    void shuffle() {
        std::mt19937 mt{static_cast<std::mt19937::result_type>(time(nullptr))};
        std::shuffle(m_deck.begin(), m_deck.end(), mt);
        m_cardIndex = 0;
    }
    const Card & dealCard(){
      return m_deck[m_cardIndex++];
    }
private:
    Deck_Type   m_deck{};
    Index_Type  m_cardIndex{};
};

class Player{
  int m_score{};
public:
  Player(int score=0):m_score(score){}
  void drawCard(Deck & deck){
    m_score +=deck.dealCard().value();
  }
  bool isBust(){
    return (m_score == maximumScore);
  }
  int score() const{
    return m_score;
  }
};
bool playerWantsHit()
{
  while (true){
    std::cout << "(h) to hit, or (s) to stand: ";
     char ch{};
    std::cin >> ch; 
    switch (ch)
    {
    case 'h':
      return true;
    case 's':
      return false;
    }
  }
}
 
// Returns true if the player went bust. False otherwise.
bool playerTurn( Deck& deck, Player& player)
{
  while (true){
    std::cout << "You have: " << player.score() << '\n';
     if(player.isBust()){
      return true;
    } else {
      if(playerWantsHit()){
        player.drawCard(deck);
      }else {
        // The player didn't go bust.
        return false;
      }
    }
  }
}
 
// Returns true if the dealer went bust. False otherwise.
bool dealerTurn( Deck& deck, Player& dealer)
{
  while (dealer.score() < minimumDealerScore){
    dealer.drawCard(deck);
  } 
  return dealer.isBust();
}
 
bool playBlackjack( Deck& deck)
{
 
  Player dealer{};
  dealer.drawCard(deck); 
  std::cout << "The dealer is showing: " << dealer.score() << '\n';
 
  Player player{};
  player.drawCard(deck);
  player.drawCard(deck);

  if (playerTurn( deck, player))
  {
    return false;
  }
 
  if (dealerTurn( deck, dealer))
  {
    return true;
  }
 
  return (player.score() > dealer.score());
}
int main()
{
  const Card cardQueenHearts{ Card::Rank::RANK_QUEEN, Card::Suit::SUIT_HEART };
  cardQueenHearts.print();
  std::cout << " has the value " << cardQueenHearts.value() << '\n';
  Deck deck;
  deck.print();
  deck.shuffle();
  deck.print();
  std::cout << "The first card has value: " << deck.dealCard().value() << '\n';
  std::cout << "The second card has value: " << deck.dealCard().value() << '\n';
  
  Player player{};
  Player dealer{};
 
  player.drawCard(deck);
  dealer.drawCard(deck);
 
  std::cout << "The player drew a card with value: " << player.score() << '\n';
  std::cout << "The dealer drew a card with value: " << dealer.score() << '\n';
 
  if (playBlackjack(deck))
  {
    std::cout << "You win!\n";
  }
  else
  {
    std::cout << "You lose!\n";
  }
  return 0;
}
