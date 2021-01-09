//  first smart pointer class auto_ptr added in c++98 , move sematics using copy constructor & assignmnet operator & 
// ---> not preferred
#if 0
#include<iostream>

class Resource{
    public:
        Resource(){ 
            std::cout<<"Resource acquired \n";
        }
        ~Resource(){
            std::cout<<"Resource Destroyed \n";
        }
        void sayHi(){
            std::cout<<" Hi!!\n";
        }
};

template<class T>
class Auto_ptr{
    T * m_ptr;
public:
    Auto_ptr(T *ptr =nullptr):m_ptr(ptr){}
    ~Auto_ptr(){ delete m_ptr;}

    //delete copy & assigment
    //Auto_ptr(const Auto_ptr & src) = delete;
   // Auto_ptr& operator=(const Auto_ptr &src) = delete;

    // move implemented by copy constr
    Auto_ptr(Auto_ptr &src){// note: not const
        // m_ptr = src.m_ptr;/// transfer our dumb pointer from the source to our local object
        src.m_ptr = nullptr;//// make sure the source no longer owns the pointer
    }

    // An assignment operator that implements move semantics
    Auto_ptr & operator=(Auto_ptr &src){// note: not const
        if(&src == this) //self copy
            return *this;
        delete m_ptr;// make sure we deallocate any pointer the destination is already holding first
        m_ptr = src.m_ptr;/// then transfer our dumb pointer from the source to the local object
        src.m_ptr = nullptr;// make sure the source no longer owns the pointer
        return *this;
    }

    //override * & -> so that Auto_ptr objct can be used in a similar way that m_ptr would
    T& operator*(){ return *m_ptr;}
    T* operator->(){ return m_ptr; }
    bool isNull(){ return m_ptr == nullptr;}
};

int main(){

    Auto_ptr<Resource> res1{new Resource};
    Auto_ptr<Resource> res2{};
    std::cout << "res1 is " << (res1.isNull() ? "null\n" : "not null\n");
	std::cout << "res2 is " << (res2.isNull() ? "null\n" : "not null\n");
    std::cout << "Ownership transferred\n";
    res2 = res1;// res2 assumes ownership, res1 is set to null. so no duplicate copies of Resource
    std::cout << "res1 is " << (res1.isNull() ? "null\n" : "not null\n");
	std::cout << "res2 is " << (res2.isNull() ? "null\n" : "not null\n");
    res2->sayHi();
    return 0;
}

/* 
This program prints:

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
std::auto_ptr, and why to avoid it
    std::auto_ptr, introduced in C++98, was C++’s first attempt at a standardized smart pointer. 
    std::auto_ptr opted to implement move semantics just like the Auto_ptr class does.

    However, std::auto_ptr (and our Auto_ptr class) has a number of problems that makes using it dangerous.

        First, because std::auto_ptr implements move semantics through the copy constructor and assignment operator, passing a std::auto_ptr by value to a function will cause your resource to get moved to the function parameter (and be destroyed at the end of the function when the function parameters go out of scope). Then when you go to access your auto_ptr argument from the caller (not realizing it was transferred and deleted), you’re suddenly dereferencing a null pointer. Crash!

        Second, std::auto_ptr always deletes its contents using non-array delete. This means auto_ptr won’t work correctly with dynamically allocated arrays, because it uses the wrong kind of deallocation. Worse, it won’t prevent you from passing it a dynamic array, which it will then mismanage, leading to memory leaks.

        Finally, auto_ptr doesn’t play nice with a lot of the other classes in the standard library, including most of the containers and algorithms. This occurs because those standard library classes assume that when they copy an item, it actually makes a copy, not a move.

    Because of the above mentioned shortcomings, std::auto_ptr has been deprecated in C++11, and it should not be used. In fact, std::auto_ptr is slated for complete removal from the standard library as part of C++17!

Rule: std::auto_ptr is deprecated and should not be used. (Use std::unique_ptr or std::shared_ptr instead)..

    The core problem with the design of std::auto_ptr is that prior to C++11, the C++ language simply had no mechanism to differentiate “copy semantics” from “move semantics”. Overriding the copy semantics to implement move semantics leads to weird edge cases and inadvertent bugs.
    For example, you can write res1 = res2 and have no idea whether res2 will be changed or not!

    Because of this, in C++11, the concept of “move” was formally defined, and “move semantics” were added to the language to properly differentiate copying from moving. 

    In C++11, std::auto_ptr has been replaced by a bunch of other types of “move-aware” smart pointers: std::unique_ptr, std::weak_ptr, and std::shared_ptr. 
 */