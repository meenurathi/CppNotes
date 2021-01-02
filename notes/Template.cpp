/* 
    a template is a model that serves as a pattern for creating similar objects”
    In C++, function templates are functions that serve as a pattern for creating other similar functions. The basic idea behind function templates is to create a function without having to specify the exact type(s) of some or all of the variables. Instead, we define the function using placeholder types, called template type parameters.
        int max(int x, int y)
        {
            return (x > y) ? x : y;
        }
       
 */
#if 0
    template<typename T> //// this is the template parameter declaration,  T is a placeholder type
    T max (T x, T y){ // template definition
        return x<y;
    }

#endif

/* 
         We start with the keyword template -- this tells the compiler that what follows is going to be a list of template parameters.
          We place all of our parameters inside angled brackets (<>). 
  
    Note::  To create a template type parameter, use either the keyword typename or class. There is no difference between the two keywords in this context, so which you use is up to you. If you use the class keyword, the type passed in does not actually have to be a class (it can be a fundamental variable, pointer, or anything else that matches). Then you name your type (usually “T”).

          If the template function uses multiple template type parameter, they can be separated by commas:
            template <typename T1, typename T2>
            // template function here

        One final note: Because the function argument passed in for type T could be a class type, and it’s generally not a good idea to pass classes by value, it would be better to make the parameters and return types of our templated function const references:

*/

#if 0
            #include<iostream>
            template <typename T>
            const T& max(const T& x, const T& y)
            {
                return (x > y) ? x : y;
            }
            int main()
            {
                int i = max(3, 7); // returns 7
                std::cout << i << '\n';
            
                double d = max(6.34, 18.523); // returns 18.523
                std::cout << d << '\n';
            
                char ch = max('a', '6'); // returns 'a'
                std::cout << ch << '\n';
            
                return 0;
            }
           
#endif
/* Advantage of using templates
    Template functions can save a lot of time, because you only need to write one function, and it will work with many different types. 
    Template functions reduce code maintenance, because duplicate code is reduced significantly.
    template functions can be safer, because there is no need to copy functions and change types by hand whenever you need the function to work with a new type!
  Drawbacks:
    First, some older compilers do not have very good template support. However, this downside is no longer as much of a problem as it used to be. 
    Second, template functions often produce crazy-looking error messages that are much harder to decipher than those of regular functions (we’ll see an example of this in the next lesson). 
    Third, template functions can increase your compile time and code size, as a single template might be “realized” and recompiled in many files (there are ways to work around this one).

However, these drawbacks are fairly minor compared with the power and flexibility templates bring to your programming toolkit!
*/

/*
    Function template instances
        It turns out that C++ does not compile the template function directly. Instead, at compile time, when the compiler encounters a call to a template function, it replicates the template function and replaces the template type parameters with actual types. The function with actual types is called a function template instance.

        Let’s take a look at an example of this process. First, we have a templated function:
        template <typename T> // this is the template parameter declaration
        const T& max(const T& x, const T& y)
        {
            return (x > y) ? x : y;
        }
        When compiling your program, the compiler encounters a call to the templated function:
        int i{ max(3, 7) }; // calls max(int, int)
        The compiler says, “oh, we want to call max(int, int)”. The compiler replicates the function template and creates the template instance max(int, int):
        const int& max(const int &x, const int &y)
        {
            return (x > y) ? x : y;
        }
        This is now a “normal function” that can be compiled into machine language.

        Now, let’s say later in your code, you called max() again using a different type:
        double d{ max(6.34, 18.523) }; // calls max(double, double)
        C++ automatically creates a template instance for max(double, double):

        const double& max(const double &x, const double &y)
        {
            return (x > y) ? x : y;
        }
        and then compiles it.

        The compiler is smart enough to know it only needs to create one template instance per set of unique type parameters (per file). It’s also worth noting that if you create a template function but do not call it, no template instances will be created.

       >> Operators, function calls, and function templates

        Template functions will work with both built-in types (e.g. char, int, double, etc…) and classes, with one caveat. When the compiler compiles the template instance, it compiles it just like a normal function. 
            In a normal function, any operators or function calls that you use with your types must be defined, or you will get a compiler error. 
            Similarly, any operators or function calls in your template function must be defined for any types the function template is instantiated for. Let’s take a look at this in more detail.

            First, we’ll create a simple class:
                class Cents
                {
                private:
                    int m_cents;
                public:
                    Cents(int cents)
                        : m_cents{ cents }
                    {
                    }
                };
            Now, let’s see what happens when we try to call our templated max() function with the Cents class:

                template <typename T> // this is the template parameter declaration
                const T& max(const T& x, const T& y)
                {
                    return (x > y) ? x : y;
                }
                
                class Cents
                {
                private:
                    int m_cents;
                public:
                    Cents(int cents)
                        : m_cents{ cents }
                    {
                    }
                };
                
                int main()
                {
                    Cents nickle{ 5 };
                    Cents dime{ 10 };
                
                    Cents bigger{ max(nickle, dime) };
                
                    return 0;
                }
            C++ will create a template instance for max() that looks like this:

                const Cents& max(const Cents &x, const Cents &y)
                {
                    return (x > y) ? x : y;
                }
            And then it will try to compile this function. See the problem here? C++ can’t evaluate x > y, because x and y are Cents class objects, and doesn’t know how to compare them. Consequently, this will produce a fairly-tame looking compile error, like this:

                1>c:\consoleapplication1\main.cpp(4): error C2676: binary '>': 'const Cents' does not define this operator or a conversion to a type acceptable to the predefined operator
                1>  c:\consoleapplication1\main.cpp(23): note: see reference to function template instantiation 'const T &max(const T &,const T &)' being compiled
                1>          with
                1>          [
                1>              T=Cents
                1>          ]
            The top error message points out the fact that there is no overloaded operator > for the Cents class.
            The bottom error points out the templated function call that spawned the error, along with the type of the templated parameter.

            To get around this problem, simply overload the > operator for any class we wish to use max() with:
                class Cents
                {
                private:
                    int m_cents;
                public:
                    Cents(int cents)
                        : m_cents{ cents }
                    {
                    }
                
                    friend bool operator>(const Cents &c1, const Cents &c2)
                    {
                        return (c1.m_cents > c2.m_cents);
                    }
                };
            Now C++ will know how to compare x > y when x and y are objects of the Cents class! As a result, our max() function will now work with two objects of type Cents.
 */



//>>> template classes
                /* template declaration
                template <class T>
                      class Array {};                      
                      or
                 template <typename T>
                      class Array {};
                */
#if 0
            #ifndef ARRAY_H
            #define ARRAY_H
            
            #include <cassert>
            
            template <class T>
            class Array
            {
            private:
                int m_length{};
                T *m_data{};
            
            public:
            
                Array(int length)
                {
                    assert(length > 0);
                    m_data = new T[length]{};
                    m_length = length;
                }
            
                Array(const Array&) = delete;
                Array& operator=(const Array&) = delete;
            
                ~Array()
                {
                    delete[] m_data;
                }
            
                void Erase()
                {
                    delete[] m_data;
                    // We need to make sure we set m_data to 0 here, otherwise it will
                    // be left pointing at deallocated memory!
                    m_data = nullptr;
                    m_length = 0;
                }
            
                T& operator[](int index)
                {
                    assert(index >= 0 && index < m_length);
                    return m_data[index];
                }
            
                // templated getLength() function defined below
                int getLength() const; 
            };
            
            // member functions defined outside the class need their own template declaration
            template <class T>
            int Array<T>::getLength() const // note class name is Array<T>, not Array
            {
            return m_length;
            }
            //////////////////////////////
            #include <iostream>
            #include "Array.h"
            
            int main()
            {
                Array<int> intArray(12);
                Array<double> doubleArray(12);
            
                for (int count{ 0 }; count < intArray.getLength(); ++count)
                {
                    intArray[count] = count;
                    doubleArray[count] = count + 0.5;
                }
            
                for (int count{ intArray.getLength() - 1 }; count >= 0; --count)
                    std::cout << intArray[count] << '\t' << doubleArray[count] << '\n';
            
                return 0;
            }
            #endif
            /*             
             Each templated member function defined outside the class declaration needs its own template declaration.
              Also, note that the name of the templated array class is Array<T>, not Array -- Array would refer to a non-templated version of a class named Array, unless Array is used inside of the class. For example, the copy constructor and copy-assignment operator used Array rather than Array<T>.
              When the class name is used without template arguments inside of the class, the arguments are the same as the ones of the current instantiation.

Note:   Template classes are instanced in the same way template functions are --
             the compiler stencils out a copy upon demand, with the template parameter replaced by the actual data type the user needs, and then compiles the copy. 
             If you don’t ever use a template class, the compiler won’t even compile it.
         */
#endif
   
/*Splitting up template classes 
        A template is not a class or a function -- it is a stencil used to create classes or functions.
        In order for the compiler to use a template, it must see both 
            the template definition (not just a declaration) 
            and the template type used to instantiate the template.
        Also, remember that C++ compiles files individually. 

 managing template header files :
        normal practice :move function definitions in .cpp :
        
            the common procedure is to put the class definition in a header file,
            and the member function definitions in a similarly named code file. 
            In this way, the source for the class is compiled as a separate project file.
            example: 

            Array.cpp:
                #include "Array.h"        
                template <class T>
                int Array<T>::getLength() const // note class name is Array<T>, not Array
                {
                return m_length;
                }
            main.cpp:
                #include "Array.h"        
                int main()
                {
                    Array<int> intArray(12);
                    Array<double> doubleArray(12);

                    for (int count{ 0 }; count < intArray.getLength(); ++count)
                    {
                        intArray[count] = count;
                        doubleArray[count] = count + 0.5;
                    }
                    for (int count{ intArray.getLength() - 1 }; count >= 0; --count)
                        std::cout << intArray[count] << '\t' << doubleArray[count] << '\n';
                    return 0;
                }
            linker error:    The above program will compile, but cause a linker error:
                    unresolved external symbol "public: int __thiscall Array::getLength(void)" (?GetLength@?$Array@H@@QAEHXZ)

              When the compiler sees that we need two template instances, Array<int>, and Array<double>, it will instantiate these, and compile them as part of main.cpp. However, when it gets around to compiling Array.cpp separately, it will have forgotten that we need an Array<int> and Array<double>, so that template function is never instantiated. Thus, we get a linker error, because the compiler can’t find a definition for Array<int>::getLength() or Array<double>::getLength().

        solution:
            1)  put all of your template class code in the header file (in this case, put the contents of Array.cpp into Array.h, below the class). 
                In this way, when you #include the header, all of the template code will be in one place. 
                Advantage: simple.
                Disadvantage: if the template class is used in many places, you will end up with many local copies of the template class, 
                which can increase your compile and link times (your linker should remove the duplicate definitions, so it shouldn’t bloat your executable). 
                This is our preferred solution unless the compile or link times start to become a problem.
            2)  Rename Array.cpp to Array.inl (.inl stands for inline), 
                and then include Array.inl from the bottom of the Array.h header. 
                That yields the same result as putting all the code in the header, but helps keep things a little cleaner.
            3) #including .cpp files, but we don’t recommend these because of the non-standard usage of #include.
                    With this solution, you'd need to potentially modify array.cpp for each program, which introduces risk of breaking either array.cpp or a dependent program.
            4) three-file approach. 
              File 1 : Header - The template class definition goes in the header. 
              File 2 : code (.cpp) - The template class member functions goes in the code file. 
              File 3 : template (.cpp) -  instantiations - which contains all of the instantiated classes you need:
                templates.cpp:
*/
#if 0
                 // Ensure the full Array template definition can be seen
                #include "Array.h"
                #include "Array.cpp" // we're breaking best practices here, but only in this one place
                
                // #include other .h and .cpp template definitions you need here
                
                template class Array<int>; // Explicitly instantiate template Array<int>
                template class Array<double>; // Explicitly instantiate template Array<double>
                
                // instantiate other templates here
                /* 
                    the “template class” command causes the compiler to explicitly instantiate the template class. 
                    In the above case, the compiler will stencil out both Array<int> and Array<double> inside of templates.cpp.
                    Because templates.cpp is inside our project, this will then be compiled.
                    These functions can then be linked to from elsewhere.
                    This method is more efficient, but requires maintaining the templates.cpp file for each program.
                 */
#endif
/*
            The goal is to separate the reusable parts from the program-specific parts. 
            In the three-file solution, the Array.h and Array.cpp are totally reusable. Only the third file (templates.cpp) needs to be modified per program.
                what if you wanted an Array of some user-defined data structure?
                 With solution 3, you'd have to have Array.cpp include that user-defined data structure, which then means that user-defined data structure will be included in all your other programs that use Array.cpp. Not good.
            With the three-file solution, only the third file needs to include the user-defined data structure, which is fine because that file is program specific anyway.   
 */

//Template non-type parameters
/* 
    A template non-type parameter is a special type of parameter that does not substitute for a type, but is instead replaced by a value. 
    A non-type parameter can be any of the following:
        A value that has an integral type or enumeration
        A pointer or reference to a class object
        A pointer or reference to a function
        A pointer or reference to a class member function
        std::nullptr_t

    In the following example, we create a non-dynamic (static) array class that uses both a type parameter and a non-type parameter. The type parameter controls the data type of the static array, and the non-type parameter controls how large the static array is.
    #include <iostream>
 */
#if 0
        template <class T, int size> // size is the non-type parameter
        class StaticArray
        {
        private:
            // The non-type parameter controls the size of the array
            T m_array[size];
        
        public:
            T* getArray();
            
            T& operator[](int index)
            {
                return m_array[index];
            }
        };
        
        // Showing how a function for a class with a non-type parameter is defined outside of the class
        template <class T, int size>
        T* StaticArray<T, size>::getArray()
        {
            return m_array;
        }
        
        int main()
        {
            // declare an integer array with room for 12 integers
            StaticArray<int, 12> intArray;
        
            // Fill it up in order, then print it backwards
            for (int count=0; count < 12; ++count)
                intArray[count] = count;
        
            for (int count=11; count >= 0; --count)
                std::cout << intArray[count] << " ";
            std::cout << '\n';
        
            // declare a double buffer with room for 4 doubles
            StaticArray<double, 4> doubleArray;
        
            for (int count=0; count < 4; ++count)
                doubleArray[count] = 4.4 + 0.1*count;
        
            for (int count=0; count < 4; ++count)
                std::cout << doubleArray[count] << ' ';
        
            return 0;
        }
        /* This code produces the following:
        11 10 9 8 7 6 5 4 3 2 1 0
        4.4 4.5 4.6 4.7
        Here, we do not have to dynamically allocate the m_array member variable! 
        This is because for any given instance of the StaticArray class, size is actually constant. 
        For example, if you instantiate a StaticArray<int, 12>, the compiler replaces size with 12. 
           Thus m_array is of type int[12], which can be allocated statically.

      Note:  This functionality is used by the standard library class std::array. 
      When you allocate a std::array<int, 5>, the int is a type parameter, and the 5 is a non-type parameter! */
#endif


/* 
— Function template specialization - implemet templated function slightly different for a specific data type
        --sometimes called a full or explicit function template specialization
        When instantiating a function template for a given type,
        the compiler stencils out a copy of the templated function and replaces the template type parameters with the actual types used in the variable declaration.
        This means a particular function will have the same implementation details for each instanced type (just using different types). 
        Some times, there are cases where it is useful to implement a templated function slightly different for a specific data type.

        Template specialization is one way to accomplish this.
        example: */
        #if 0
            template <class T>
            class Storage
            {
            private:
                T m_value;
            public:
                Storage(T value)
                {
                    m_value = value;
                }
                ~Storage()
                {
                }
                void print()
                {
                    std::cout << m_value << '\n';
                }
            };
            //The above code will work fine for many data types:

            int main()
            {
                // Define some storage units
                Storage<int> nValue(5);
                Storage<double> dValue(6.7);
            
                // Print out some values
                nValue.print();
                dValue.print();
            }
            /* This prints:
            5
            6.7
            Now, let’s say we want double values (and only double values) to output in scientific notation. 
            To do so, we can use a function template specialization to create a specialized version of the print() function for type double. 
               simply define the specialized function (if the function is a member function, do so outside of the class definition),
               replacing the template type with the specific type you wish to redefine the function for. 
            Here is our specialized print() function for doubles: */
            template <>
            void Storage<double>::print()
            {
                std::cout << std::scientific << m_value << '\n';
            }
            /* When the compiler goes to instantiate Storage<double>::print(), it will see we’ve already explicitly defined that function, and it will use the one we’ve defined instead of stenciling out a version from the generic templated class.

        The template <> tells the compiler that this is a template function, but that there are no template parameters (since in this case, we’re explicitly specifying all of the types). 

            As a result, when we rerun the above program, it will print:
            5
            6.700000e+000
            */
        /*
           >>>>>>> Another example Storage class with datatype char*:
        */

            int main()
            {
                // Dynamically allocate a temporary string
                char *string = new char[40];
            
                // Ask user for their name
                std::cout << "Enter your name: ";
                std::cin >> string;
            
                // Store the name
                Storage<char*> storage(string);
            
                // Delete the temporary string
                delete[] string;
            
                // Print out our value
                storage.print(); // This will print garbage
            }
            /* As it turns out, instead of printing the name the user input, storage.print() prints garbage! 
            When Storage is instantiated for type char*, the constructor for Storage<char*> looks like this:
             */
                template <>
                Storage<char*>::Storage(char* value)
                {
                    m_value = value;
                }
            /* In other words, this just does a pointer assignment (shallow copy)! 
            As a result, m_value ends up pointing at the same memory location as string.
             When we delete string in main(), we end up deleting the value that m_value was pointing at! And thus, we get garbage when trying to print that value.

            Fortunately, we can fix this problem using template specialization. 
            Instead of doing a pointer copy, we’d really like our constructor to make a copy of the input string.
            */
                template <>
                Storage<char*>::Storage(char* value)
                {
                    // Figure out how long the string in value is
                    int length=0;
                    while (value[length] != '\0')
                        ++length;
                    ++length; // +1 to account for null terminator
                
                    // Allocate memory to hold the value string
                    m_value = new char[length];
                
                    // Copy the actual value string into the m_value memory we just allocated
                    for (int count=0; count < length; ++count)
                        m_value[count] = value[count];
                }
            /*  Now when we allocate a variable of type Storage<char*>, this constructor will get used instead of the default one. 
            As a result, m_value will receive its own copy of string. Consequently, when we delete string, m_value will be unaffected.

            However, this class now has a memory leak for type char*, because m_value will not be deleted when a Storage variable goes out of scope. As you might have guessed, this can also be solved by specializing the Storage<char*> destructor:
                */
                template <>
                Storage<char*>::~Storage()
                {
                    delete[] m_value;
                }
        /* Now when variables of type Storage<char*> go out of scope, the memory allocated in the specialized constructor will be deleted in the specialized destructor.
            Although the above examples have all used member functions, you can also specialize non-member template functions in the same way.
         */
 #endif

 /* 
    Class template specialization

        Class template specialization allows us to specialize a template class for a particular data type (or data types, if there are multiple template parameters).

        Class template specializations are treated as completely independent classes, even though they are allocated in the same way as the templated class. This means that we can change anything and everything about our specialization class, including the way it’s implemented and even the functions it makes public, just as if it were an independent class. Here’s our specialized class:
                    
            following is example of class template specialization to write a customized version of Storage8<bool> that will take precedence over the generic Storage8<T> class.
            This works analogously to how a specialized function takes precedence over a generic template function.
  */
 #if 0
    //generic version
        template<typename T>
        class Storage8{
            T m_array[8]{};
        public :
            void set(int idx, const T & val){ m_array[idx] = val;}
            const T& get(int idx)const { return m_array[idx] ;}
        };
    //specialized version for bool to save memory
        template<>
        class Storage8<bool>{
            unsigned char m_data{};
        public:
            void set(int idx, bool val){
                auto mask{1 <<idx};
                if(val)//need to add 1
                {
                    m_data |= mask; //Use bitwise-or to turn that bit on
                }else{//reset 
                    m_data &= !mask; //bitwise-and the inverse mask to turn that bit off
                }
            }
            bool get(int idx){
                auto mask{1<< idx};
                 // bitwise-and to get the value of the bit we're interested in
                // Then implicit cast to boolean
                return (m_data & mask);
            }
        };
        /* 
            First, we start off with template<>.
                The template keyword tells the compiler that what follows is templated, 
                and the empty angle braces means that there aren’t any template parameters.
                In this case, there aren’t any template parameters because we’re replacing the only template parameter (T) with a specific type (bool).
            Next, we add <bool> to the class name to denote that we’re specializing a bool version of class Storage8.
         */
        #include<iostream>
        int main()
        {
            // Define a Storage8 for integers (instantiates Storage8<T>, where T = int)
            Storage8<int> intStorage;
        
            for (int count{ 0 }; count < 8; ++count)
            {
                intStorage.set(count, count);
            }
        
            for (int count{ 0 }; count<8; ++count)
            {
                std::cout << intStorage.get(count) << '\n';
            }
        
            // Define a Storage8 for bool  (instantiates Storage8<bool> specialization)
            Storage8<bool> boolStorage;
            
            for (int count{ 0 }; count < 8; ++count)
            {
                boolStorage.set(count, count & 3);
            }
        
            std::cout << std::boolalpha;
        
            for (int count{ 0 }; count < 8; ++count)
            {
                std::cout << boolStorage.get(count) << '\n';
            }
        
            return 0;
        }
        /* 
            0
            1
            2
            3
            4
            5
            6
            7
            false
            true
            true
            true
            false
            true
            true
            true
         */
       // note:  keeping the public interface between your template class and all of the specializations identical is generally a good idea,
       // as it makes them easier to use -- however, it’s not strictly necessary.
 #endif

 /* 
        >> Partial template specialization

 
  */
 #if 0
    //This class takes two template parameters, a type parameter, and an expression parameter.
        #include <iostream>
        #include <cstring>
        template<class T, int size>// size is the expression parameter
        class StaticArray{
            T m_data[size]{};// The expression parameter controls the size of the array
        public:
            T * getArray(){ return m_data;}
            T & operator[](int idx){ return m_data[idx];}
        };
        //non member function 
        template<class T, int size>
        void print(StaticArray<T, size> &array){
            for (int count{ 0 }; count < size; ++count)
                std::cout << array[count] << ' ';
        }
        // main that works:
        int main1()
        {
            // declare an int array
            StaticArray<int, 4> int4{};
            int4[0] = 0;
            int4[1] = 1;
            int4[2] = 2;
            int4[3] = 3;        
            // Print the array
            print(int4); //0 1 2 3          
            return 0;
        }
         //issue
        int main()
        {
            // declare a char array
            StaticArray<char, 14> char14{};        
            std::strcpy(char14.getArray(), "Hello, world!");
            // Print the array
            print(char14); //H e l l o ,   w o r l d !
            return 0;
        }
       // space is not desired for printing string
    /*sol?
       1) full specialization : using full template specialization means we have to explicitly define the length of the array this function will accept!
                                We’d have to copy the function for each different array size. That’s redundant.
    */
            // print no space for StaticArray<char, 14>
            template<>
            void print(StaticArray<char, 14> & array){
                for (int count{ 0 }; count < 14; ++count)
                    std::cout << array[count] ;
            } // output >> Hello, world!
    /*  2) Partial specialization : only for class not for fucntion
 */
        // overload of print() function for partially specialized StaticArray<char, size>
            template <int size> // size is still a templated expression parameter
            void print(StaticArray<char, size> &array) // we're explicitly defining type char here
            {
                for (int count{ 0 }; count < size; ++count)
                    std::cout << array[count];
            }
            /* 
              As you can see here, we’ve explicitly declared that this function will only work for StaticArray of type char,
               but size is still a templated expression parameter, so it will work for char arrays of any size.
             */
            int main2()
            {
                // Declare an char array of size 14
                StaticArray<char, 14> char14{};
                std::strcpy(char14.getArray(), "Hello, world!");
                // Print the array
                print(char14);
                // Now declare an char array of size 12
                StaticArray<char, 12> char12{};
                std::strcpy(char12.getArray(), "Hello, mom!");
                // Print the array
                print(char12);
                return 0;
            }
            //out  : Hello, world! Hello, mom!

            /* 
            note: Note that as of C++14, partial template specialization can only be used with classes, not template functions (functions must be fully specialized). Our void print(StaticArray<char, size> &array) example works because the print function is not partially specialized (it’s just an overloaded function using a class parameter that’s partially specialized).
             */
#endif
 //>>  Partial template specialization for member functions
#if 0
       #include <iostream>
        #include <cstring>
            template <class T, int size> // size is the expression parameter
            class StaticArray
            {
            private:
                // The expression parameter controls the size of the array
                T m_array[size]{};
            
            public:
                T* getArray() { return m_array; }
                
                T& operator[](int index)
                {
                    return m_array[index];
                }
        // print() is now a member function of class StaticArray<T, int>. 
            
                void print()
                {
                    for (int i{ 0 }; i < size; ++i)
                        std::cout << m_array[i] << ' ';
                    std::cout << '\n';
                }
            };
        //So what happens when we want to partially specialize print(), so that it works differently? :
            // Doesn't work
           /*      template <int size>
                void StaticArray<double, size>::print()
                {
                    for (int i{ 0 }; i < size; ++i)
                        std::cout << std::scientific << m_array[i] << ' ';
                    std::cout << '\n';
                } */
        //Unfortunately, this doesn’t work, because we’re trying to partially specialize a function, which is disallowed.

        //So how do we get around this? 
        //1)One obvious way is to partially specialize the entire class:

            /* #include<iostream>
            template <class T, int size> // size is the expression parameter
            class StaticArray
            {
            private:
                // The expression parameter controls the size of the array
                T m_array[size]{};
            public:
                T* getArray() { return m_array; }
                T& operator[](int index)
                {
                    return m_array[index];
                }
                void print()
                {
                    for (int i{ 0 }; i < size; ++i)
                        std::cout << m_array[i] << ' ';
                    std::cout << "\n";
                }
            };
            template <int size> // size is the expression parameter
            class StaticArray<double, size>
            {
            private:
                // The expression parameter controls the size of the array
                double m_array[size]{};
            public:
                double* getArray() { return m_array; }
            
                double& operator[](int index)
                {
                    return m_array[index];
                }
                void print()
                {
                    for (int i{ 0 }; i < size; ++i)
                        std::cout << std::scientific << m_array[i] << ' ';
                    std::cout << '\n';
                }
            };
            int main()
            {
                // declare an integer array with room for 6 integers
                StaticArray<int, 6> intArray{};
                // Fill it up in order, then print it
                for (int count{ 0 }; count < 6; ++count)
                    intArray[count] = count;
                intArray.print();
                // declare a double buffer with room for 4 doubles
                StaticArray<double, 4> doubleArray{};
                for (int count{ 0 }; count < 4; ++count)
                    doubleArray[count] = (4.0 + 0.1 * count);
                doubleArray.print();
                return 0;
            }
            This prints:
            0 1 2 3 4 5
            4.000000e+00 4.100000e+00 4.200000e+00 4.300000e+00
     While it works, this isn’t a great solution, because we had to duplicate a lot of code from StaticArray<T, size> to StaticArray<double, size>.
 */
    /*  2)   If only there were some way to reuse the code in StaticArray<T, size> in StaticArray<double, size>. Sounds like a job for inheritance!
        You might start off trying to write that code like this: */
        /* 
            template <int size> // size is the expression parameter
            class StaticArray<double, size>: public StaticArray< // Then what?
            How do we reference StaticArray? We can’t.
 */
       // 3)  there’s a workaround, by using a common base class:
                #include<iostream>
                template <class T, int size> // size is the expression parameter
                class StaticArray_Base
                {
                protected:
                    // The expression parameter controls the size of the array
                    T m_array[size]{};
                public:
                    T* getArray() { return m_array; }
                    T& operator[](int index)
                    {
                        return m_array[index];
                    }
                    void print()
                    {
                        for (int i{ 0 }; i < size; ++i)
                            std::cout << m_array[i];
                        std::cout << '\n';
                    }
                    virtual ~StaticArray_Base() = default;
                };
                
                template <class T, int size> // size is the expression parameter
                class StaticArray: public StaticArray_Base<T, size>
                {
                public:
                };
                
                template <int size> // size is the expression parameter
                class StaticArray<double, size>: public StaticArray_Base<double, size>
                {
                public:
                
                    void print()
                    {
                        for (int i{ 0 }; i < size; ++i)
                            std::cout << std::scientific << this->m_array[i] << ' ';
                // note: The this-> prefix in the above line is needed.
                // See https://stackoverflow.com/a/6592617 or https://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members for more info on why.
                        std::cout << '\n';
                    }
                };
                
                int main()
                {
                    // declare an integer array with room for 6 integers
                    StaticArray<int, 6> intArray{};
                    // Fill it up in order, then print it
                    for (int count{ 0 }; count < 6; ++count)
                        intArray[count] = count;
                    intArray.print();
                    // declare a double buffer with room for 4 doubles
                    StaticArray<double, 4> doubleArray{};
                    for (int count{ 0 }; count < 4; ++count)
                        doubleArray[count] = (4.0 + 0.1 * count);
                    doubleArray.print();
                    return 0;
                }
        //This prints the same as above, but has significantly less duplicated code
 #endif

 //>> Partial template specialization for pointers

 #if 0

        #include <iostream>
        template <class T>
        class Storage
        {
        private:
            T m_value;
        public:
            Storage(T value)
            {
                m_value = value;
            }
            ~Storage()
            {            }
            void print()
            {
                std::cout << m_value << '\n';
            }
        };
        /* 
        we’ll use class partial template specialization to define a special version of the Storage class that works for pointer values. This class is considered partially specialized because we’re telling the compiler that it’s only for use with pointer types, even though we haven’t specified the underlying type exactly. */
        template <typename T>
        class Storage<T*> // this is a partial-specialization of Storage that works with pointer types
        {
        private:
            T* m_value;
        public:
            Storage(T* value) // for pointer type T
            {
                // For pointers, we'll do a deep copy
                m_value = new T(*value); // this copies a single value, not an array
            }
        
            ~Storage()
            {
                delete m_value; // so we use scalar delete here, not array delete
            }
        
            void print()
            {
                std::cout << *m_value << '\n';
            }
        };

        int main()
        {
            // Declare a non-pointer Storage to show it works
            Storage<int> myint(5);
            myint.print();
        
            // Declare a pointer Storage to show it works
            int x = 7;
            Storage<int*> myintptr(&x);
        
            // Let's show that myintptr is separate from x.
            // If we change x, myintptr should not change
            x = 9;
            myintptr.print();
        
            return 0;
        }
        //output 5 7
#endif

        /* 
        
        It’s worth noting that because this partially specialized Storage class only allocates a single value, 
        for C-style strings, only the first character will be copied. 
        If the desire is to copy entire strings, a specialization of the constructor (and destructor) for type char* can be fully specialized.
note:
         The fully specialized version will take precedence over the partially specialized version. Here’s an example program that uses both partial specialization for pointers, and full specialization for char*:
    */
 #if 0
        // Full specialization of constructor for type char*
        #include <iostream>
        #include <cstring>
        
        // Our Storage class for non-pointers
        template <class T>
        class Storage
        {
        private:
            T m_value;
        public:
            Storage(T value)
            {
                m_value = value;
            }
            ~Storage()
            {            }
            void print()
            {
                std::cout << m_value << '\n';
            }
        };
        
        // Partial-specialization of Storage class for pointers
        template <class T>
        class Storage<T*>
        {
        private:
            T* m_value;
        public:
            Storage(T* value)
            {
                m_value = new T(*value);
            }
        
            ~Storage()
            {
                delete m_value;
            }
        
            void print()
            {
                std::cout << *m_value << '\n';
            }
        };
        // Full specialization of constructor for type char*
        template <>
        Storage<char*>::Storage(char* value)
        {
            // Figure out how long the string in value is
            int length = 0;
            while (value[length] != '\0')
                ++length;
            ++length; // +1 to account for null terminator
        
            // Allocate memory to hold the value string
            m_value = new char[length];
        
            // Copy the actual value string into the m_value memory we just allocated
            for (int count = 0; count < length; ++count)
                m_value[count] = value[count];
        }
        
        // Full specialization of destructor for type char*
        template<>
        Storage<char*>::~Storage()
        {
            delete[] m_value;
        }
        
        // Full specialization of print function for type char*
        // Without this, printing a Storage<char*> would call Storage<T*>::print(), which only prints the first element
        template<>
        void Storage<char*>::print()
        {
            std::cout << m_value;
        }
        
        int main()
        {
            // Declare a non-pointer Storage to show it works
            Storage<int> myint(5);
            myint.print();
        
            // Declare a pointer Storage to show it works
            int x = 7;
            Storage<int*> myintptr(&x);
        
            // If myintptr did a pointer assignment on x,
            // then changing x will change myintptr too
            x = 9;
            myintptr.print();
        
            // Dynamically allocate a temporary string
            //char *name = new char[40]{ "Alex" }; // requires C++14
        
            // If your compiler isn't C++14 compatible, comment out the above line and uncomment these
        	char *name = new char[40];
        	strcpy(name, "Alex");
        
            // Store the name
            Storage< char*> myname(name);
        
            // Delete the temporary string
            delete[] name;
        
            // Print out our name
            myname.print();
        }

        /* out: 
            5
            7
            Alex
        Using partial template class specialization to create separate pointer and non-pointer implementations of a class is extremely useful 
        when you want a class to handle both differently, but in a way that’s completely transparent to the end-user.
         */
 #endif