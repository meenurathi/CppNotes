
// inheritance >> is-a relationship, provides code reusability
/* 
C++ constructs derived classes in phases, starting with the most-base class (at the top of the inheritance tree) and finishing with the most-child class (at the bottom of the inheritance tree). As each class is constructed, the appropriate constructor from that class is called to initialize that part of the class. 

Here’s what actually happens when base is instantiated:
    Memory for base is set aside
    The appropriate Base constructor is called
    The initialization list initializes variables
    The body of the constructor executes
    Control is returned to the calle
Here’s what actually happens when derived is instantiated:
    Memory for derived is set aside (enough for both the Base and Derived portions)
    The appropriate Derived constructor is called
    The Base object is constructed first using the appropriate Base constructor. If no base constructor is specified, the default constructor will be used.
    The initialization list initializes variables
    The body of the constructor executes
    Control is returned to the caller

... only non-inherited variables can be initialized in the initialization list.    
// C++ prevents classes from initializing inherited member variables in the initialization list of a constructor. 
    In other words, the value of a member variable can only be set in an initialization list of a constructor 
    belonging to the same class as the variable. //

    Why does C++ do this? 
    The answer has to do with const and reference variables. Consider what would happen if m_id were const. Because const variables must be initialized with a value at the time of creation, the base class constructor must set its value when the variable is created. However, when the base class constructor finishes, the derived class constructors initialization lists are then executed. Each derived class would then have the opportunity to initialize that variable, potentially changing its value! By restricting the initialization of variables to the constructor of the class those variables belong to, C++ ensures that all variables are initialized only once.
    inherited variables can still have their values changed in the body of the constructor using an assignment

    C++ gives us the ability to explicitly choose which Base class constructor will be called! To do this, simply add a call to the base class Constructor in the initialization list of the derived class:

        class Derived: public Base
        {
        public:
            double m_cost;
        
            Derived(double cost=0.0, int id=0)
                : Base{ id }, // Call Base(int) constructor with value id!
                    m_cost{ cost }
            {
            }
        
            double getCost() const { return m_cost; }
        };

//Note : it doesn’t matter where in the Derived constructor initialization list the Base constructor is called 
            --- it will always execute first.
//Note :  constructors can only call constructors from their immediate parent/base class

// When a derived class is destroyed, each destructor is called in the reverse order of construction.
// access specifier : public, private(deafult), protected
//Inheritance type : public(preferred), private(default), protected
// Inherit from Base publicly
        class Pub: public Base
        {        };        
        // Inherit from Base protectedly
        class Pro: protected Base
        {       };
        // Inherit from Base privately
        class Pri: private Base
        { };
        class Def: Base // Defaults to private inheritance
        {        };

Note :  C++ defaults to private inheritance

// Rules:
    A class can always access its own (non-inherited) members.
    The public accesses the members of a class based on the access specifiers of the class it is accessing.
    A derived class accesses inherited members based on the access specifier inherited from the parent class. This varies depending on the access specifier and type of inheritance used.
    //example
    Base can access its own members without restriction. The public can only access m_public. Derived classes can access m_public and m_protected.
        class Base
        {
        public:
            int m_public;
        protected:
            int m_protected;
        private:
            int m_private;
        };
        D2 can access its own members without restriction. D2 can access Base’s m_public and m_protected members, but not m_private. Because D2 inherited Base privately, m_public and m_protected are now considered private when accessed through D2. This means the public can not access these variables when using a D2 object, nor can any classes derived from D2.

        class D2 : private Base // note: private inheritance
        {
            // Private inheritance means:
            // Public inherited members become private
            // Protected inherited members become private
            // Private inherited members stay inaccessible
        public:
            int m_public2;
        protected:
            int m_protected2;
        private:
            int m_private2;
        };

        D3 can access its own members without restriction. D3 can access D2’s m_public2 and m_protected2 members, but not m_private2. Because D3 inherited D2 publicly, m_public2 and m_protected2 keep their access specifiers when accessed through D3. D3 has no access to Base’s m_private, which was already private in Base. Nor does it have access to Base’s m_protected or m_public, both of which became private when D2 inherited them
        class D3 : public D2
        {
            // Public inheritance means:
            // Public inherited members stay public
            // Protected inherited members stay protected
            // Private inherited members stay inaccessible
        public:
            int m_public3;
        protected:
            int m_protected3;
        private:
            int m_private3;
        };
//summary
    First, a class (and friends) can always access its own non-inherited members. The access specifiers only affect whether outsiders and derived classes can access those members.

    Second, when derived classes inherit members, those members may change access specifiers in the derived class. This does not affect the derived classes’ own (non-inherited) members (which have their own access specifiers). It only affects whether outsiders and classes derived from the derived class can access those inherited members.

Here’s a table of all of the access specifier and inheritance types combinations:
        Access specifier  |	  Access specifier 	    |     Access specifier        |       Access specifier
        in base class    | when inherited publicly |   when inherited privately  |  when inherited protectedly
            Public	            Public	                    Private                     	Protected
            Protected       	Protected               	Private	                        Protected
            Private         	Inaccessible	            Inaccessible	                Inaccessible
        These access rules hold true for all members (e.g. member functions and types declared inside the class).

//Calling a base class function
        When a member function is called with a derived class object, the compiler first looks to see if that member exists in the derived class. If not, it begins walking up the inheritance chain and checking whether the member has been defined in any of the parent classes. It uses the first one it finds

//....>>Derived classes can add new functions, change the way functions that exist in the base class work in the derived class, change an inherited member’s access level, or hide functionality.
....Redefining behaviors
        However, if we had defined called function in the Derived class, it would have been used instead of base class
    Note:  when you redefine a function in the derived class, the derived function does not inherit the access specifier of the function with the same name in the base class. It uses whatever access specifier it is defined under in the derived class. Therefore, a function that is defined as private in the base class can be redefined as public in the derived class, or vice-versa!
            class Base {
            private:
                void print(){
                    std::cout << "Base";
                }
            };
            class Derived : public Base {
            public:
                void print(){ //  it hoides base  calss functin completely
                    std::cout << "Derived ";
                }
            };
            int main(){
                Derived derived;
                derived.print(); // calls derived::print(), which is public
                return 0;
            }

....... Adding to existing functionality 
            class Derived: public Base{
            public:
                Derived(int value)
                    : Base(value){}
                int GetValue() { return m_value; }
                void identify(){
                    Base::identify(); // call Base::identify() first
                    std::cout << "I am a Derived\n"; // then identify ourselves
                }
            };
//note: Calling function identify() without a scope resolution qualifier would default to the identify() in the current class, which would be Derived::identify(). This would cause Derived::identify() to call itself, which would lead to an infinite loop!
....
    There’s one bit of trickiness that we can run into when trying to call friend functions in base classes, such as operator<<. Because friend functions of the base class aren’t actually part of the base class, using the scope resolution qualifier won’t work. Instead, we need a way to make our Derived class temporarily look like the Base class so that the right version of the function can be called.
        class Base
        {
        private:
            int m_value;
        public:
            Base(int value)
                : m_value(value)
            {
            }
            friend std::ostream& operator<< (std::ostream &out, const Base &b)
            {
                out << "In Base\n";
                out << b.m_value << '\n';
                return out;
            }
        };
        class Derived : public Base
        {
        public:
            Derived(int value)
                : Base(value)
            {
            }
            friend std::ostream& operator<< (std::ostream &out, const Derived &d)
            {
                out << "In Derived\n";
                // static_cast Derived to a Base object, so we call the right version of operator<<
                out << static_cast<Base>(d); 
                return out;
            }
        };
        int main()
        {
            Derived derived(7);
            std::cout << derived;
            return 0;
        }
Note:  Because a Derived is-a Base, we can static_cast our Derived object into a Base, so that the appropriate version of operator<< that uses a Base is called. 

.......Hiding inherited functionality

>>>> Changing an inherited member’s access level
    C++ gives us the ability to change an inherited member’s access specifier in the derived class. This is done by using a using declaration to identify the (scoped) base class member that is having its access changed in the derived class, under the new access specifier.
    e.g.
        class Base
        {
        private:
            int m_value;
        
        public:
            Base(int value)
                : m_value(value)
            {
            }
        protected:
            void printValue() { std::cout << m_value; }
        };
        class Derived: public Base
        {
        public:
            Derived(int value)
                : Base(value)
            {            }
            // Base::printValue was inherited as protected, so the public has no access
            // But we're changing it to public via a using declaration
            using Base::printValue; // note: no parenthesis here
        };
    This means that this code will now work:
        int main()
        {
            Derived derived(7);
        
            // printValue is public in Derived, so this is okay
            derived.printValue(); // prints 7
            return 0;
        }
Note:
    you can only change the access specifiers of base members the derived class would normally be able to access. Therefore, you can never change the access specifier of a base member from private to protected or public, because derived classes do not have access to private members of the base class.

.....Hiding functionality
        In C++, it is not possible to remove or restrict functionality from a base class other than by modifying the source code. However, in a derived class, it is possible to hide functionality that exists in the base class, so that it can not be accessed through the derived class. This can be done simply by changing the relevant access specifier.
    For example, we can make a public member private:
        #include <iostream>
        class Base
        {
        public:
            int m_value;
        };
        
        class Derived : public Base
        {
        private:
            using Base::m_value;
        
        public:
            Derived(int value)
            // We can't initialize m_value, since it's a Base member (Base must initialize it)
            {
                // But we can assign it a value
                m_value = value;
            }
        };
        
        int main()
        {
            Derived derived(7);
        
            // The following won't work because m_value has been redefined as private
            std::cout << derived.m_value;
        
            return 0;
        }

 Note:  This allowed us to take a poorly designed base class and encapsulate its data in our derived class.      
        Alternatively, instead of inheriting Base’s members publicly and making m_value private by overriding its access specifier, we could have inherited Base privately, which would have caused all of Base’s member to be inherited privately in the first place.

You can also mark member functions as deleted in the derived class, which ensures they can’t be called at all through a derived object:
            class Base
            {
            private:
                int m_value;
            public:
                Base(int value)
                    : m_value(value)
                {                }
                int getValue() { return m_value; }
            };
            class Derived : public Base
            {
            public:
                Derived(int value)
                    : Base(value)
                {                }
                int getValue() = delete; // mark this function as inaccessible
            };
            int main()
            {
                Derived derived(7);
                // The following won't work because getValue() has been deleted!
                std::cout << derived.getValue();
                return 0;
            }

In the above example, we’ve marked the getValue() function as deleted. This means that the compiler will complain when we try to call the derived version of the function. 

Note : the Base version of getValue() is still accessible though. This means that a Derived object can still access getValue() by upcasting the Derived object to a Base first:
        int main()
        {
            Derived derived(7);
        
            // We can still access the function deleted in the Derived class through the Base class
            std::cout << static_cast<Base>(derived).getValue();
        
            return 0;
        }

...... // Multiple inheritance \\
To use multiple inheritance, simply specify each base class (just like in single inheritance), separated by a comma.
    // Teacher publicly inherits Person and Employee
        class Teacher: public Person, public Employee
.... Problems with multiple inheritance
    1)  ambiguity can result when multiple base classes contain a function with the same name.
        class USBDevice
        {
        private:
            long m_id;
        public:
            USBDevice(long id)
                : m_id(id)
            {
            }
            long getID() { return m_id; }
        };
        class NetworkDevice
        {
        private:
            long m_id;
        public:
            NetworkDevice(long id)
                : m_id(id)
            {
            }
            long getID() { return m_id; }
        };
        class WirelessAdapter: public USBDevice, public NetworkDevice
        {
        public:
            WirelessAdapter(long usbId, long networkId)
                : USBDevice(usbId), NetworkDevice(networkId)
            {
            }
        };
        
        int main()
        {
            WirelessAdapter c54G(5442, 181742);
            std::cout << c54G.getID(); // Which getID() do we call? WirelessAdapter have two versions, do compilation error
             std::cout << c54G.USBDevice::getID(); // allowed
        
            return 0;
        }
    2) more seroious Diamond problem : 
        This occurs when a class multiply inherits from two classes which each inherit from a single base class. This leads to a diamond shaped inheritance pattern.
           class PoweredDevice {};
           class Scanner: public PoweredDevice{};
           class Printer: public PoweredDevice{};
           class Copier: public Scanner, public Printer{};

        you have already been using classes written using multiple inheritance without knowing it: the iostream library objects std::cin and std::cout are both implemented using multiple inheritance!
    Rule: Avoid multiple inheritance unless alternatives lead to more complexity.
*/