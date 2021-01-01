/* 
Virtual functions and polymorphism

        A virtual function is a special type of function that, when called, resolves to the most-derived version of the function that exists between the base and derived class. 
        This capability is known as polymorphism.
        A derived function is considered a match if it has the same signature (name, parameter types, and whether it is const) and return type as the base version of the function. Such functions are called overrides. 
note: 
        the signature of the derived class function must exactly match the signature of the base class virtual function in order for the derived class function to be used. 
        If the derived class function has different parameter types, the program will likely still compile fine, but the virtual function will not resolve as intended.

*/
//e.g.1
#if 0
    #include <iostream>
    #include <string_view>
        
        class A
        {
        public:
            virtual std::string_view getName() const { return "A"; }
        };
        
        class B: public A
        {
        public:
            virtual std::string_view getName() const { return "B"; }
        };
        
        class C: public B
        {
        public:
            virtual std::string_view getName() const { return "C"; }
        };
        
        class D: public C
        {
        public:
            virtual std::string_view getName() const { return "D"; }
        };
        
        int main1()
        {
            C c;
            A &rBase{ c };
            std::cout << "rBase is a " << rBase.getName() << '\n';
        
            return 0;
        }
#endif
//e.g.2
#if 0
        #include <iostream>
        #include <string_view>
        
        class A
        {
        public:
            virtual std::string_view getName() const { return "A"; }
        };
        
        class B: public A
        {
        public:
            virtual std::string_view getName() const { return "B"; }
        };
        
        class C: public B
        {
        public:
        };
        
        class D: public C
        {
        public:
            virtual std::string_view getName() const { return "D"; }
        };
        
        int main()
        {
            C c;
            A &rBase{ c };
            std::cout << rBase.getName() << '\n';
        
            return 0;
        }
#endif
        /* ans: 
          B. rBase is an A reference pointing to a C object. Normally rBase.getName() would call A::getName(), but A::getName() is virtual so it instead calls the most derived matching function between A and C. 
          That is B::getName(), which prints B.
         */
/* Use of the virtual keyword

        If a function is marked as virtual, all matching overrides are also considered virtual, even if they are not explicitly marked as such. 
        However, having the keyword virtual on the derived functions does not hurt, and it serves as a useful reminder that the function is a virtual function rather than a normal one.
        
 */
#if 0
        #include <iostream>
        #include <string_view>
        
        class A
        {
        public:
            virtual std::string_view getName() const { return "A"; }
        };
        
        class B: public A
        {
        public:
            std::string_view getName() const { return "B"; }
        };
        
        class C: public B
        {
        public:
            std::string_view getName() const { return "C"; }
        };
        
        class D: public C
        {
        public:
            std::string_view getName() const { return "D"; } 
        };
        
        int main()
        {
            C c;
            B &rBase{ c }; // note: rBase is a B this time
            std::cout << rBase.getName() << '\n';
        
            return 0;
        }
#endif
        /* 
            answer : C.
             Even though B and C aren’t marked as virtual functions, A::getName() is virtual and B::getName() and C::getName() are overrides. 
             Therefore, B::getName() and C::getName() are considered implicitly virtual, and thus the call to rBase.getName() resolves to C::getName(), not B::getName().
        */
#if 0
        #include <iostream>
        #include <string_view>
        
        class A
        {
        public:
            virtual std::string_view getName() const { return "A"; }
        };
        
        class B: public A
        {
        public:
            virtual std::string_view getName() { return "B"; }
        };
        
        class C: public B
        {
        public:
            virtual std::string_view getName() { return "C"; }
        };
        
        class D: public C
        {
        public:
            virtual std::string_view getName() { return "D"; }
        };
        
        int main()
        {
            C c;
            A &rBase{ c };
            std::cout << rBase.getName() << '\n';
        
            return 0;
        }

#endif
    /* Answer: A
        rBase is an A reference to a C object, so rBase.getName() would normally call A::getName(). 
        But A::getName() is virtual, so it calls the most derived version of the function between A and C. 
        And that is A::getName(). Because B::getName() and c::getName() are not const, they are not considered overrides! Consequently, this program prints A.

     */
/*
Return types of virtual functions

    Under normal circumstances, the return type of a virtual function and its override must match. 
    Consider the following example:
    */
        /*
        class Base
        {
        public:
            virtual int getValue() const { return 5; }
        };
        
        class Derived: public Base
        {
        public:
            virtual double getValue() const { return 6.78; }
        };
        */
/*    In this case, Derived::getValue() is not considered a matching override for Base::getValue() (it is considered a completely separate function)
so we get compilation error.
        error: conflicting return type specified for 'virtual double Derived::getValue() const'
*/

/* 
Do not call virtual functions from constructors or destructors

    Remember that when a Derived class is created, the Base portion is constructed first. 
    If you were to call a virtual function from the Base constructor, and Derived portion of the class hadn’t even been created yet, it would be unable to call the Derived version of the function because there’s no Derived object for the Derived function to work on. 
    In C++, it will call the Base version instead.

    A similar issue exists for destructors. 
    If you call a virtual function in a Base class destructor, it will always resolve to the Base class version of the function, because the Derived portion of the class will already have been destroyed.

@@Rule: Never call virtual functions from constructors or destructors

 */
#if 0
        #include <iostream>
        #include <string_view>
        
        class A
        {
        public:
            A() { std::cout << getName(); } // note addition of constructor
        
            virtual std::string_view getName() const { return "A"; }
        };
        
        class B : public A
        {
        public:
            virtual std::string_view getName() const { return "B"; }
        };
        
        class C : public B
        {
        public:
            virtual std::string_view getName() const { return "C"; }
        };
        
        class D : public C
        {
        public:
            virtual std::string_view getName() const { return "D"; }
        };
        
        int main()
        {
            C c;
        
            return 0;
        }

#endif
    /* Answer:
        A.
        When we create a C object, the A part is constructed first. When the A constructor is called to do this, it calls virtual function getName(). 
        Because the B and C parts of the class aren’t set up yet, this resolves to A::getName().    
     */
/* 
The downside of virtual functions

    Since most of the time you’ll want your functions to be virtual, why not just make all functions virtual? 
    The answer is because it’s inefficient -- resolving a virtual function call takes longer than resolving a regular one. 
    Furthermore, the compiler also has to allocate an extra pointer for each class object that has one or more virtual functions
 */

//>>>>>>>>>>>>>> The override and final specifiers, and covariant return types
/* The override specifier

    a derived class virtual function is only considered an override if its signature and return types match exactly. 
    That can lead to inadvertent issues, where a function that was intended to be an override actually isn’t. 
*/
#if 0
        class A
        {
        public:
            virtual const char* getName1(int x) { return "A"; }
            virtual const char* getName2(int x) { return "A"; }
        };
        
        class B : public A
        {
        public:
            virtual const char* getName1(short int x) { return "B"; } // note: parameter is a short int
            virtual const char* getName2(int x) const { return "B"; } // note: function is const
        };
        
        int main()
        {
            B b{};
            A& rBase{ b };
            std::cout << rBase.getName1(1) << '\n';
            std::cout << rBase.getName2(2) << '\n';
        
            return 0;
        }
/*  Answer: A A
    Because rBase is an A reference to a B object, the intention here is to use virtual functions to access B::getName1() and B::getName2(). However, because B::getName1() takes a different parameter (a short int instead of an int), it’s not considered an override of A::getName1(). More insidiously, because B::getName2() is const and A::getName2() isn’t, B::getName2() isn’t considered an override of A::getName2().

  */
#endif

/* 
    The override specifier can be applied to any override function by placing the specifier in the same place const would go. If the function does not override a base class function (or is applied to a non-virtual function), the compiler will flag the function as an error.
 */
#if 0
    class A
    {
    public:
        virtual const char* getName1(int x) { return "A"; }
        virtual const char* getName2(int x) { return "A"; }
        virtual const char* getName3(int x) { return "A"; }
    };
    
    class B : public A
    {
    public:
        virtual const char* getName1(short int x) override { return "B"; } // compile error, function is not an override
        virtual const char* getName2(int x) const override { return "B"; } // compile error, function is not an override
        virtual const char* getName3(int x) override { return "B"; } // okay, function is an override of A::getName3(int)
    
    };
    
    int main()
    {
        return 0;
    }
#endif

//@@Rue: Apply the override specifier to every intended override function you write.

/* 
    The final specifier

        Enforce compiler to give an error, if any derive class
            >> override a virtual function,
            >> or inherit from a class. 
 */
#if 0
/* final override function */
        class A
        {
        public:
            virtual const char* getName() { return "A"; }
        };
        
        class B : public A
        {
        public:
            // note use of final specifier on following line -- that makes this function no longer overridable
            virtual const char* getName() override final { return "B"; } // okay, overrides A::getName()
        };
        
        class C : public B
        {
        public:
            virtual const char* getName() override { return "C"; } // compile error: overrides B::getName(), which is final
        };
/* final class  */
        class A
        {
        public:
            virtual const char* getName() { return "A"; }
        };
        
        class B final : public A // note use of final specifier here
        {
        public:
            virtual const char* getName() override { return "B"; }
        };
        
        class C : public B // compile error: cannot inherit from final class
        {
        public:
            virtual const char* getName() override { return "C"; }
        };
#endif

/*  Covariant return types

        derived class virtual function override can have a different return type than the base class and still be considered a matching override. If the return type of a virtual function is a pointer or a reference to a class, override functions can return a pointer or a reference to a derived class. These are called covariant return types. 

 */
#if 0
#include <iostream>
 
        class Base
        {
        public:
            // This version of getThis() returns a pointer to a Base class
            virtual Base* getThis() { std::cout << "called Base::getThis()\n"; return this; }
            void printType() { std::cout << "returned a Base\n"; }
        };
        
        class Derived : public Base
        {
        public:
            // Normally override functions have to return objects of the same type as the base function
            // However, because Derived is derived from Base, it's okay to return Derived* instead of Base*
            Derived* getThis() override { std::cout << "called Derived::getThis()\n";  return this; }
            void printType() { std::cout << "returned a Derived\n"; }
        };
        
        int main()
        {
            Derived d{};
            Base* b{ &d };
            d.getThis()->printType(); // calls Derived::getThis(), returns a Derived*, calls Derived::printType
            b->getThis()->printType(); // calls Derived::getThis(), returns a Base*, calls Base::printType
        
            return 0;
        }
/* Answer
        called Derived::getThis()
        returned a Derived
        called Derived::getThis()
        returned a Base

        One interesting note about covariant return types:
         C++ can’t dynamically select types, so you’ll always get the type that matches the base version of the function being called.

        In the above example, we first call d.getThis(). Since d is a Derived, this calls Derived::getThis(), which returns a Derived*. This Derived* is then used to call non-virtual function Derived::printType().
        Now the interesting case. We then call b->getThis(). Variable b is a Base pointer to a Derived object. Base::getThis() is virtual function, so this calls Derived::getThis(). Although Derived::getThis() returns a Derived*, because base version of the function returns a Base*, the returned Derived* is upcast to a Base*. And thus, Base::printType() is called.

        In other words, in the above example, you only get a Derived* if you call getThis() with an object that is typed as a Derived object in the first place
 */
#endif
///>>>>Virtual destructors
/* Rule: Whenever you are dealing with inheritance, you should make any explicit destructors virtual.

        As with normal virtual member functions, if a base class function is virtual, all derived overrides will be considered virtual regardless of whether they are specified as such. It is not necessary to create an empty derived class destructor just to mark it as virtual. 
*/
#if 0
        #include <iostream>
        class Base
        {
        public:
            virtual ~Base() // note: virtual
            {
                std::cout << "Calling ~Base()\n";
            }
        };
        
        class Derived: public Base
        {
        private:
            int* m_array;
        
        public:
            Derived(int length)
            : m_array{ new int[length] }
            {
            }
        
            virtual ~Derived() // note: virtual //
            {
                std::cout << "Calling ~Derived()\n";
                delete[] m_array;
            }
        };
        
        int main()
        {
            Derived *derived { new Derived(5) };
            Base *base { derived };
        
            delete base;
        
            return 0;
        }
/* 
Calling ~Derived()
Calling ~Base()
 */
#endif

/* 
    Virtual assignment

        It is possible to make the assignment operator virtual. However, unlike the destructor case where virtualization is always a good idea, virtualizing the assignment operator really opens up a bag full of worms
    
    Ignoring virtualization

        Very rarely you may want to ignore the virtualization of a function
                class Base
                {
                public:
                    virtual const char* getName() const { return "Base"; }
                };
                
                class Derived: public Base
                {
                public:
                    virtual const char* getName() const { return "Derived"; }
                };
        here may be cases where you want a Base pointer to a Derived object to call Base::getName() instead of Derived::getName().
             #include <iostream>
            int main()
            {
                Derived derived;
                const Base &base { derived };
                // Calls Base::GetName() instead of the virtualized Derived::GetName()
                std::cout << base.Base::getName() << '\n';
            
                return 0;
            }

      Should we make all destructors virtual?

        As noted in the top example, if the base class destructor isn’t marked as virtual, then the program is at risk for leaking memory if a programmer later deletes a base class pointer that is pointing to a derived object. One way to avoid this is to mark all your destructors as virtual. But should you?

        It’s easy to say yes, so that way you can later use any class as a base class -- but there’s a performance penalty for doing so (a virtual pointer added to every instance of your class). So you have to balance that cost, as well as your intent.

        Conventional wisdom (as initially put forth by Herb Sutter, a highly regarded C++ guru) has suggested avoiding the non-virtual destructor memory leak situation as follows, 
            “A base class destructor should be either public and virtual, or protected and nonvirtual.” 
        A class with a protected destructor can’t be deleted via a pointer, thus preventing the accidental deleting of a derived class through a base pointer when the base class has a non-virtual destructor. 

            Unfortunately, this also means the base class can’t be deleted through a base class pointer, which essentially means the class can’t be dynamically allocated or deleted except by a derived class. 
            This also precludes using smart pointers (such as std::unique_ptr and std::shared_ptr) for such classes, which limits the usefulness of that rule.
            It also means the base class can’t be allocated on the stack.
             That’s a pretty heavy set of penalties.

         our recommendations are as follows:

        If you intend your class to be inherited from, make sure your destructor is virtual.
        If you do not intend your class to be inherited from, mark your class as final. This will prevent other classes from inheriting from it in the first place, without imposing any other use restrictions on the class itself.
 */

/*>>>>>>.Binding refers to the process that is used to convert identifiers (such as variable and function names) into addresses.
    Early binding 
        (also called static binding) means the compiler (or linker) is able to directly associate the identifier name (such as a function or variable name) with a machine address.
    Late Binding
        In some programs, it is not possible to know which function will be called until runtime (when the program is run). This is known as late binding (or dynamic binding).        
        One way is
         to use function pointers


    Late binding is slightly less efficient since it involves an extra level of indirection. 
    With early binding, the CPU can jump directly to the function’s address. 
    With late binding, the program has to read the address held in the pointer and then jump to that address. This involves one extra step, making it slightly slower. 
    However, the advantage of late binding is that it is more flexible than early binding, because decisions about what function to call do not need to be made until run time.
*/

////////////////////// 
/* The virtual table
    To implement virtual functions, C++ uses a special form of late binding known as the virtual table. The virtual table is a lookup table of functions used to resolve function calls in a dynamic/late binding manner. The virtual table sometimes goes by other names, such as “vtable”, “virtual function table”, “virtual method table”, or “dispatch table”
        FIrst : Vtable:
            every class that uses virtual functions (or is derived from a class that uses virtual functions) is given its own virtual table. 
            This table is simply a static array that the compiler sets up at compile time. A virtual table contains one entry for each virtual function that can be called by objects of the class.
            Each entry in this table is simply a function pointer that points to the most-derived function accessible by that class.
        Second: Vptr :
            the compiler also adds a hidden pointer to the base class, which we will call *__vptr. *__vptr is set (automatically) when a class instance is created so that it points to the virtual table for that class. 
            Consequently, it makes each class object allocated bigger by the size of one pointer. 
            It also means that *__vptr is inherited by derived classes, which is important.


 */
#if 0
            class Base
            {
            public:
                FunctionPointer *__vptr;// added by compiler in most base class with vf
                virtual void function1() {};
                virtual void function2() {};
            };
            
            class D1: public Base
            {
            public:
                virtual void function1() {};
            };
            
            class D2: public Base
            {
            public:
                virtual void function2() {};
            };
/* 
            1) Because there are 3 classes here, the compiler will set up 3 virtual tables: one for Base, one for D1, and one for D2
                Because there are only two virtual functions here, each virtual table will have two entries (one for function1() and one for function2()).
                each entry is filled out with the most-derived function an object of that class type can call.
            2) The compiler also adds a hidden pointer to the most base class that uses virtual functions.It points to the virtual table for that class
                when an object of type Base is created, *__vptr is set to point to the virtual table for Base. 
                When objects of type D1 or D2 are constructed, *__vptr is set to point to the virtual table for D1 or D2 respectively.
            The entries in the virtual table point to the most-derived version of the function objects of that class are allowed to call.
            Base *_vptr       --> base vtable : 
                                    base::function1() 
                                    base::function2()
            Derived D1 *_vptr --> derived D1 vtable : 
                                    D1::function1() 
                                    Base::function2()
            Derived D2 *_vptr --> derived D2 vtable :
                                    base::function1() 
                                    D2::function2()
            

            int main()
            {
                D1 d1;
                Base *dPtr = &d1; // vptr points to d1 vtable
                    // dPtr is a base pointer, it only points to the Base portion of d1
                    //dPtr->__vptr points to the D1 virtual table! Consequently, even though dPtr is of type Base, it still has access to D1’s virtual table (through __vptr).

                dPtr->function1();
                    1)program recognizes that function1() is a virtual function.
                    2)the program uses dPtr->__vptr to get to D1’s virtual table.
                    3)it looks up which version of function1() to call in D1’s virtual table. This has been set to D1::function1(). 
                    4)dPtr->function1() resolves to D1::function1()!

                Base b;;
                Base *bPtr = &b;
                bPtr->function1();
                //base::function1() is called
                return 0;
            }
                
 */
#endif
/* 
    Calling a virtual function is slower than calling a non-virtual function for a couple of reasons: 
        First, we have to use the *__vptr to get to the appropriate virtual table. 
        Second, we have to index the virtual table to find the correct function to call. 
        Only then can we call the function. 
    As a result, we have to do 3 operations to find the function to call, as opposed to 2 operations for a normal indirect function call, or one operation for a direct function call.
    However, with modern computers, this added time is usually fairly insignificant.
     */
//>>>>>>>Pure virtual functions, abstract base classes, and interface classes

/* 
     pure virtual function (or abstract function) : are virtual functions that has no body at all!
     It simply acts as a placeholder that is meant to be redefined by derived classes.

        class Base
        {
        public:
            const char* sayHi() const { return "Hi"; } // a normal non-virtual function    
        
            virtual const char* getName() const { return "Base"; } // a normal virtual function
        
            virtual int getValue() const = 0; // a pure virtual function
        
            int doSomething() = 0; // Compile error: can not set non-virtual functions to 0
        };
        Using a pure virtual function has two main consequences:        
            First, any class with one or more pure virtual functions becomes an abstract base class, which means that it can not be instantiated!
                int main()
                {
                    Base base; // We can't instantiate an abstract base class, but for the sake of example, pretend this was allowed
                    base.getValue(); // what would this do?
                
                    return 0;
                }
            Second, any derived class must define a body for this function, or that derived class will be considered an abstract base class as well.

        A pure virtual function is useful
             when we have a function that we want to put in the base class, but only the derived classes know what it should return. 
             A pure virtual function makes it so the base class can not be instantiated, and the derived classes are forced to define these functions before they can be instantiated.
        
        Pure virtual functions with bodies :
            we can define pure virtual functions that have bodies.
            When providing a body for a pure virtual function, the body must be provided separately (not inline).
            Derived calss still need to define pure virtual function
            & base class is still abstract base class
                #include <string>            
                class Animal // This Animal is an abstract base class
                {
                protected:
                    std::string m_name;
                
                public:
                    Animal(const std::string& name)
                        : m_name{ name }
                    {                }
                    std::string getName() { return m_name; }
                    virtual const char* speak() const = 0; // The = 0 means this function is pure virtual
                    virtual ~Animal() = default;
                };
                const char* Animal::speak() const  // even though it has a body
                {
                    return "buzz";
                }
            This paradigm can be useful when you want your base class to provide a default implementation for a function, but still force any derived classes to provide their own implementation. 
            However, if the derived class is happy with the default implementation provided by the base class, it can simply call the base class implementation directly.
                class Dragonfly: public Animal
                {
                public:
                    Dragonfly(const std::string& name)
                        : Animal{name}
                    {                    }
                    const char* speak() const override// this class is no longer abstract because we defined this function
                    {
                        return Animal::speak(); // use Animal's default implementation
                    }
                };
                int main()
                {
                    Dragonfly dfly{"Sally"};
                    std::cout << dfly.getName() << " says " << dfly.speak() << '\n';
                    return 0;
                }
 */
/* 
    Interface classes

        An interface class is a class that has no member variables, and where all of the functions are pure virtual! In other words, the class is purely a definition, and has no actual implementation. 
        Interfaces are useful when you want to define the functionality that derived classes must implement, but leave the details of how the derived class implements that functionality entirely up to the derived class.
            class IErrorLog
            {
            public:
                virtual bool openLog(const char *filename) = 0;
                virtual bool closeLog() = 0;
            
                virtual bool writeError(const char *errorMessage) = 0;
            
                virtual ~IErrorLog() {} // make a virtual destructor in case we delete an IErrorLog pointer, so the proper derived destructor is called
            };
        Note: Don’t forget to include a virtual destructor for your interface classes, so that the proper derived destructor will be called if a pointer to the interface is deleted.
        Interface classes have become extremely popular because they are 
            easy to use, easy to extend, and easy to maintain.

Pure virtual functions and the virtual table
    Abstract classes still have virtual tables, as these can still be used if you have a pointer or reference to the abstract class. T
    he virtual table entry for a pure virtual function will generally 
        either contain a null pointer,
        or point to a generic function that prints an error (sometimes this function is named __purecall) if no override is provided.
 */

/* 
    Virtual base classes: The diamond problem
        To share a base class, simply insert the “virtual” keyword in the inheritance list of the derived class.
        This creates what is called a virtual base class, which means there is only one base object. The base object is shared between all objects in the inheritance tree and it is only constructed once.
        
 */
#if 0
        #include<iostream>
        //diamond problem
            class PoweredDevice
            {
            public:
                PoweredDevice(int power)
                {
                    std::cout << "PoweredDevice: " << power << '\n';
                }
            };
            class Scanner: public PoweredDevice
            {
            public:
                Scanner(int scanner, int power)
                    : PoweredDevice{ power }
                {
                    std::cout << "Scanner: " << scanner << '\n';
                }
            };
            class Printer: public PoweredDevice
            {
            public:
                Printer(int printer, int power)
                    : PoweredDevice{ power }
                {
                    std::cout << "Printer: " << printer << '\n';
                }
            };
            class Copier: public Scanner, public Printer
            {
           public:
                Copier(int scanner, int printer, int power)
                    : Scanner{ scanner, power }, Printer{ printer, power }
                {
                }
            };
            
            int main()
            {
                Copier copier{ 1, 2, 3 };
            
                return 0;
            }
            /* Answer :PoweredDevice got constructed twice.
                    PoweredDevice: 3 
                    Scanner: 1
                    PoweredDevice: 3
                    Printer: 2
             */
#endif
//better inherit shared base class as virtual base class
        //base is not shared between objects but classes
        // 1 base per objt
#if 0
            class PoweredDevice
            {
            };
            
            class Scanner: virtual public PoweredDevice
            {
            };
            
            class Printer: virtual public PoweredDevice
            {
            };
            
            class Copier: public Scanner, public Printer
            {
            };
/* 
        when you create a Copier class object, you will get only one copy of PoweredDevice per Copier that will be shared by both Scanner and Printer.
 */ 

#endif
/*      However, this leads to one more problem: 
        if Scanner and Printer share a PoweredDevice base class, who is responsible for creating it? 
        The answer, as it turns out, is Copier.
         The Copier constructor is responsible for creating PoweredDevice. 
         Consequently, this is one time when Copier is allowed to call a non-immediate-parent constructor directly 

         */
#if 0
             #include <iostream>
            
            class PoweredDevice
            {
            public:
                PoweredDevice(int power)
                {
                    std::cout << "PoweredDevice: " << power << '\n';
                }
            };
            
            class Scanner: virtual public PoweredDevice // note: PoweredDevice is now a virtual base class
            {
            public:
                Scanner(int scanner, int power)
                    : PoweredDevice{ power } // this line is required to create Scanner objects, but ignored in this case
                {
                    std::cout << "Scanner: " << scanner << '\n';
                }
            };
            
            class Printer: virtual public PoweredDevice // note: PoweredDevice is now a virtual base class
            {
            public:
                Printer(int printer, int power)
                    : PoweredDevice{ power } // this line is required to create Printer objects, but ignored in this case
                {
                    std::cout << "Printer: " << printer << '\n';
                }
            };
            
            class Copier: public Scanner, public Printer
            {
            public:
                Copier(int scanner, int printer, int power)
                    :// PoweredDevice{ power }, // PoweredDevice is constructed here
                    Scanner{ scanner, power }, Printer{ printer, power }
                {
                }
            };
            int main()
            {
                Copier copier{ 1, 2, 3 };
            
                return 0;
            }
/*  
        Answer: PoweredDevice only gets constructed once.
            PoweredDevice: 3
            Scanner: 1
            Printer: 2
 */
#endif
/*   Impt notes for virtual base classes:
        First, virtual base classes are always created before non-virtual base classes, which ensures all bases get created before their derived classes.

        Second, note that the Scanner and Printer constructors still have calls to the PoweredDevice constructor.
        When creating an instance of Copier, these constructor calls are simply ignored because Copier is responsible for creating the PoweredDevice, not Scanner or Printer. 
        However, if we were to create an instance of Scanner or Printer, those constructor calls would be used, and normal inheritance rules apply.

        Third, if a class inherits one or more classes that have virtual parents, the most derived class is responsible for constructing the virtual base class.
        In this case, Copier inherits Printer and Scanner, both of which have a PoweredDevice virtual base class. Copier, the most derived class, is responsible for creation of PoweredDevice. 
        Note that this is true even in a single inheritance case:
         if Copier was singly inherited from Printer, and Printer was virtually inherited from PoweredDevice, Copier is still responsible for creating PoweredDevice.

         Fourth, all classes inheriting a virtual base class will have a virtual table, even if they would normally not have one otherwise, and thus be larger by a pointer.
            Because Scanner and Printer derive virtually from PoweredDevice, Copier will only be one PoweredDevice subobject.
            Scanner and Printer both need to know how to find that single PoweredDevice subobject, so they can access its members (because after all, they are derived from it). 
            This is typically done through some virtual table magic (which essentially stores the offset from each subclass to the PoweredDevice subobject).
 */

/* 
    18.9 — Object slicing
        The assigning of a Derived class object to a Base class object is called object slicing (or slicing for short).

        int main()
        {
            Derived derived{ 5 };
            Base base{ derived }; // what happens here?
            std::cout << "base is a " << base.getName() << " and has value " << base.getValue() << '\n';
        
            return 0;
        }
        //Answer: base is a Base and has value 5
        Remember that derived has a Base part and a Derived part. 
        When we assign a Derived object to a Base object, only the Base portion of the Derived object is copied. The Derived portion is not. 
        In the example above, base receives a copy of the Base portion of derived, but not the Derived portion. 
        That Derived portion has effectively been “sliced off”. 
        
        Because variable base does not have a Derived part, base.getName() resolves to Base::getName().

    Slicing vectors : trying to implement polymorphism with     std::vector
        #include <vector>        
        int main()
        {
            std::vector<Base> v{};
            v.push_back(Base{ 5 }); // add a Base object to our vector
            v.push_back(Derived{ 6 }); // add a Derived object to our vector
        
                // Print out all of the elements in our vector
            for (const auto& element : v)
                std::cout << "I am a " << element.getName() << " with value " << element.getValue() << '\n';
        
            return 0;
        }
        Output:
            I am a Base with value 5
            I am a Base with value 6
        Fixing (wrong way) 
            std::vector<Base&> v{};
            Unfortunately, this won’t compile. The elements of std::vector must be assignable, whereas references can’t be reassigned (only initialized).
        Fixing - Correct way : using pointer
            #include <iostream>
            #include <vector>
            
            int main()
            {
                std::vector<Base*> v{};
                
                Base b{ 5 }; // b and d can't be anonymous objects
                Derived d{ 6 };
            
                v.push_back(&b); // add a Base object to our vector
                v.push_back(&d); // add a Derived object to our vector
            
                // Print out all of the elements in our vector
                for (const auto* element : v)
                    std::cout << "I am a " << element->getName() << " with value " << element->getValue() << '\n';
            
                return 0;
            }
            output:
                I am a Base with value 5
                I am a Derived with value 6
 */

/* // FrankenObject :
        Object slicing due to assigning new derived object, to base class reference/pointer to some other derived object 
        cz, the operator= that C++ provides for classes isn’t virtual by default. */  
#if 0
            class Base
            {
            protected:
                int m_value{};
            
            public:
                Base(int value)
                    : m_value{ value }
                {
                }
            
                virtual const char* getName() const { return "Base"; }
                int getValue() const { return m_value; }
            };
            
            class Derived: public Base
            {
            public:
                Derived(int value)
                    : Base{ value }
                {
                }
            
                virtual const char* getName() const { return "Derived"; }
            };
            
            int main()
            {
                Derived d1{ 5 };
                Derived d2{ 6 };
                Base &b{ d2 };
            
                b = d1; // this line is problematic
            
                return 0;
            }
            /* The first three lines in the function are pretty straightforward. Create two Derived objects, and set a Base reference to the second one.

            The fourth line is where things go astray.
             Since b points at d2, and we’re assigning d1 to b, you might think that the result would be that d1 would get copied into d2 -- and it would, if b were a Derived. But b is a Base, and the operator= that C++ provides for classes isn’t virtual by default. Consequently, only the Base portion of d1 is copied into d2.

            As a result, you’ll discover that d2 now has the Base portion of d1 and the Derived portion of d2. In this particular example, that’s not a problem (because the Derived class has no data of its own), but in most cases, you’ll have just created a Frankenobject -- composed of parts of multiple objects. Worse, there’s no easy way to prevent this from happening (other than avoiding assignments like this as much as possible). */

        //Note: Make sure your function parameters are references (or pointers) and try to avoid any kind of pass-by-value when it comes to derived classes.
#endif

/* 
    dynamic_cast
        upcasting --> Implicitly let you convert a Derived pointer into a Base pointer 
        dynamic_cast --> have a few different capabilities, by far the most common use for dynamic casting is for converting base-class pointers into derived-class pointers. 

        The need for dynamic_cast

            When dealing with polymorphism, you’ll often encounter cases where you have a pointer to a base class, but you want to access some information that exists only in a derived class.

        Consider the following (slightly contrived) program:
 */
#if 0
        #include <iostream>
        #include <string>
        
        class Base
        {
        protected:
            int m_value{};
        
        public:
            Base(int value)
                : m_value{value}
            {
            }
            
            virtual ~Base() = default;
        };
        
        class Derived : public Base
        {
        protected:
            std::string m_name{};
        
        public:
            Derived(int value, const std::string& name)
                : Base{value}, m_name{name}
            {
            }
        
            const std::string& getName() const { return m_name; }
        };
        
        Base* getObject(bool bReturnDerived)
        {
            if (bReturnDerived)
                return new Derived{1, "Apple"};
            else
                return new Base{2};
        }
    #if 0
            int main()
            {
                Base *b{ getObject(true) };
                // how do we print the Derived object's name here, having only a Base pointer?
                delete b;
                return 0;
            }
    #endif
        // How to call Derived::getName() using base pointer
        /* 
            1)add a virtual function to Base called getName() (so we could call it with a Base pointer/reference, and have it dynamically resolve to Derived::getName()). 
                But what would this function return if you called it with a Base pointer/reference that was actually pointing to a Base object? There isn’t really any value that makes sense. 
                Furthermore, we would be polluting our Base class with things that really should only be the concern of the Derived class.
            2)use dynamic_cast
    */
   #if 0
        int main()
        {
            Base *b{ getObject(true) };
        
                Derived *d{ dynamic_cast<Derived*>(b) }; // use dynamic cast to convert Base pointer into Derived pointer
        
                std::cout << "The name of the Derived is: " << d->getName() << '\n';
        
            delete b;
        
            return 0;
        }
    #endif
        // The name of the Derived is: Apple

        /* 
         we’ve made quite a dangerous assumption: that b is pointing to a Derived object.
          What if b wasn’t pointing to a Derived object? This is easily tested by changing the argument to getObject() from true to false.
           In that case, getObject() will return a Base pointer to a Base object. When we try to dynamic_cast that to a Derived, it will fail, because the conversion can’t be made.
         */



/*  dynamic_cast failure :
        for pointers: If a dynamic_cast fails, the result of the conversion will be a null pointer.
        for reference: C++ does not have a “null reference”, dynamic_cast can’t return a null reference upon failure. Instead, if the dynamic_cast of a reference fails, an exception of type std::bad_cast is thrown.
*/
    #if 0
        int main()
        {
            Base *b{ getObject(true) };
        
                Derived *d{ dynamic_cast<Derived*>(b) }; // use dynamic cast to convert Base pointer into Derived pointer
        
                if (d) // make sure d is non-null
                    std::cout << "The name of the Derived is: " << d->getName() << '\n';
        
            delete b;
        
            return 0;
        }

    #endif
   /*  Rule 
        Always ensure your dynamic casts actually succeeded by checking for a null pointer result. */

    /* Note because dynamic_cast does some consistency checking at runtime (to ensure the conversion can be made), use of dynamic_cast does incur a performance penalty.
        Also note that there are several cases where downcasting using dynamic_cast will not work:
        1) With protected or private inheritance.
        2) For classes that do not declare or inherit any virtual functions (and thus don’t have a virtual table).
        3) In certain cases involving virtual base classes (see this page for an example of some of these cases, and how to resolve them). */

#endif

/* 
    Downcasting with static_cast -- fast but dangerous
        It turns out that downcasting can also be done with static_cast. The main difference is that static_cast does no runtime type checking to ensure that what you’re doing makes sense. This makes using static_cast faster, but more dangerous. If you cast a Base* to a Derived*, it will “succeed” even if the Base pointer isn’t pointing to a Derived object. This will result in undefined behavior when you try to access the resulting Derived pointer (that is actually pointing to a Base object).

         If you’re absolutely sure that the pointer you’re downcasting will succeed, then using static_cast is acceptable. One way to ensure that you know what type of object you’re pointing to is to use a virtual function. 
     */
    /* 
    dynamic_cast vs static_cast
         use static_cast unless you’re downcasting, in which case dynamic_cast is usually a better choice.
         However, you should also consider avoiding casting altogether and just using virtual functions. */

    /* 
    Downcasting vs virtual functions
        There are some developers who believe dynamic_cast is evil and indicative of a bad class design. Instead, these programmers say you should use virtual functions.
        In general, using a virtual function should be preferred over downcasting. 
        However, there are times when downcasting is the better choice:

            When you can not modify the base class to add a virtual function (e.g. because the base class is part of the standard library)
            When you need access to something that is derived-class specific (e.g. an access function that only exists in the derived class)
            When adding a virtual function to your base class doesn’t make sense (e.g. there is no appropriate value for the base class to return). Using a pure virtual function may be an option here if you don’t need to instantiate the base class.
     */

    /* 
    A warning about dynamic_cast and RTTI

        Run-time type information (RTTI) is a feature of C++ that exposes information about an object’s data type at runtime. This capability is leveraged by dynamic_cast. 
        Because RTTI has a pretty significant space performance cost, some compilers allow you to turn RTTI off as an optimization.
        Needless to say, if you do this, dynamic_cast won’t function correctly
     */

    /* 
        Printing inherited classes using operator<<
             Can we make Operator << virtual?

                If this issue is that operator<< isn’t virtual, can’t we simply make it virtual?
                The short answer is no. There are a number of reasons for this.

                First, only member functions can be virtualized -- this makes sense, since only classes can inherit from other classes, and there’s no way to override a function that lives outside of a class (you can overload non-member functions, but not override them). Because we typically implement operator<< as a friend, and friends aren’t considered member functions, a friend version of operator<< is ineligible to be virtualized. (For a review of why we implement operator<< this way, please revisit lesson 13.5 -- Overloading operators using member functions).

                Second, even if we could virtualize operator<< there’s the problem that the function parameters for Base::operator<< and Derived::operator<< differ (the Base version would take a Base parameter and the Derived version would take a Derived parameter). Consequently, the Derived version wouldn’t be considered an override of the Base version, and thus be ineligible for virtual function resolution.
        Solution:
             First, we set up operator<< as a friend in our base class as usual. But instead of having operator<< do the printing itself, we delegate that responsibility to a normal member function that can be virtualized!
     */
    #if 1
        #include <iostream>
        class Base
        {
        public:
            // Here's our overloaded operator<<
            friend std::ostream& operator<<(std::ostream &out, const Base &b)
            {
                // Delegate printing responsibility for printing to member function print()
                return b.print(out);
            }
        
            // We'll rely on member function print() to do the actual printing
            // Because print is a normal member function, it can be virtualized
            virtual std::ostream& print(std::ostream& out) const
            {
                out << "Base";
                return out;
            }
        };
        
        class Derived : public Base
        {
        public:
            // Here's our override print function to handle the Derived case
            virtual std::ostream& print(std::ostream& out) const override
            {
                out << "Derived";
                return out;
            }
        };
        
        int main()
        {
            Base b{};
            std::cout << b << '\n';
        
            Derived d{};
            std::cout << d << '\n'; // note that this works even with no operator<< that explicitly handles Derived objects
        
            Base &bref{ d };
            std::cout << bref << '\n';
        
            return 0;
        }

/* The above program works in all three cases:

Base
Derived
Derived

 */
    #endif