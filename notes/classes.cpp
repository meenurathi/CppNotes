#include<iostream>
//... NAming convention
/* Name your classes starting with a capital letter.
 “m_” prefix for member variables */ 

//.... Implicit object 
/*the associated object is implicitly passed to the member function*/

//.... member function can call each other without forward declaration
        class foo
        {
        public:
            void x() { y(); } // okay to call y() here, even though y() isn't defined until later in this class
            void y() { };
        };

/* ...Rule: Use the struct keyword for data-only structures. 
Use the class keyword for objects that have both data and functions. */

/* ...public interface :
    The group of public members of a class are often referred to as a public interface. 
    Because only public members can be accessed from outside of the class,
    the public interface defines how programs using the class will interact with the class. */ 

/* Access controls work on a per-class basis
when a function has access to the private members of a class, it can access the private members of any object of that class type that it can see. */
        class DateClass // members are private by default
        {
            int m_month; // private by default, can only be accessed by other members
            int m_day; // private by default, can only be accessed by other members
            int m_year; // private by default, can only be accessed by other members
        public:
            void setDate(int month, int day, int year) {
                m_month = month;
                m_day = day;
                m_year = year;
            }        
            void print() {
                std::cout << m_month << '/' << m_day << '/' << m_year;
            }        
            // Note the addition of this function
            void copyFrom(const DateClass &d) {
                // Note that we can access the private members of d directly
                m_month = d.m_month;
                m_day = d.m_day;
                m_year = d.m_year;
            }
        };

/* A class defaults its members to private. A struct defaults its members to public.
structs inherit from other classes publicly and classes inherit privately. */

/* .... Encapsulation ---> hiding using access specifiers(private & protected)
     Encapsulation (also called information hiding) is the process of keeping the details about how an object is implemented hidden away from users of the object. Instead, users of the object access the object through a public interface. In this way, users are able to use the object without having to understand how it is implemented. 
     
     The word encapsulation is also sometimes used to refer to the packaging of data and functions that work on that data together. We prefer to just call that object-oriented programming.
     Benefits:
        > easier to use and reduce the complexity :The primary benefit is that encapsulation allows us to use a class without having to know how it was implemented
        > help  protect your data and prevent misuse
        > classes are easier to change
        > easier to debug
     */

/* ....... Access functions : public func used to access private members variabls
    > getter : retrieve private members >>>> should provide only read only access  
       Getters should return by value or const reference.
    >setter /mutators : change the private members
    note :: make access function only for members that are req to be used used outside */

/* //This pointer -- > hidden const pointer parameter 
        1) When the compiler compiles a normal member function, it implicitly adds a new parameter to the function named “this”. 
            simple.setID(2);
        Although the call to function setID() looks like it only has one argument, it actually has two! When compiled, the compiler converts simple.setID(2); into the following:
            setID(&simple, 2); // note that simple has been changed from an object prefix to a function argument!
        2)The this pointer is a hidden const pointer that holds the address of the object the member function was called on.
            void setID(int id) { m_id = id; }
             is converted by the compiler into:
             void setID(Simple* const this, int id) { this->m_id = id; } 
        3)Any member variables inside setID() are prefixed with “this->”.
           when “this” points to the address of simple, this->m_id will resolve to simple.m_id.*/

//...Chaining member functions
        //member function return the object it was working with as a return value
        //to allow a series of member functions to be “chained” together,
            class Calc
            {
            private:
                int m_value{};
            
            public:
                Calc& add(int value) { m_value += value; return *this; }
                Calc& sub(int value) { m_value -= value; return *this; }
                Calc& mult(int value) { m_value *= value; return *this; }
            
                int getValue() { return m_value; }
            };
            #include <iostream>
            int main11()
            {
                Calc calc{};
                calc.add(5).sub(3).mult(4);
            
                std::cout << calc.getValue() << '\n';
                return 0;
            }

//Doesn’t defining a class in a header file violate the one-definition rule?
        /* Types (which include classes), are exempt from the part of the one-definition rule that says you can only have one definition per program. Therefore, there isn’t an issue #including class definitions into multiple code files (if there was, classes wouldn’t be of much use). 
//Doesn’t defining member functions in the header violate the one-definition rule
         Member functions defined inside the class definition are considered implicitly inline.
         Member functions defined outside the class definition are treated like normal functions, and are subject to the one definition per program part of the one-definition rule.
RECOMMENDATION:
            For classes used in only one file that aren’t generally reusable, define them directly in the single .cpp file they’re used in.
            For classes used in multiple files, or intended for general reuse, define them in a .h file that has the same name as the class.
            Trivial member functions (trivial constructors or destructors, access functions, etc…) can be defined inside the class.
            Non-trivial member functions should be defined in a .cpp file that has the same name as the class
            Default parameters for member functions should be declared in the class definition (in the header file), where they can be seen by whomever #includes the header.
         */

//Const class objects and member functions
    /* const int value1 = 5; // copy initialization
        const int value2(7); // direct initialization
        const int value3 { 9 }; // uniform initialization (C++11) */
    //Const classes
    /*  const Date date1; // initialize using default constructor
        const Date date2(2020, 10, 16); // initialize using parameterized constructor
        const Date date3 { 2020, 10, 16 }; // initialize using parameterized constructor (C++11) 
        const object cannot call non const function which may try to change member vars
        const ref to object also, cannot modify members & call non const function*/

    //Const member functions --> const this pointer to const obj
        /*      //>> cannot modify any member vars
                //cannot call non const funct
                //>>can be called by both const & non const obj
        For member functions defined outside of the class definition, the const keyword must be used on both the function prototype in the class definition and on the function definition: */   
        class Something
        {
        public:
            int m_value;
        
            Something(): m_value{0} { }
        
            void resetValue() { m_value = 0; }
            void setValue(int value) { m_value = value; }
        
            int getValue() const; // note addition of const keyword here
        };        
        int Something::getValue() const // and here
        {
            return m_value;
        }
       /* note:  constructors cannot be marked as const. This is because constructors need to be able to initialize their member variables, and a const constructor would not be able to do so. Consequently, the language disallows const constructors. */
//rule: Make any member function that does not modify the state of the class object const, so that it can be called by const objects.
   /* 
    // ..Overloading const and non-const function
        This works because the const-ness of the function is considered part of the function’s signature, so a const and non-const function which differ only in const-ness are considered distinct.
    
     */


//......static member variables, and static member functions.
        /*Static member variables : keep their values and are not destroyed even after they go out of scope
            >they are created when the program starts, and destroyed when the program ends.
            > static member variables are shared by all objects of the class.
            > static members as belonging to the class itself, not to the objects of the class. 
            > is referenced by class name rather than through an object Something::s_value.
            > you must explicitly define the static member outside of the class, in the global scope.
                int Something::s_value{ 1 }; // defines the static member variable
                    This line serves two purposes: it instantiates the static member variable (just like a global variable), and optionally initializes it. In this case, we’re providing the initialization value 1. If no initializer is provided, C++ initializes the value to 0.
            > is not subject to access controls: you can define and initialize the value even if it’s declared as private (or protected) in the class.
            if the static member variables are public, we can access them directly using the class name and the scope resolution operator. But what if the static member variables are private? 
                we can’t access Something::s_value directly from main(), because it is private
                can be accessed by static member func
            > Do not put the static member definition in a header file -- may give redefination error
            */

        /* //Inline initialization of static member variables
                First, when the static member is a const INTEGRAL TYPES (which includes char and bool) or a const enum, the static member can be initialized inside the class definition: */
                class Whatever
                {
                public:
                    static const int s_value{ 4 }; // a static const int can be declared and initialized directly
                };
            /* In the above example, because the static member variable is a const int, no explicit definition line is needed.
                Second, static constexpr members can be initialized inside the class definition:*/
                #include <array>                
                class Whatever2
                {
                public:
                    static constexpr double s_value{ 2.2 }; // ok
                    static constexpr std::array<int, 3> s_array{ 1, 2, 3 }; // this even works for classes that support constexpr initialization
                };

// ... Static member functions
        /* static member functions are not attached to any particular object.
        Like static member variables, they can also be called through objects of the class type, though this is not recommended.
        First, because static member functions are not attached to an object, they have no this pointer! 
        Second, static member functions can directly access other static members (variables or functions), but not non-static members */
            class IDGenerator
            {
            private:
                static int s_nextID; // Here's the declaration for a static member
            
            public:
                static int getNextID(); // Here's the declaration for a static function
            };
            
            // Here's the definition of the static member outside the class.  Note we don't use the static keyword here.
            // We'll start generating IDs at 1
            int IDGenerator::s_nextID{ 1 };
            
            // Here's the definition of the static function outside of the class.  Note we don't use the static keyword here.
            int IDGenerator::getNextID() { return s_nextID++; } 
            
            int main10()
            {
                for (int count{ 0 }; count < 5; ++count)
                    std::cout << "The next ID is: " << IDGenerator::getNextID() << '\n';
            
                return 0;
            }

//note: C++ does not support static constructors

//initializing static member var
//One way that works with all variables, static or not, is to use a lambda and call it immediately.
        #include<vector>
        class MyClass
        {
        public:
            static std::vector<char> s_mychars;
        };        
        std::vector<char> MyClass::s_mychars{
        []{ // The parameter list of lambdas without parameters can be omitted.
            // Inside the lambda we can declare another vector and use a loop.
            std::vector<char> v{};
            
            for (char ch{ 'a' }; ch <= 'z'; ++ch)
            {
                v.push_back(ch);
            }
            
            return v;
        }() // Call the lambda right away
        };
/*         The following code presents a method that behaves more like a regular constructor. */
        class MyClass1
        {
        public:
            static std::vector<char> s_mychars;        
        public:        
            class init_static // we're defining a nested class named init_static
            {
            public:
                init_static() // the init constructor will initialize our static variable
                {
                    for (char ch{ 'a' }; ch <= 'z'; ++ch)
                    {
                        s_mychars.push_back(ch);
                    }
                }
            } ;        
        private:
            static init_static s_initializer; // we'll use this static object to ensure the init_static constructor is called
        };
        
        /* std::vector<char> MyClass::s_mychars{}; // define our static member variable
        MyClass::init_static MyClass::s_initializer{}; // define our static initializer, which will call the init_static constructor, which will initialize s_mychars
        When static member s_initializer is defined, the init_static() default constructor will be called (because s_initializer is of type init_static). We can use this constructor to initialize any static member variables. The nice thing about this solution is that all of the initialization code is kept hidden inside the original class with the static member. */

//....Friend function --> access of priv member to any specific class or func
  /*    >To declare a friend function, simply use the friend keyword in front of the prototype of the
                function you wish to be a friend of the class. 
        >It does not matter whether you declare the friend function in the private or public section of the class
        >  It does not have a *this pointer,. */
            class Accumulator
            {
            private:
                int m_value;
            public:
                Accumulator() { m_value = 0; } 
                void add(int value) { m_value += value; }
            
                // Make the reset() function a friend of this class
                friend void reset(Accumulator &accumulator);
            };
            
            // reset() is now a friend of the Accumulator class
            void reset(Accumulator &accumulator)
            {
                // And can access the private data of Accumulator objects
                accumulator.m_value = 0;
            }
            
            int main112()
            {
                Accumulator acc;
                acc.add(5); // add 5 to the accumulator
                reset(acc); // reset the accumulator to 0
            
                return 0;
            }
            class Value
            {
            private:
                int m_value;
            public:
                Value(int value) { m_value = value; }
                friend bool isEqual(const Value &value1, const Value &value2);
            };
            
            bool isEqual(const Value &value1, const Value &value2)
            {
                return (value1.m_value == value2.m_value);
            }
//.. Friend classes
    /*    First, even though class A is a friend of class B, Display has no direct access to the
             *this pointer of B objects.All member func of class A can access members of class B via B objt passed to them as parameter
         Second, just because A is a friend of B, that does not mean B is also a friend of Display. 
            If you want two classes to be friends of each other, both must declare the other as a friend. 
         Finally, if class A is a friend of B, and B is a friend of C, that does not mean A is a friend of C */
            #include <iostream>
 
            class Storage1
            {
            private:
                int m_nValue;
                double m_dValue;
            public:
                Storage1(int nValue, double dValue)
                {
                    m_nValue = nValue;
                    m_dValue = dValue;
                }
            
                // Make the Display class a friend of Storage
                friend class Display1;
            };
            
            class Display1
            {
            private:
                bool m_displayIntFirst;
            
            public:
                Display1(bool displayIntFirst) { m_displayIntFirst = displayIntFirst; }
            
                void displayItem(const Storage1 &storage)
                {
                    if (m_displayIntFirst)
                        std::cout << storage.m_nValue << ' ' << storage.m_dValue << '\n';
                    else // display double first
                        std::cout << storage.m_dValue << ' ' << storage.m_nValue << '\n';
                }
            };
            
            int main12()
            {
                Storage1 storage(5, 6.7);
                Display1 display(false);
            
                display.displayItem(storage);
            
                return 0;
            }

// Friend member function:
        /* instead of making an entire class a friend, you can make a single member function a friend
            using the name of the member function with the className:: prefix included (e.g. Display::displayItem). 
                >the compiler has to have seen the full definition for the class of the friend member function (not just a forward declaration).
                Note that making a specific member function a friend requires the full definition for the class of the member function to have been seen first. */
                //Display::displayItem needs to be friend of storage
                #include <iostream> 
                class Storage; // forward declaration for class Storage
                
                class Display
                {
                private:
                    bool m_displayIntFirst;
                
                public:
                    Display(bool displayIntFirst) { m_displayIntFirst = displayIntFirst; }
                    
                    void displayItem(const Storage &storage); // forward declaration above needed for this declaration line
                };
                
                class Storage // full definition of Storage class
                {
                private:
                    int m_nValue;
                    double m_dValue;
                public:
                    Storage(int nValue, double dValue)
                    {
                        m_nValue = nValue;
                        m_dValue = dValue;
                    }
                
                    // Make the Display::displayItem member function a friend of the Storage class (requires seeing the full declaration of class Display, as above)
                    friend void Display::displayItem(const Storage& storage);
                };
                
                // Now we can define Display::displayItem, which needs to have seen the full definition of class Storage
                void Display::displayItem(const Storage &storage)
                {
                    if (m_displayIntFirst)
                        std::cout << storage.m_nValue << ' ' << storage.m_dValue << '\n';
                    else // display double first
                        std::cout << storage.m_dValue << ' ' << storage.m_nValue << '\n';
                }
                
                int main13()
                {
                    Storage storage(5, 6.7);
                    Display display(false);
                
                    display.displayItem(storage);
                
                    return 0;
                }
    
/* Anonymous class objects

Although our prior examples have been with built-in data types, it is possible to construct anonymous objects of our own class types as well. This is done by creating objects like normal, but omitting the variable name.
Cents cents{ 5 }; // normal variable
Cents{ 7 }; // anonymous object 
        note:  anonymous objects are treated as rvalues (not lvalues, which have an address)*/
        #include <iostream>
        class Cents
        {
        private:
            int m_cents{};
        
        public:
            Cents(int cents) { m_cents = cents; }
        
            int getCents() const { return m_cents; }
        };
        
        void print(const Cents &cents)
        {
        std::cout << cents.getCents() << " cents\n";
        }
        
        int main()
        {
            print(Cents{ 6 }); // Note: Now we're passing an anonymous Cents value
            return 0;
        }
/* ...Nested members
    // C++ will let you define other types within a class, such as typedefs, type aliases, and even other classes!
    Like any normal member of a class, nested classes have the same access to members of the enclosing class that the enclosing class does.
    limitation>>
        1) the nested class does not have any special access to the “this” pointer of the enclosing class.
        2) they can’t be forward declared. */