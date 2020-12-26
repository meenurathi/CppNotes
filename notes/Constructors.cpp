#include<iostream>
/* note: 
When all members of a class (or struct) are public, we can use aggregate initialization to 
initialize the class (or struct) directly using an initialization list or uniform initialization: */
        class Foo {
        public:
            int m_x;
            int m_y;
        };        
        int main2(){
            //can only work if all class members are public
            Foo foo1 = { 4, 5 }; // initialization list
            Foo foo2 { 6, 7 }; // uniform initialization      
            foo1 = foo2;//unused var   
            return 0;
        }

/* ... Default Constructor --> no parameters */
     /* Implicit construtor --> automatically generated default construtor by compiler
        allows us to create an object with no arguments, but doesn’t initialize these arg if initial values of variabe is not provide 
        it has to be mandatory user defined , incase class has 1 or more parametric construtor */
     
        //To make sure the member variables get initialized, we can initialize them at their declaration.
        /*  initializing members using non-static member initialization requires using either an equals sign, or a brace (uniform) initializer -- the direct initialization form doesn’t work here. */
        class Date1
        {
        private:
            int m_year{ 1900 };
            int m_month{ 1 };
            int m_day{ 1 };
        };

    /*To allow construction of a Date without arguments, either add default arguments to the constructor, add an empty default constructor, or explicitly add a default constructor:*/
        class Date2
        {
        private:
            int m_year{ 1900 };
            int m_month{ 1 };
            int m_day{ 1 };
        
        public:
            // Tell the compiler to create a default constructor, even if
            // there are other user-provided constructors.
            Date2() = default;
        
            Date2(int year, int month, int day) // normal non-default constructor
            {
                m_year = year;
                m_month = month;
                m_day = day;
            }
        };
        
        int main1()
        {
            Date2 date{}; // date is initialized to Jan 1st, 1900
            Date2 today{ 2020, 10, 14 }; // today is initialized to Oct 14th, 2020
        
            return 0;
        }
        /*Using = default is almost the same as adding a default constructor with an empty body. The only difference is that = default allows us to safely initialize member variables even if they don’t have an initializer:*/
        class Date3
        {
        private:
            // Note: No initializations at member declarations
            int m_year;
            int m_month;
            int m_day;
        
        public:
            // Explicitly defaulted constructor
            Date3() = default;
        };
        
        class Date4
        {
        private:
            // Note: No initializations at member declarations
            int m_year;
            int m_month;
            int m_day;
        
        public:
            // Empty user-provided constructor
            Date4() {};
        };
        class Date5
        {
        private:
            // Note:  initializations at member declarations
            int m_year{2};
            int m_month{3};
            int m_day{4};
        
        public:
            // Empty user-provided constructor
            Date5() :m_year(5){}
            Date5(int m_year) :m_year(m_year){}
            void print(){
                std::cout<<m_year<<" "<<m_month<<" "<<m_day<<"\n";
            }
        };
        
        int main3()
        {
            Date3 today{}; // today is 0, 0, 0
            Date4 tomorrow{}; // tomorrows's members are uninitialized
            Date5 other{};
            other.print();
            return 0;
        }
    //...Rule : If you have constructors in your class and need a default constructor that does nothing, use = default.

/* //... parametric  Constructor --> with parameter 
        supports both Direct and uniform initialization(preferred oer direct)
            Fraction fiveThirds{ 5, 3 }; // List initialization, calls Fraction(int, int)
            Fraction threeQuarters(3, 4); // Direct initialization, also calls Fraction(int, int) */

 /* .... Rule -->Favor brace initialization to initialize class objects. */

/*  //Copy initialization using equals with classes (not preferred)
        // Constructor with two parameters, one parameter having a default value
            Fraction(int numerator, int denominator=1)
            {
                assert(denominator != 0);
                m_numerator = numerator;
                m_denominator = denominator;
            }
        Fraction six = Fraction{ 6 }; // Copy initialize a Fraction, will call Fraction(6, 1)
        Fraction seven = 7; // Copy initialize a Fraction.  The compiler will try to find a way to 
        convert 7 to a Fraction, which will invoke the Fraction(7, 1) constructor. */

/* //When implementing your constructors, consider how you might keep the number of constructors down 
through smart defaulting of values. */

/* ..Classes containing classes
    A class may contain other classes as member variables. By default, when the outer class is constructed, the member variables will have their default constructors called. This happens before the body of the constructor executes. */

//const and reference variables) must be initialized either by default initionlization or initializer list

/* Initializing array members with member initializer lists
Since C++11, you can fully initialize a member array using uniform initialization:
 */
        class Something
        {
        private:
            const int m_array[5];
        
        public:
            Something(): m_array { 1, 2, 3, 4, 5 } // use uniform initialization to initialize our member array
            {
            }
        
        };

/* Initializer list order
variables in the initializer list are not initialized in the order that they are specified in the initializer list. Instead, they are initialized in the order in which they are declared in the class.
 */
/* 
Even though we’ve provided default values for all members, no default constructor has been provided, so we are unable to create Rectangle objects with no arguments. */
            #include <iostream>
            
            class Rectangle
            {
            private:
                double m_length{ 1.0 };
                double m_width{ 1.0 };
            
            public:
            
                // note: No default constructor provided in this example
            
                Rectangle(double length, double width)
                    : m_length{ length },
                    m_width{ width }
                {
                    // m_length and m_width are initialized by the constructor (the default values aren't used)
                }
            
                void print()
                {
                    std::cout << "length: " << m_length << ", width: " << m_width << '\n';
                }
            
            };
            
            int main5()
            {
                //Rectangle x{}; // will not compile, no default constructor exists, even though members have default initialization values            
                return 0;
            }

/* If a default initialization value is provided and the constructor initializes the member via the member initializer list, the member initializer list will take precedence */

/*... Constructors with overlapping functionality 
        if you try to have one constructor call another constructor in this way, it will compile and maybe cause a warning, but it will not work as you expect, and you will likely spend a long time trying to figure out why, even with a debugger. What’s happening is that Foo(); instantiates a new Foo object, which is immediately discarded, because it’s not stored in a variable. */
        class Foo6
        {
        public:
            Foo6()
            {
                // code to do A
            }
        
            Foo6(int value)
            {
                Foo6(); // use the above constructor to do A (doesn't work)//NO WARNING OR COMPILATION ERROR
                // code to do B
            }
        };

/* ..... Delegating constructors (prefered to about duplicate code
        Constructors are allowed to call other constructors. This process is called delegating constructors (or constructor chaining).MAKE SURE to call the constructor in the member initializer list. 
         >> limitation
        1) a constructor that delegates to another constructor is not allowed to do any member initialization itself. So your constructors can delegate or initialize, but not both.
        2)it’s possible for one constructor to delegate to another constructor, which delegates back to the first constructor. This forms an infinite loop, and will cause your program to run out of stack space and crash. You can avoid this by ensuring all of your constructors resolve to a non-delegating constructor.
        */

        class Foo2
        {
        private:
        
        public:
            Foo2()
            {
                // code to do A
            }
        
            Foo2(int value): Foo2{} // use Foo() default constructor to do A
            {
                // code to do B
            }
        
        };
    //other eg :
    #include <string>
    #include <iostream>
 
        class Employee
        {
        private:
            int m_id{};
            std::string m_name{};
        
        public:
            Employee(int id=0, const std::string &name=""):
                m_id{ id }, m_name{ name }
            {
                std::cout << "Employee " << m_name << " created.\n";
            }
        
            // Use a delegating constructor to minimize redundant code
            Employee(const std::string &name) : Employee{ 0, name }
            { }
        };
    // do not call const fron a function to reset/init class
    // but to avoid duplicay, make seperate function & let constr call it in it`s body.
    //Constructors are allowed to call non-constructor functions in the class
    // make sure to check dynamic allocation in init func

//Destructor >> called automatically, no parameter, no return type, name starts with ~
    //>> only 1 per class is allowed ; i.e. cannot be overoaded
    // can call other functions, but cannot be called by other function or via obj

//pointer data members : delete copy constructor & assignment operator overloaded func to avoid error -Werror=effc++
        #include <cassert>
        #include <cstddef>
        
        class IntArray
        {
        private:
            int *m_array{};
            int m_length{};
        
        public:
            IntArray(int length) // constructor
            {
                assert(length > 0);
        
                m_array = new int[static_cast<std::size_t>(length)]{};
                m_length = length;
            }
        
            ~IntArray() // destructor
            {
                // Dynamically delete the array we allocated earlier
                delete[] m_array;
            }
        
            void setValue(int index, int value) { m_array[index] = value; }
            int getValue(int index) { return m_array[index]; }
        
            int getLength() { return m_length; }
        };
        
        int main()
        {
            IntArray ar(10); // allocate 10 integers
            for (int count{ 0 }; count < ar.getLength(); ++count)
                ar.setValue(count, count+1);
        
            std::cout << "The value of element 5 is: " << ar.getValue(5) << '\n';
        
            return 0;
        } // ar is destroyed here, so the ~IntArray() destructor function is called here

        /*If you compile the above example and get the following error:
            error: 'class IntArray' has pointer data members [-Werror=effc++]|
            error:   but does not override 'IntArray(const IntArray&)' [-Werror=effc++]|
            error:   or 'operator=(const IntArray&)' [-Werror=effc++]|
            Then you can either remove the “-Weffc++” flag from your compile settings for this example, or you can add the following two lines to the class:
                IntArray(const IntArray&) = delete;
                IntArray& operator=(const IntArray&) = delete;*/

/* Global variables are constructed before main() and destroyed after main(). */
//...RAII (Resource Acquisition Is Initialization
        // programming technique whereby resource use is tied to the lifetime of objects with automatic duration (e.g. non-dynamically allocated objects).
        /*  implements RAII -- allocation in the constructor, deallocation in the destructor. std::string and std::vector are examples of classes in the standard library that follow RAII -- dynamic memory is acquired on initialization, and cleaned up automatically on destruction. */

//.. NOte: if you use the exit() function, your program will terminate and no destructors will be called. 

