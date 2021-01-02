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

#if 1
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