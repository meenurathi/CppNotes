#include<algorithm>
#include<vector>
/* syntax
    [capture clause](para list)->retuntype {
        statements
    }
    return type auto is assumed by default */

 /* >>Types of a lambda :   */
    //1st function literal
         std::vector<int> vec(10,4);
        int main1(){        
            std::all_of(vec.begin(), vec.end(), [](int x){ return x%2 == 0; });
            return 0; }

    //2nd  generic lambdas.(when one of its parameter is of type auto)
        #include<array>
        #include<iostream>
        constexpr std::array months{"jan", "feb", "mar", "april", "may", "june", "july"};
        const auto sameLetter{std::adjacent_find(months.begin(), months.end(), 
                            [](const auto &m1, const auto &m2){
                                return (m1[0] == m2[0]); })};
        int main2(){
            std::cout<<*sameLetter<< " " <<*std::next(
            sameLetter)<<std::endl;
            return 0;};

/*>> storing a lambda as  named lambda for easy readability */
    //1) as real type > functor (preferred: no overhead compared to std::function.)
        /*  In actuality, lambdas aren’t functions (because C++ does not support nested functions). 
    They’re a special kind of object called a functor.Functors are objects that contain an 
    overloaded operator() that make them callable like a function. */
        void func(){
            auto isEven {[](int x){ return x%2 == 0;} };
            std::all_of(vec.begin(),vec.end(), isEven);
        }

    //2 function pointer ---------> only if they have empty capture clause
        double (*addNumbers)(double , double){
            [](double a, double b){
                return(a+b);
            }
        };

    //3rd  using std functions  -->used both with/without  capture clause
        #include<functional>
            std::function addNumbers2{[](double a, double b){ return(a+b);  } };

        /* >> rules :Use auto when initializing variables with lambdas,
            and std::function if you can’t initialize the variable with the lambda.   
            we’re passing a lambda to a function as a parameter and the caller determines what lambda will be passed in*/
            // We don't know what fn will be. std::function works with regular functions and lambdas.
            void repeat(int repetitions, const std::function<void(int)>& fn)//auto lambda cannot be used here
            {
                for (int i{ 0 }; i < repetitions; ++i)
                {
                    fn(i);
                }
            }

/*Note: Generic lambdas and static variables
    a unique lambda will be generated for each different type that auto resolves to. 
    The following example shows how one generic lambda turns into two distinct lambdas:
    if the generic lambda uses static duration variables,those variables are not shared between the generated lambdas. */
        #include <string_view>
        int main3()
        {
            // Print a value and count how many times @print has been called.
            auto print{
                [](auto value) {
                static int callCount{ 0 };
                std::cout << callCount++ << ": " << value << '\n';
                }
            }; 
            print("hello"); // 0: hello
            print("world"); // 1: world 
            print(1); // 0: 1
            print(2); // 1: 2 
            print("ding dong"); // 2: ding dong 
            return 0;
        }

/* >>>Return type deduction and trailing return types
    If return type deduction is used, a lambda’s return type is deduced 
    from the return-statements inside the lambda. If return type inference is used, 
    all return statements in the lambda must return the same type (otherwise the compiler won’t know which one to prefer).
    In the case where we’re returning different types, we have two options:
    1) Do explicit casts to make all the return types match, or
    2) explicitly specify a return type for the lambda, and let the compiler do implicit conversions.(prefered) */

// <functional> header.    //contains addition, negation, or comparison >>std::greater{}/*

// ***********************************************************************************************************/
/*  //lambdas can only access specific kinds of identifiers:
     1) global identifiers, 
     2) entities that are known at compile time, 
     3) entities with static storage duration.  */

            int i{};
            static int j{};            
            int getValue()
            {
                return 0;
            }
            
            int main15()
            {
                int a{};
                constexpr int b{};
                static int c{};
                static constexpr int d{};
                const int e{};
                const int f{ getValue() };
                static const int g{}; 
                static const int h{ getValue() }; 
                
                [](){
                    // Try to use the variables without explicitly capturing them.
                // a;X not allowed-->. a has automatic storage duration.
                    b;//Yes. b is usable in a constant expression
                    c;//yes. c has static storage duration.
                    d;//yes
                    e;//yes. e is usable in a constant expression.
                //  f;//NO f‘s value depends on getValue, which might require the program to run.
                    g;//yES
                    h;//Yes  h has static storage duration.
                    i;//Yes is a global variable.
                    j;//Yes j is accessible in the entire file
                }();
                
                return 0;
            }

/* Lambda captures : is used to (indirectly) 
    >give a lambda access to variables available in the surrounding scope that it normally would not have access to. 
    >When the compiler encounters a lambda definition, it creates a custom object definition for the lambda.
     Each captured variable becomes a data member of the object, 
     which are clones of the outer scope variables, not the actual variables.*/

    //type 1) pass by value : 
            //Captures default to const value
                int main4(){
                    int ammo{10};
                    auto shoot{[ammo](){
                        //--ammo; ----> compilation error, cannot change value of const clone of captured variable
                        std::cout<<ammo<<" shorts left!! \n";
                    }};
                    shoot();
                    std::cout<<ammo<<" shorts left!! \n";
                    return 0;
                }
    //type 2) Mutable capture by value:
             /*  mark the lambda as mutable, to allow modifications of variables that were captured by value. 
               The mutable keyword in this context removes the const qualification from
               >> all variables captured by value  */
                int main5(){
                    int ammo{10};
                    auto shoot{[ammo]() mutable{ // removes const from all captured var
                        --ammo; // Changes to ammo will affect main's ammo
                        std::cout<<ammo<<" shorts left!! \n";
                    }};
                    shoot();
                    std::cout<<ammo<<" shorts left!! \n";
                    return 0;
                } 
               /* output >>  9 shorts left!!
                            10 shorts left!! */

    //type 3) :Capture by reference
        /* variables that are captured by reference are non-const, unless the variable they’re capturing is const 
           It should be preferred over capture by value, whenever you would normally prefer passing an argument 
           to a function by reference (e.g. for non-fundamental types).*/
                int main6(){
                    int ammo{10};
                    auto shoot{[&ammo](){ 
                        --ammo; 
                        std::cout<<ammo<<" shorts left!! \n";
                        --ammo;
                    }};
                    shoot();
                    std::cout<<ammo<<" shorts left!! \n";
                    return 0;
                } 
               /* output >>  9 shorts left!!
                            8 shorts left!! */
      // Type 4) Capturing multiple variables
            #include<vector>
            int main7(){
                constexpr std::array CEnemy {"pluto" , "mack", "rayan"};
                int health{ 33 };
                int armor{ 100 };
                std::vector<const char *> enemies (CEnemy.begin(), CEnemy.end()) ;
                 // Capture health and armor by value, and enemies by reference
                [health, armor, &enemies](){ enemies.push_back(" mario");};
                return 0;
            }
     // Type 5) Default captures
            /* To capture all used variables by value, use a capture value of =.
               To capture all used variables by reference, use a capture value of &. */
                int main8()
                {
                    std::array areas{ 100, 25, 121, 40, 56 };                    
                    int width{};
                    int height{};                    
                    std::cout << "Enter width and height: ";
                    std::cin >> width >> height;                    
                    auto found{ std::find_if(areas.begin(), areas.end(),
                                            [=](int knownArea) { // will default capture width and height by value
                                                return (width * height == knownArea); // because they're mentioned here
                                            }) };
                    if (found == areas.end())                    {
                        std::cout << "I don't know this area :(\n";
                    }else{
                        std::cout << "Area found :)\n";
                    }                    
                    return 0;
                }
        //type 6) Default captures can be mixed with normal captures
                /* We can capture some variables by value and others by reference, but each variable can only be captured once. */
                int main9(){
                    constexpr std::array CEnemy {"pluto" , "mack", "rayan"};
                    int health{ 33 };
                    int armor{ 100 };
                    std::vector<const char *> enemies (CEnemy.begin(), CEnemy.end()) ;

                    [health, armor, & enemies](){};// Capture health and armor by value, and enemies by reference.
                    [=, &enemies](){} ;//catpure all by const value except enemies as ref
                    [&, health](){};//capture all by ref except health as const value
                   // [&, &armor](){};  // Illegal, we already said we want to capture everything by reference.                        
                   // [=, armor](){};   // Illegal, we already said we want to capture everything by value.                                 
                   // [armor, &health, &armor](){};  // Illegal, armor appears twice.                                     
                   // [armor, &](){};// Illegal, the default capture has to be the first element in the capture group.
                    return 0;
                }
        //type 7 Defining new variables in the lambda-capture :
            /* const by default , do not change in sub call,  unless explicity specied mutable
                Sometimes we want to capture a variable with a slight modification or declare a new variable 
                that is only visible in the scope of the lambda.
                We can do so by defining a variable in the lambda-capture without specifying its type. */
                int main10()
                {
                    std::array areas{ 100, 25, 121, 40, 56 };                    
                    int width{};
                    int height{};                    
                    std::cout << "Enter width and height: ";
                    std::cin >> width >> height;  
                     // Declare a new variable that's visible only to the lambda.
                    // The type of userArea is automatically deduced to int.
                    auto checkArea{[userarea{width * height}](int knownArea) { // will default capture width and height by value
                                           // userarea++; requires mutable lambda
                                            std::cout<<"userarea ="<<userarea<<"\n";
                                            return (userarea == knownArea); // because they're mentioned here
                                  }};                  
                    auto found{ std::find_if(areas.begin(), areas.end(),checkArea )};
                    if (found == areas.end())                    {
                        std::cout << "I don't know this area :(\n";
                    }else{
                        std::cout << "Area found :)\n";
                    }                    
                    width = height = 5;
                    //userarea will not change to 25, it retains init value 
                    found = { std::find_if(areas.begin(), areas.end(),checkArea )};
                    //userarea value will be old one
                    if (found == areas.end())                    {
                        std::cout << "I don't know this area :(\n";
                    }else{
                        std::cout << "Area found :)\n";
                    }              
                    return 0;
                }
                    /* userArea will only be calculated once when the lambda is defined. The calculated area is stored in the lambda
                    object and is the same for every call. If a lambda is mutable and modifies a variable that was defined in the capture,
                    the original value will be overridden. */
         /*  note:  Only initialize variables in the capture if their value is short and their type is obvious. 
                    Otherwise it’s best to define the variable outside of the lambda and capture it. */

/*-----Dangling captured variables:. The captured variables must outlive the lambda. If a variable captured by
   reference dies before the lambda, the lambda will be left holding a dangling reference. */
            
            // returns a lambda
            auto makeWalrus(const std::string& name){
            // Capture name by reference and return the lambda.
                return [&]() {
                    std::cout << "I am a walrus, my name is " << name << '\n'; // Undefined behavior
                };
            }            
            int main11(){
                // Create a new walrus whose name is Roofus.
                // sayName is the lambda returned by makeWalrus.
                auto sayName{ makeWalrus("Roofus") };        
                // Call the lambda function that makeWalrus returned.
                sayName();                
                return 0;
            }

//Unintended copies of mutable lambdas     : Because lambdas are objects, they can be copied                      
            int main12()
            {
                int i{ 0 };                
                // Create a new lambda named count
                auto count{ [i]() mutable {
                    std::cout << ++i << '\n';
                } };
                
                count(); // invoke count
                
                auto otherCount{ count }; // create a copy of count
                
                // invoke both count and the copy
                count();
                otherCount();
                
                return 0;
            }
 /*            Output
                    1
                    2
                    2 */
    /*     Rather than printing 1, 2, 3, the code prints 2 twice. When we created otherCount as a copy of count,
         we created a copy of count in its current state. count‘s i was 1, so otherCount‘s i is 1 as well.
          Since otherCount is a copy of count, they each have their own i. */

/* //std::reference_wrapper // std::ref
            By wrapping our lambda in a std::reference_wrapper, whenever anybody tries to make a copy of our lambda, 
            they’ll make a copy of the reference instead, which will copy the reference rather than the actual object. */

            void invoke(const std::function<void(void)>& fn) {
                fn();
            }            
            int main13() {
                int i{ 0 };            
                // Increments and prints its local copy of @i.
                auto count{ [i]() mutable {
                std::cout << ++i << '\n';
                } };            
                invoke(count);
                invoke(count);
                invoke(count);            
                return 0;
            }
            /* Output:
                        1
                        1
                        1 */
        //BETTER.USE STD::REF 
            int main14(){
                int i{ 0 };            
                // Increments and prints its local copy of @i.
                auto count{ [i]() mutable {
                    std::cout << ++i << '\n';
                } };            
                // std::ref(count) ensures count is treated like a reference
                // thus, anything that tries to copy count will actually copy the reference
                // ensuring that only one count exists
                invoke(std::ref(count));
                invoke(std::ref(count));
                invoke(std::ref(count));
                return 0;
            }
         /*    Our output is now as expected:
                1
                2
                3 */
/* Rule

Standard library functions may copy function objects (reminder: lambdas are function objects).
 If you want to provide lambdas with mutable captured variables, 
 pass them by reference using std::ref. */