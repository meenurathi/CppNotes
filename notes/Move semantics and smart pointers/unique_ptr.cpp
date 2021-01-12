//std::unique_ptr

/* 
    Resource leak:
        when function early returns, or throws an exception, and the pointer is not properly deleted.
    Smart pointer 
        : manages a dynamically allocated resource, and ensures the dynamically allocated object is properly cleaned up at the appropriate time (usually when the smart pointer goes out of scope).

    Note: 
        Smart pointers should never be dynamically allocated themselves
         Otherwise, there is the risk that the smart pointer may not be properly deallocated, which means the object it owns would not be deallocated, causing a memory leak.
         > By always allocating smart pointers on the stack (as local variables or composition members of a class), we’re guaranteed that the smart pointer will properly go out of scope 
            when the function or object it is contained within ends, ensuring the object the smart pointer owns is properly deallocated.

        C++11 standard library ships with 4 smart pointer classes:  
            1) std::auto_ptr (which you shouldn’t use -- it’s being removed in C++17), 
            2) std::unique_ptr - replacement of auto_ptr
            3) std::shared_ptr
            4) std::weak_ptr. 
            std::unique_ptr is by far the most used smart pointer 
 */

/* std::unique_ptr  : <memory> header
        > is an replacement of auto_ptr
        > should completely own the object it manages
        > not share that ownership with other classes. 
        > copy assignment & copy intializationare disabled, only move opr is allowed
            >> to transfer content, we must use std::move, which convert l-value to modifiable r-value
                >>which triggers a move assignment instead of a copy assignment
        > has overloaded  operators * & -> for managed object access
        > has a cast to bool that returns true if the std::unique_ptr is managing a resource.
        > unlike auto_ptr, it remembers whether to use scalar delete or array delete.(however atd::array for fixed array, vector por dynamic array, string for cstyle string is preferred over unique_ptr)
        > use std::make_unique() instead of creating std::unique_ptr and using new yourself, for simplicity & for exception issue resolution due to unknown compiler`s order of evaluation for function parameters
        > std::unique_ptr can be safely returned from a function by value, it calls move contructor due to temp object & transfer ownership of managed objct to the caller
        > pass it to the function by value, using std::move as copy is disabled
        > avoid returning ref or ptr to unique_ptr
        > To get a raw resource pointer from a std::unique_ptr, you can use the get() member function

 */
    #if 0 //example ownership transfer
    #include<iostream>
    #include<memory>
        class Resource{
        public:
            Resource() { std::cout << "Resource acquired\n"; }
            ~Resource() { std::cout << "Resource destroyed\n"; }
        };
        int main(){
            std::unique_ptr<Resource> res1{new Resource{}};
            std::unique_ptr<Resource> res2{};

            std::cout << "res1 is " << (static_cast<bool>(res1) ? "not null\n" : "null\n");
	        std::cout << "res2 is " << (static_cast<bool>(res2) ? "not null\n" : "null\n"); 
            //res2 = res1;//compilation error ,copy assignment is disabled in unique_ptr
            res2 = std::move(res1);//// res2 assumes ownership, res1 is set to null

            std::cout << "Ownership transferred\n";
 
            std::cout << "res1 is " << (static_cast<bool>(res1) ? "not null\n" : "null\n");
            std::cout << "res2 is " << (static_cast<bool>(res2) ? "not null\n" : "null\n");
        
            return 0;
        }
        /* output
            Resource acquired
            res1 is not null
            res2 is null
            Ownership transferred
            res1 is null
            res2 is not null
            Resource destroyed
         */

    #endif

    /* 
    Accessing the managed object

        std::unique_ptr has overloaded operators * &->, which can be used to return the resource being managed.
             an overloaded operator*   :returns a reference to the managed resource
             and overloaded operator-> : returns a pointer.
                 

        Remember that std::unique_ptr may not always be managing an object -- 
            either because it was created empty (using the default constructor or passing in a nullptr as the parameter),   or because the resource it was managing got moved to another std::unique_ptr. 
        So before we use either of these operators(*, ->), 
            we should check whether the std::unique_ptr actually has a resource.
                std::unique_ptr has a cast to bool that returns true if the std::unique_ptr is managing a resource.
     */
        #include <iostream>
        #include <memory> // for std::unique_ptr
        
        class Resource
        {
        public:
            Resource() { std::cout << "Resource acquired\n"; }
            ~Resource() { std::cout << "Resource destroyed\n"; }
            friend std::ostream& operator<<(std::ostream& out, const Resource &res)
            {
                out << "I am a resource\n";
                return out;
            }
        };
    #if 0
         int main(){
            std::unique_ptr<Resource> res{new Resource{}};
            // use implicit cast to bool to ensure res contains a Resource
            if(res){
		        std::cout << *res << '\n'; // print the Resource that res is owning
            }
            return 0;
        }
        /* 
            Resource acquired
            I am a resource
            Resource destroyed
         */
    #endif

/* std::unique_ptr and arrays

    Unlike std::auto_ptr, std::unique_ptr is smart enough to know whether to use scalar delete or array delete, 
    so std::unique_ptr is okay to use with both scalar objects and arrays.

    However, std::array or std::vector (or std::string) are almost always better choices than using std::unique_ptr with a fixed array, dynamic array, or C-style string.

Rule:>>> Favor std::array, std::vector, or std::string over a smart pointer managing a fixed array, dynamic array, or C-style string

*/
/*  //std::make_unique <memory header> recommended over unique_ptr self creation

        > added in C++14 
        > This templated function
            >> constructs an dynamic object of the template type 
            >> and initializes it with the arguments passed into the function.
        >Use of std::make_unique() is optional, but is recommended over creating std::unique_ptr yourself. 
            This is because 
                code using std::make_unique is simpler,
                and it also requires less typing (when used with automatic type deduction).
                Furthermore it resolves an exception safety issue that can result from C++ leaving the order of evaluation for function arguments unspecified.

        //rule: use std::make_unique() instead of creating std::unique_ptr and using new yourself
 */
#if 0

        #include <memory> // for std::unique_ptr and std::make_unique
        #include <iostream>
        class Fraction
        {
        private:
            int m_numerator{ 0 };
            int m_denominator{ 1 };
        
        public:
            Fraction(int numerator = 0, int denominator = 1) :
                m_numerator{ numerator }, m_denominator{ denominator }
            {
            }
        
            friend std::ostream& operator<<(std::ostream& out, const Fraction &f1)
            {
                out << f1.m_numerator << '/' << f1.m_denominator;
                return out;
            }
        };
        int main(){
            // Create a single dynamically allocated Fraction with numerator 3 and denominator 5
            auto f1{std::make_unique<Fraction>(3,5)};
            std::cout << *f1 << '\n';
            // Create a dynamically allocated array of Fractions of length 4
            auto f2{std::make_unique<Fraction[]>(4)};
            return 0;
        }

        /* output
            3/5
            0/1
         */
#endif

/* 

    The exception safety issue resolved by using make_unique() 
    e.g.
        some_function(std::unique_ptr<T>(new T),
                function_that_can_throw_exception());
    The compiler is given a lot of flexibility in terms of how it handles this call. 
        > It could create a new T,
        > then call function_that_can_throw_exception(), 
        > then create the std::unique_ptr that manages the dynamically allocated T.
        ! If function_that_can_throw_exception() throws an exception,
             then the T that was allocated will not be deallocated, 
                because the smart pointer to do the deallocation hasn’t been created yet.
                This leads to T being leaked.

    std::make_unique() doesn’t suffer from this problem because 
        the creation of the object T 
        and the creation of the std::unique_ptr 
    happen inside the std::make_unique() function,
         where there’s no ambiguity about order of execution.
 */

//Returning std::unique_ptr from a function
#if 0
    #include<memory>
    std::unique_ptr<Resource> createResource(){
        return std::make_unique<Resource>();
    }
    int main2(){
        auto ptr{createResource()};
        return 0;
    }
#endif
    /* 
        In the above code, createResource() returns a std::unique_ptr by value. 
        If this value is not assigned to anything, the temporary return value will go out of scope and the Resource will be cleaned up. 
        If it is assigned (as shown in main()),
            > in C++14 or earlier, move semantics will be employed to transfer the Resource from the return value to the object assigned to (in the above example, ptr),
            > and in C++17 or newer, the return will be elided. This makes returning a resource by std::unique_ptr much safer than returning raw pointers!

        In general, you should not return std::unique_ptr by pointer (ever) or reference (unless you have a specific compelling reason to).
     */

    //Passing std::unique_ptr to a function
  /*      
     If you want the function to take ownership of the contents of the pointer, pass the std::unique_ptr by value. 
     
     Note that because copy semantics have been disabled, you’ll need to use std::move to actually pass the variable in. 
     */
    #if 0
        void takeOwnership(std::unique_ptr<Resource> res){
            if (res){
                std::cout << *res << '\n';
            }
        }//// the Resource is destroyed here
        int main(){
            auto ptr{std::make_unique<Resource>()};
            //takeOwnership(ptr);// This doesn't work, need to use move semantics
            takeOwnership(std::move(ptr)); // ok: use move semantics
            std::cout << "Ending program\n";
            return 0;
        }
        /* output
            Resource acquired
            I am a resource

            Resource destroyed
            Ending program
         */

        /* 
        Note that in this case, ownership of the Resource was transferred to takeOwnership(), so the Resource was destroyed at the end of takeOwnership() rather than the end of main().
         */
    #endif

    /* 
    
    However, most of the time, you won’t want the function to take ownership of the resource.
     Although you can pass a std::unique_ptr by reference (which will allow the function to use the object without assuming ownership),
         you should only do so when the called function might alter or change the object being managed.

        Instead, it’s better to just pass the resource itself (by pointer or reference, depending on whether null is a valid argument).     
            This allows the function to remain agnostic of how the caller is managing its resources.
     To get a raw resource pointer from a std::unique_ptr, you can use the get() member function:
     */

    #if 0
    // The function only uses the resource, so we'll accept a pointer to the resource, not a reference to the whole std::unique_ptr<Resource>
        void useResource(Resource * res){
	        if (res)
		    std::cout << *res << '\n';
        }
        int main(){
            auto ptr{std::make_unique<Resource>()};
            useResource(ptr.get());// note: get() used here to get a pointer to the Resource
            std::cout << "Ending program\n";
            return 0;
        }// The Resource is destroyed here
/* 
        Resource acquired
        I am a resource

        Ending program
        Resource destroyed
 */
    #endif

    //std::unique_ptr and classes
/* 
    You can, of course, use std::unique_ptr as a composition member of your class. 
        This way, you don’t have to worry about ensuring your class destructor deletes the dynamic memory, as the std::unique_ptr will be automatically destroyed when the class object is destroyed. 
        However, do note that if your class object is dynamically allocated, the object itself is at risk for not being properly deallocated, in which case even a smart pointer won’t help.
 */
    //Misusing std::unique_ptr

    /* 
    There are two easy ways to misuse std::unique_ptrs, both of which are easily avoided.
        First, don’t let multiple classes manage the same resource. For example:

                Resource *res{ new Resource() };
                std::unique_ptr<Resource> res1{ res };
                std::unique_ptr<Resource> res2{ res };
            While this is legal syntactically, the end result will be that both res1 and res2 will try to delete the Resource, which will lead to undefined behavior.

        Second, don’t manually delete the resource out from underneath the std::unique_ptr.

                Resource *res{ new Resource() };
                std::unique_ptr<Resource> res1{ res };
                delete res;
            If you do, the std::unique_ptr will try to delete an already deleted resource, again leading to undefined behavior.

        Note that std::make_unique() prevents both of the above cases from happening inadvertently.
     */

    //Practice
    //Convert the following program from using a normal pointer to using std::unique_ptr where appropriate:
    #if 0
        #include <iostream>
        
        class Fraction
        {
        private:
            int m_numerator{ 0 };
            int m_denominator{ 1 };
        
        public:
            Fraction(int numerator = 0, int denominator = 1) :
                m_numerator{ numerator }, m_denominator{ denominator }
            {
            }
        
            friend std::ostream& operator<<(std::ostream& out, const Fraction &f1)
            {
                out << f1.m_numerator << '/' << f1.m_denominator;
                return out;
            }
        };
        
        void printFraction(const Fraction* const ptr)
        {
            if (ptr)
                std::cout << *ptr << '\n';
        }
        
        int main()
        {
            auto *ptr{ new Fraction{ 3, 5 } };
        
            printFraction(ptr);
        
            delete ptr;
        
            return 0;
        }
        #endif
    //converted
        #if 1
        #include <iostream>
        #include<memory>
        
        class Fraction
        {
        private:
            int m_numerator{ 0 };
            int m_denominator{ 1 };
        
        public:
            Fraction(int numerator = 0, int denominator = 1) :
                m_numerator{ numerator }, m_denominator{ denominator }
            {
            }
        
            friend std::ostream& operator<<(std::ostream& out, const Fraction &f1)
            {
                out << f1.m_numerator << '/' << f1.m_denominator;
                return out;
            }
        };
        
        void printFraction(const Fraction* const ptr)
        {
            if (ptr)
                std::cout << *ptr << '\n';
        }
        
        int main()
        {
            auto ptr{ std::make_unique<Fraction>( 3, 5 ) };
            printFraction(ptr.get());
            return 0;
        }
        #endif