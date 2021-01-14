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