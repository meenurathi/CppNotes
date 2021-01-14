/* 
        std::shared_ptr
            > <memory> // header

            >  multiple smart pointer can share/co-own same resources

            >  It keeps track of the number of smart pointers pointing to the managed resource

            >  As soon as the last std::shared_ptr managing the resource goes out of scope (or is reassigned to point at something else), the resource will be deallocated.

            >  Always make a copy of an existing std::shared_ptr if you need more than one std::shared_ptr pointing to the same resource.

            > make_shared :  For unique pointer  std::make_unique() is preferred over directly creating unique_ptr
                For shared pointer  std::make_shared() is preferrred over directly creating
                Reason:
                1) it is simpler , easy to use using auto keyword, less typing
                2) it is safer: there is no way to directly create two shared_ptr pointing directly to the same resource using this method.
                3) it is more performant than not using it. Reason of this lie in the way that shared_ptr keeps track of how many pointers are pointing at a given resource.

            > unique ptr keeps single pointer (managed source) internally
            > shared pointer uses two pointers internally.
                1) managed resource: pointer to the resource being managed
                2) control block :dynamically allocated object that keep tracks of many things ref count :
                 how many shared ptr are pointing at the resource

            > when shared ptr is created by via shared_ptr ctr, memory for the managed object (which is usaually passed in ) & control block( which conctructor creates ) are allocated seperately.
                > > using make_shared this can be optimized into single memory allocation, which leads to better performance.
                    -imp ->> `std::make_shared` uses _placement-new_.

                        It first allocates N bytes of memory without creating an object.
                        Then it uses placement-new to create the control block (If required) and another placement-new to create the object. 
                        Placement-new doesn't allocate any memory, it creates an object at a predefined address.

                        Without `std::make_shared`, you'd
                        1. allocate memory for your object and create the object (Both using a single `new` call)
                        2. have `std::make_shared` allocate memory for- and create the control block.
                        = 2 allocations

           > Shared pointer can be created using unique ptr but not vie versa   
                >> this is done using spl shared_ptr constructor that takes unique_ptr r value. It moves content of unique ptr to shared ptr
            >Shared ptr cannot be safely converted into unique_ptr
                >> if you need to return smart pointer from any function, return unique ptr & assign it to shared ptr if & when that`s appropriate

            > it itself can be leaked if it was allocated dynamically.
                >>If the `std::shared_ptr` doesn't die, the resource doesn't die.

            >managing resource arrays: in c++14 & earlier shared ptr doesnt have proper supoport for managing arrays.
                > this is added in c++17 but make_shared is still lacking support for arrays & should not be used

            > layout
            In a typical implementation, std::shared_ptr holds only two pointers:
                the stored pointer (one returned by get())
                a pointer to control block

            The control block is a dynamically-allocated object that holds:
                - either a pointer to the managed object or the managed object itself
                - the deleter (type-erased)
                - the allocator (type-erased)
                - the number of shared_ptrs that own the managed object
                - the number of weak_ptrs that refer to the managed object
                The pointer held by the shared_ptr directly is the one returned by get(), while the pointer or object held by the control block is the one that will be deleted when the number of shared owners reaches zero. 
                These pointers are not necessarily equal.
 */
    #if 1
        #include<iostream>
        #include<memory>// std::shared_ptr
        class Resource
        {
        public:
            Resource() { std::cout << "Resource acquired\n"; }
            ~Resource() { std::cout << "Resource destroyed\n"; }
        };
    #endif
     #if 0 // right way of sharing
        int main(){
            // allocate a Resource object and have it owned by std::shared_ptr
            Resource * res { new Resource{}};
            std::shared_ptr<Resource> ptr1{res};
            {
                // use copy initialization to make another std::shared_ptr pointing to the same thing
                std::shared_ptr<Resource> ptr2{ptr1};
                std::cout << "Killing one shared pointer\n";
            }// ptr2 goes out of scope here, but nothing happens
            std::cout << "Killing another shared pointer\n";
            return 0;
        }// ptr1 goes out of scope here, and the allocated Resource is destroyed
        /* output
            Resource acquired
            Killing one shared pointer
            Killing another shared pointer
            Resource destroyed
         */
     #endif
     #if 0// wrong way
        int main(){
            Resource * res { new Resource};
            std::shared_ptr<Resource> ptr1{res};
            {
                // create ptr2 directly from res (instead of ptr1)
                std::shared_ptr<Resource> ptr2{res};
                 std::cout << "Killing one shared pointer\n";
            }// ptr2 goes out of scope here, and the allocated Resource is destroyed
             std::cout << "Killing another  shared pointer\n";
            return 0;
        }// ptr1 goes out of scope here, and the allocated Resource is destroyed again
        /* Output: crash on some machine as resource is destroyed twice
            Resource acquired
            Killing one shared pointer
            Resource destroyed
            Killing another  shared pointer
            Resource destroyed        
         */

     #endif

     /* 
     The difference here is that we created two std::shared_ptr independently from each other. 
        As a consequence, even though they’re both pointing to the same Resource, they aren’t aware of each other.
         When ptr2 goes out of scope, it thinks it’s the only owner of the Resource, and deallocates it. 
         When ptr1 later goes out of the scope, it thinks the same thing, and tries to delete the Resource again. Then bad things happen.

    Fortunately, this is easily avoided by using copy assignment or copy initialization when you need multiple shared pointers pointing to the same Resource.
      */
    /* 
     Rule:
      Always make a copy of an existing std::shared_ptr if you need more than one std::shared_ptr pointing to the same resource.
    */

   //std::make_shared
    /* 
      
      Much like std::make_unique() can be used to create a std::unique_ptr in C++14, std::make_shared() can (and should) be used to make a std::shared_ptr. std::make_shared() is available in C++11.

        how does one declare/initialize a std::shared_ptr member variable in the constructor initialization list with std::make_shared?
            Anthony()
                : m_pointer{ std::make_shared<Pointer>(1, 2, 3) }
                {}
       */
      #if 0
        int main(){
            //// allocate a Resource object and have it owned by std::shared_ptr
            auto ptr1{ std::make_shared<Resource>()};
            {
                 // create ptr2 using copy initialization of ptr1
                auto ptr2 = ptr1;
                std::cout << "Killing one shared pointer\n";
            }// ptr2 goes out of scope here, but nothing happens
 
	        std::cout << "Killing another shared pointer\n";
	        return 0;
        }// // ptr1 goes out of scope here, and the allocated Resource is destroyed

        /* 
            Resource acquired
            Killing one shared pointer
            Killing another shared pointer
            Resource destroyed
        
         */
      #endif
/* 
    coversion from unique to shared    
 */
    #if 0
        int main(){
            // allocate a Resource object and have it owned by std::shared_ptr
            auto ptr1{std::make_unique<Resource>()};
            {
                // create ptr2 using rvalue of unique ptr, which willc ause moving of resource from unique to shared
                std::shared_ptr<Resource> ptr2 {std::move(ptr1)};                
                std::cout << "Killing one shared pointer\n";
            }/// ptr2 goes out of scope here, resource is destroyed here  
            std::cout << "Killing unique pointer\n";
            return 0;
        }// ptr1 goes out of scope here, and It doesnt has resource to be destroyed
        /* output
            Resource acquired
            Killing one shared pointer
            Resource destroyed        
            Killing unique pointer
         */
    #endif

    /* 
    why are two different pointer(one with shareptr  & another inside control block) needed for the managed object (in addition to the pointer to the control block)? Doesn't the one returned by get() suffice? And why aren't these pointers necessarily equal?
    https://stackoverflow.com/questions/34046070/why-are-two-raw-pointers-to-the-managed-object-needed-in-stdshared-ptr-impleme
     */

#if 1
     /*   The reason for this is that you can have a shared_ptr which points to something else than what it owns, and that is by design. This is implemented using the constructor listed as nr. 8 on cppreference:

        template< class Y >
        shared_ptr( const shared_ptr<Y>& r, T *ptr );
    A shared_ptr created with this constructor shares ownership with r, but points to ptr. Consider this (contrived, but illustrating) code: */

        std::shared_ptr<int> creator1()
        {
            using Pair = std::pair<int, double>;

            std::shared_ptr<Pair> p(new Pair(42, 3.14));
            std::shared_ptr<int> q(p, &(p->first));
            return q;
        }
        /*  
            Once this function exits, only a pointer to the int subobject of the pair is available to client code. But because of the shared ownership between q and p, the pointer q keeps the entire Pair object alive.

            Once dealloacation is supposed to happen, the pointer to the entire Pair object must be passed to the deleter. Hence the pointer to the Pair object must be stored somewhere alongside the deleter—in other words, in the control block.
        */
       /*
            For a less contrived example (probably even one closer to the original motivation for the feature), consider the case of pointing to a base class. Something like this: */
                struct Base1
                {
                // :::
                };
                struct Base2
                {
                // :::
                };
                struct Derived : Base1, Base2
                {
                // :::
                };
                std::shared_ptr<Base2> creator()
                {
                    std::shared_ptr<Derived> p(new Derived());
                    std::shared_ptr<Base2> q(p, static_cast<Base2*>(p.get()));
                    return q;
                }
          /*   Of course, the real implementation of std::shared_ptr has all the implicit conversions in place so that the p-and-q dance in creator is not necessary, but I've kept it there to resemble the first example. */



#endif


      /* 
        Shared pointer class implementation
       */
      #if 0
      template<typename T>
      class SharedPointer{
          template typename<T>
          class ControlBlock{
              public:
                T * m_managedObj{};
                int m_refCount{};
                ControlBlock(T* ptr)
                    :m_managedObj(ptr), m_refCount{1} {
                }
                int getRefCount(){ return m_refcount;};
                int getObj(){ return m_managedObj;}
                ~ControlBlock(){
                    std::cout<<"delete m_managedObj\n";
                    delete m_managedObj;
                }
                void operator++(){ ++m_refCount;}
                int operator++(int) { 
                    auto ret {m_refCount};
                    ++m_refCount;
                    return ret;
                }
                void operator--(){ --m_refCount;}
                int operator(int){ 
                    auto ret {m_refCount};
                    --m_refCount;
                    return ret;
                }
            };
        ControlBlock<T> *m_controlBlock;
        SharedPointer(T *ptr= nullptr)
            : m_controlBlock{new m_controlBlock<T>{ptr}}{}
        //copy
        SharedPointer

      };
      #endif

/* // to avoid circular dependency weak_ptr are used
        > //we cannot access managed objct via weak ptr
            - to access managed objct get shared_ptr from  weak_ptr using weakr_ptr`s lock() member function. If shared ptr & its control block is gone, weak.lock() returns null ptr
            - weak_ptr.expired() can we used to check if it is danging or not
                std::shared_ptr<Resource> ptr1{ new Resource()};
                std::weak_ptr<Resource> ptrw = shared_ptr;
                auto ptr2 = ptrw.lock();
                ptr2->print();
        */
       //eg
        #if 0

           #include <iostream>
            #include <memory> // for std::shared_ptr
            #include <string>
            
            class Person
            {
                std::string m_name;
                std::shared_ptr<Person> m_partner; // initially created empty
            
            public:
                    
                Person(const std::string &name): m_name(name)
                { 
                    std::cout << m_name << " created\n";
                }
                ~Person()
                {
                    std::cout << m_name << " destroyed\n";
                }
            
                friend bool partnerUp(std::shared_ptr<Person> &p1, std::shared_ptr<Person> &p2)
                {
                    if (!p1 || !p2)
                        return false;
            
                    p1->m_partner = p2;
                    p2->m_partner = p1;
            
                    std::cout << p1->m_name << " is now partnered with " << p2->m_name << "\n";
            
                    return true;
                }
            };
            
            int main()
            {
                auto lucy = std::make_shared<Person>("Lucy"); // create a Person named "Lucy"
                auto ricky = std::make_shared<Person>("Ricky"); // create a Person named "Ricky"
            
                partnerUp(lucy, ricky); // Make "Lucy" point to "Ricky" and vice-versa
            
                return 0;
            }

            /* 
                 Lucy created
                Ricky created
                Lucy is now partnered with Ricky

                And that’s it. No deallocations took place. Uh. oh. What happened?

                After partnerUp() is called, there are two shared pointers pointing to “Ricky” (ricky, and Lucy’s m_partner) and two shared pointers pointing to “Lucy” (lucy, and Ricky’s m_partner).
                At the end of main(), the ricky shared pointer goes out of scope first. When that happens, ricky checks if there are any other shared pointers that co-own the Person “Ricky”. There are (Lucy’s m_partner). Because of this, it doesn’t deallocate “Ricky” (if it did, then Lucy’s m_partner would end up as a dangling pointer). At this point, we now have one shared pointer to “Ricky” (Lucy’s m_partner) and two shared pointers to “Lucy” (lucy, and Ricky’s m_partner).

                Next the lucy shared pointer goes out of scope, and the same thing happens. The shared pointer lucy checks if there are any other shared pointers co-owning the Person “Lucy”. There are (Ricky’s m_partner), so “Lucy” isn’t deallocated. At this point, there is one shared pointer to “Lucy” (Ricky’s m_partner) and one shared pointer to “Ricky” (Lucy’s m_partner).

                Then the program ends -- and neither Person “Lucy” or “Ricky” have been deallocated! Essentially, “Lucy” ends up keeping “Ricky” from being destroyed, and “Ricky” ends up keeping “Lucy” from being destroyed.

                It turns out that this can happen any time shared pointers form a circular reference.
             */
        #endif
        /* Circular references

            A Circular reference (also called a cyclical reference or a cycle) is a series of references where each object references the next, and the last object references back to the first, causing a referential loop.
            The references do not need to be actual C++ references -- they can be pointers, unique IDs, or any other means of identifying specific objects.
 */
       /*  In the context of shared pointers, the references will be pointers.

            This is exactly what we see in the case above: “Lucy” points at “Ricky”, and “Ricky” points at “Lucy”. 
            With three pointers, you’d get the same thing when A points at B, B points at C, and C points at A. 
        The practical effect of having shared pointers form a cycle is that each object ends up keeping the next object alive -- with the last object keeping the first object alive. 
        Thus, no objects in the series can be deallocated because they all think some other object still needs it! */

       /*  A reductive case
            It turns out, this cyclical reference issue can even happen with a single std::shared_ptr -- a std::shared_ptr referencing the object that contains it is still a cycle (just a reductive one).
        */
        #if 0
                #include <iostream>
                #include <memory> // for std::shared_ptr
                
                class Resource
                {
                public:
                    std::shared_ptr<Resource> m_ptr; // initially created empty
                    
                    Resource() { std::cout << "Resource acquired\n"; }
                    ~Resource() { std::cout << "Resource destroyed\n"; }
                };
                
                int main()
                {
                    auto ptr1 = std::make_shared<Resource>();
                
                    ptr1->m_ptr = ptr1; // m_ptr is now sharing the Resource that contains it
                
                    return 0;
                }
                /* output:
                    Resource acquired
                     and that’s it.
                    In the above example, when ptr1 goes out of scope, it doesn’t deallocate the Resource because the Resource’s m_ptr is sharing the Resource. 
                    Then there’s nobody left to delete the Resource (m_ptr never goes out of scope, so it never gets a chance). 
                 */
            #endif
    
       // So what is std::weak_ptr for anyway?
        /* 
            > std::weak_ptr was designed to solve the “cyclical ownership” problem described above.
            > A std::weak_ptr is an observer -- it can observe and access the same object as a std::shared_ptr (or other std::weak_ptrs) but it is not considered an owner. 
            > Remember, when a std::shared pointer goes out of scope, it only considers whether other std::shared_ptr are co-owning the object. std::weak_ptr does not count!
        */

       // Let’s solve our Person-al issue using a std::weak_ptr:
        #if 0
            #include <iostream>
            #include <memory> // for std::shared_ptr and std::weak_ptr
            #include <string>
            
            class Person
            {
                std::string m_name;
                std::weak_ptr<Person> m_partner; // note: This is now a std::weak_ptr
            
            public:
                    
                Person(const std::string &name): m_name(name)
                { 
                    std::cout << m_name << " created\n";
                }
                ~Person()
                {
                    std::cout << m_name << " destroyed\n";
                }
            
                friend bool partnerUp(std::shared_ptr<Person> &p1, std::shared_ptr<Person> &p2)
                {
                    if (!p1 || !p2)
                        return false;
            
                    p1->m_partner = p2;
                    p2->m_partner = p1;
            
                    std::cout << p1->m_name << " is now partnered with " << p2->m_name << "\n";
            
                    return true;
                }
            };
            
            int main()
            {
                auto lucy = std::make_shared<Person>("Lucy");
                auto ricky = std::make_shared<Person>("Ricky");
            
                partnerUp(lucy, ricky);
            
                return 0;
            }
         /*    This code behaves properly:
            Lucy created
            Ricky created
            Lucy is now partnered with Ricky
            Ricky destroyed
            Lucy destroyed
            Functionally, it works almost identically to the problematic example. However, now when ricky goes out of scope, it sees that there are no other std::shared_ptr pointing at “Ricky” (the std::weak_ptr from “Lucy” doesn’t count). Therefore, it will deallocate “Ricky”. The same occurs for lucy.
            */
        #endif 
    /*    
        // Using std::weak_ptr

            > The downside of std::weak_ptr is that std::weak_ptr are not directly usable (they have no operator->). 
                >> To use a std::weak_ptr, you must first convert it into a std::shared_ptr. Then you can use the std::shared_ptr.
                >> To convert a std::weak_ptr into a std::shared_ptr, you can use the lock() member function. 
                Here’s the above example, updated to show this off: */
         #if 0 
            #include <iostream>
            #include <memory> // for std::shared_ptr and std::weak_ptr
            #include <string>
            
            class Person
            {
                std::string m_name;
                std::weak_ptr<Person> m_partner; // note: This is now a std::weak_ptr
            
            public:
            
                Person(const std::string &name) : m_name(name)
                {
                    std::cout << m_name << " created\n";
                }
                ~Person()
                {
                    std::cout << m_name << " destroyed\n";
                }
            
                friend bool partnerUp(std::shared_ptr<Person> &p1, std::shared_ptr<Person> &p2)
                {
                    if (!p1 || !p2)
                        return false;
            
                    p1->m_partner = p2;
                    p2->m_partner = p1;
            
                    std::cout << p1->m_name << " is now partnered with " << p2->m_name << "\n";
            
                    return true;
                }
            
                const std::shared_ptr<Person> getPartner() const { return m_partner.lock(); } // use lock() to convert weak_ptr to shared_ptr
                const std::string& getName() const { return m_name; }
            };
            
            int main()
            {
                auto lucy = std::make_shared<Person>("Lucy");
                auto ricky = std::make_shared<Person>("Ricky");
            
                partnerUp(lucy, ricky);
            
                auto partner = ricky->getPartner(); // get shared_ptr to Ricky's partner
                std::cout << ricky->getName() << "'s partner is: " << partner->getName() << '\n';
            
                return 0;
            }
           /*  This prints:

            Lucy created
            Ricky created
            Lucy is now partnered with Ricky
            Ricky's partner is: Lucy
            Ricky destroyed
            Lucy destroyed
            We don’t have to worry about circular dependencies with std::shared_ptr variable “partner” since it’s just a local variable inside the function. It will eventually go out of scope at the end of the function and the reference count will be decremented by 1. */
        #endif
      /* 
      
        1a) std::unique_ptr
            std::unique_ptr should be used when you want a smart pointer to manage a dynamic object that is not going to be shared.

        1b) std::shared_ptr
            std::shared_ptr should be used when you want a smart pointer to manage a dynamic object that may be shared. The object won’t be deallocated until all std::shared_ptr holding the object are destroyed.

        1c) std::weak_ptr
            std::weak_ptr should be used when you want access to an object that is being managed by a std::shared_ptr, but don’t want the lifetime of the std::shared_ptr to be tied to the lifetime of the std::weak_ptr.

        1d) std::auto_ptr
            std::auto_ptr is deprecated and slated for removal in C++17. It should not be used.

        2) Explain how r-values references enable move semantics.
            Because r-values are temporary, we know they are going to get destroyed after they are used. When passing or return an r-value by value, it’s wasteful to make a copy and then destroy the original. Instead, we can simply move (steal) the r-value’s resources, which is generally more efficient.
      
       */