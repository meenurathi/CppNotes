#include<iostream>
using std::cin;
using std::cout;
/* ...> Exception of operator overloading
 ():?, scope ::,,member selector .,   member pointer selector.*,  casting operator <>, typeid, sizeof, 
 */

/*  ....Resolving overloaded operators

        When evaluating an expression containing an operator, the compiler uses the following rules:

        >If all of the operands are fundamental data types, the compiler will call a built-in routine if one exists. If one does not exist, the compiler will produce a compiler error.
        >If any of the operands are user data types (e.g. one of your classes, or an enum type), the compiler looks to see whether the type has a matching overloaded operator function that it can call. If it can’t find one, it will try to convert one or more of the user-defined type operands into fundamental data types so it can use a matching built-in operator (via an overloaded typecast). If that fails, then it will produce a compile error. */
/* 
....What are the limitations on operator overloading?

        1) almost any existing operator in C++ can be overloaded. The exceptions are: conditional (?:), sizeof, scope (::), member selector (.), member pointer selector (.*), typeid, and the casting operators.

        2) you can only overload the operators that exist. You can not create new operators or rename existing operators. For example, you could not create an operator ** to do exponents.

        3) at least one of the operands in an overloaded operator must be a user-defined type. This means you can not overload the plus operator to work with one integer and one double. However, you could overload the plus operator to work with an integer and a Mystring.

        4) it is not possible to change the number of operands an operator supports.

        5)precedence cannot be changed:  all operators keep their default precedence and associativity (regardless of what they’re used for) and this can not be changed.
 */

/*... types of way to overload
        1) member function way
        2) friend function way
        3) standard operator() way*/

///... Friend function way
        class Cents{
            int m_cents;
        public:
            Cents(int cents=0):m_cents(cents){};
            int getCents() const { return m_cents; }

            friend Cents operator+(const Cents & c1, const Cents & c2);
            friend Cents operator-(const Cents & c1, const Cents & c2);
        };
        // note: these functions are not a member functions!
        Cents operator+(const Cents & c1, const Cents & c2){
            return {c1.m_cents + c2.m_cents};
        }
        Cents operator-(const Cents & c1, const Cents & c2){
            return {c1.m_cents - c2.m_cents};
        }
         int main1()
        {
            Cents cents1{ 6 };
            Cents cents2{ 2 };
            Cents centsSum{ cents1 - cents2 };
            std::cout << "I have " << centsSum.getCents() << " cents.\n";
        
            return 0;
        }

//............. Friend functions can be defined inside the class (not preferred)

//..Overloading operators for operands of different types
     // -> provide diff function to resolve it
     //Cents2(4) + 6 would call operator+(Cents2, int), and 6 + Cents2(4) would call operator+(int, Cents)
        class Cents2{
            int m_cents;
        public:
            Cents2(int cents=0):m_cents(cents){};
            int getCents() const { return m_cents; }

            friend Cents2 operator+(const Cents2 & c1, const Cents2 & c2){
                cout<<" both class var \n";
                return {c1.m_cents + c2.m_cents};
            }
            friend Cents2 operator+(const Cents2 & c1, int val){
                 cout<<" first class var & 2nd int \n";
                return {c1.m_cents + val};
            }
            friend Cents2 operator+(int val , const Cents2 & c1){
                cout<<" first  int & 2nd class var \n";
                return {c1 + val};//to avoid reimplementation of same functionality
            }
        };
        int main2()
        {
            Cents2 c1{ Cents2{ 4 } + 6 };
            Cents2 c2{ 6 + Cents2{ 4 } };
            Cents2 c3{ c1 +c2 };
            cout<<"...\n";
            Cents2 c4{ c1 + c2 + 5 + 8 + c3 + 16 };
            Cents2 c5{(((((c1+c2) + 5 )+ 8 )+ c3 )+ 16 )};

            std::cout << "I have c1 " << c1.getCents() << " cents.\n";
            std::cout << "I have c2 " << c2.getCents() << " cents.\n";
            std::cout << "I have c3 " << c3.getCents() << " cents.\n";
            std::cout << "I have c4 " << c4.getCents() << " cents.\n";
            std::cout << "I have c5 " << c5.getCents() << " cents.\n";
            return 0;
        }
    //overloaded operators can call other overloaded operators to simplify its implementation
/*//    prog still works if we remove below 2 functions        
           friend Cents2 operator+(const Cents2 & c1, int val);
           friend Cents2 operator+(int val , const Cents2 & c1);
        because of following 2, anonymous objt is constructed from literal, using constructor &
        then operator+ is called with both current objt & anaonymus objt
            friend Cents2 operator+(const Cents2 & c1, const Cents2 & c2)
            Cents2(int cents=0):m_cents(cents)
        // however if we remove keyword const from operator+, it wont work
            as anonymous objt act as rvalue, it cannot be non-const references cannot bind to temporaries
*/

/* ... Normal function type (preferred over private)
        -> it is neither friend nor member function
        access private var via getter functions
        
        Cents operator+(const Cents &c1, const Cents &c2){
                // use the Cents constructor and operator+(int, int)
                // we don't need direct access to private members here
                return { c1.getCents() + c2.getCents() };
        }
         */

/* 
note --> Not everything can be overloaded as a friend function

        The assignment (=), subscript ([]), function call (()), and member selection (->) operators must be overloaded as member functions, because the language requires them to be.

    --> Not everything can be overloaded as a member function :
        --- eg. >> , <<
            Because the overloaded operator must be added as a member of the left operand
        --- although we can overload operator+(Cents, int) as a member function (as we did above), we can’t overload operator+(int, Cents) as a member function, because int isn’t a class we can add members to.

Typically, we won’t be able to use a member overload if the left operand is either not a class */

//Rules >.. which one to prefer
        /* 
        1) If you’re overloading assignment (=), subscript ([]), function call (()), or member selection (->), do so as a member function.
        2) If you’re overloading a unary operator, do so as a member function.
        3) If you’re overloading a binary operator that does not modify its left operand (e.g. operator+), do so as a normal function (preferred) or friend function.
        4) If you’re overloading a binary operator that modifies its left operand, but you can’t modify the definition of the left operand (e.g. operator<<, which has a left operand of type ostream), do so as a normal function (preferred) or friend function.
        5) If you’re overloading a binary operator that modifies its left operand (e.g. operator+=), and you can modify the definition of the left operand, do so as a member function.
 */

// unary !, -, + -->memberfunction
       #include <iostream>
        
        class Point
        {
        private:
            double m_x, m_y, m_z;
        
        public:
            Point(double x=0.0, double y=0.0, double z=0.0):
                m_x{x}, m_y{y}, m_z{z}
            {}
            Point operator-(){ return {-m_x, -m_y, - m_z};}
            Point operator+(){ return *this;/*no change */}
            //Point operator!(){ return {!m_x, !m_y, !m_z};}
            Point &operator--(){//prefix--
                    --m_x, --m_y, --m_z;
                    return *this;
            }
            Point &operator++(){//prefix++
                    ++m_x, ++m_y, ++m_z;
                    return *this;
            } 
            Point operator--(int){ //postfix --, requires dummy parameter without any name & is not used inside the function
                    Point ret {m_x, m_y, m_z};//bcz of temp var usage, postfix operator are slow
                    //--m_x, --m_y, --m_z;
                    //reuse code
                    --(*this);
                    return ret;
            }
            Point operator++(int){ //postfix ++ ,requires dummy parameterle
                    Point ret {m_x, m_y, m_z};
                    //++m_x, ++m_y, ++m_z;
                    ++(*this);
                    return ret;
            }
        };
        //Note that we’re returning a copy by value rather than a const reference here. This is because users of this function will probably expect the returned object to be modifiable.

/* overlaoding [] 
            - function parameter need not be int, it can be string , double etc
            - prefered implementation is using member function
        */
        #include<cassert>
        class IntList
        {
        private:
            int m_list[10]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; // give this class some initial state for this example
        public:
        //note parameter can be int, double, string etc
            int & operator[](int index){//non const obj
                assert(index <10 && index >=0);
                return m_list[index];
            }
            const int & operator[](int index) const{//for const objt
                assert(index <10 && index >=0);
                return m_list[index];
            }
        };
/*      //Pointers to objects and overloaded operator[] don’t mix
        Because we can’t assign an integer to an IntList, this won’t compile. However, if assigning an integer was valid, this would compile and run, with undefined results.

... Rule
        Make sure you’re not trying to call an overloaded operator[] on a pointer to an object.
 */
        int main3()
        {
            IntList *list{ new IntList{} };
            //list [2] = 3; // error: this will assume we're accessing index 2 of an array of IntLists
            (*list)[2]= 3;//valid , [] has higher precedence than *
            delete list;      
            return 0;
        }


/* // overloading () operator --> must be implemented as member functions
                            --> number & type of para,eter can vary only in ()
                            --> not preferred
  */

        #include <cassert> // for assert()
        class Matrix
        {
        private:
            double m_data[4][4]{};
        
        public:
            double& operator()(int row, int col);
            double operator()(int row, int col) const;
            void operator()();
        };
        
        double& Matrix::operator()(int row, int col)
        {
            assert(col >= 0 && col < 4);
            assert(row >= 0 && row < 4);
        
            return m_data[row][col];
        }
        
        double Matrix::operator()(int row, int col) const
        {
            assert(col >= 0 && col < 4);
            assert(row >= 0 && row < 4);
        
            return m_data[row][col];
        }
        
        void Matrix::operator()()
        {
            // reset all elements of the matrix to 0.0
            for (int row{ 0 }; row < 4; ++row)
            {
                for (int col{ 0 }; col < 4; ++col)
                {
                    m_data[row][col] = 0.0;
                }
            }
        }

        int main4()
        {
            Matrix matrix{};
            matrix(1, 2) = 4.5;
            matrix(); // erase matrix, not preferred way, provide erase member function instead
            std::cout << matrix(1, 2) << '\n';        
            return 0;
        }

 /*  ... functor // operator() is used to implement functors or function object, class that operate like function
    // they can store data in member vriable .
    You may wonder why we couldn’t do the same thing with a normal function and a static local variable to preserve data between function calls. We could, but because functions only have one global instance, we’d be limited to using it for one thing at a time. With functors, we can instantiate as many separate functor objects as we need and use them all simultaneously. */
            class Accumulator
            {
            private:
                int m_counter{ 0 };
            
            public:
                int operator() (int i) { return (m_counter += i); }
            };
            
            int main5()
            {
                Accumulator acc{};
                std::cout << acc(10) << '\n'; // prints 10
                std::cout << acc(20) << '\n'; // prints 30
            
                return 0;
            }


/* ... overloading typecast operator
There are three things to note:
    1)To overload the function that casts our class to an int, we write a new function in our class called operator int(). Note that there is a space between the word operator and the type we are casting to.
    2)User-defined conversions do not take parameters, as there is no way to pass arguments to them.
    3)User-defined conversions do not have a return type. C++ assumes you will be returning the correct type. */

        class Cents3
        {
        private:
            int m_cents;
        public:
            Cents3(int cents=0)
                : m_cents{ cents }
            {
            }
            operator int() const {  //note no parameter, no return type as it is obvious, space between operator & int
                return m_cents;
            }
        };
        void printInt(int value)
        {
            std::cout << value;
        }
        int main7()
        {
            Cents3 cents{ 7 };
            printInt(cents); // print 7
            std::cout << '\n';        
            return 0;
        }
/* The compiler will first note that function printInt takes an integer parameter. Then it will note that variable cents is not an int. Finally, it will look to see if we’ve provided a way to convert a Cents into an int. Since we have, it will call our operator int() function, which returns an int, and the returned int will be passed to printInt().
 .....> We can now also explicitly cast our Cents variable to an int: */
        Cents3 cents{ 7 };
        int c{ static_cast<int>(cents) };

//copy constructor  
        // default copy constructor: memberwise initialization
        // copy can be prevented by making it private, but still can be used by member function, better use delete
    /* ...  elision   
        Note that initializing an anonymous object and then using that object to direct initialize our defined object takes two steps (one to create the anonymous object, one to call the copy constructor). However, the end result is essentially identical to just doing a direct initialization, which only takes one step.
        For this reason, in such cases, the compiler is allowed to opt out of calling the copy constructor and just do a direct initialization instead.. */
        /*        int main()
                {
                    Fraction six = Fraction(6);
                    //This form of copy initialization is evaluated the same way as the following:
                    Fraction six(Fraction(6));


                    Fraction fiveThirds(Fraction(5, 3));--> elided by compiler as following, copy const is not called 
                    Fraction fiveThirds(5, 3)
                    std::cout << six;

                    return 0;
                } 
        e.g 2>>
                class Something
                {  };                
                Something foo() 
                {
                    Something s;
                    return s;
                }                
                int main()
                {
                    Something s = foo();
                }
        In this case, the compiler will probably elide the copy constructor, even though variable s is returned by value.
        */

//Rule: Avoid using copy initialization, and use uniform initialization instead.
/* Other places copy initialization is used
        --> pass by or return by value from function
 */

//..... Converting constructors,
      /*   By default, C++ will treat any constructor as an implicit conversion operator
        This implicit conversion works for all kinds of initialization (direct, uniform, and copy).
        Constructors eligible to be used for implicit conversions are called converting constructors (or conversion constructors).
        constructors taking multiple parameters can now be converting constructors. */

//..... explicit
         /* construction of object using converting construtor which sometimes lead to undesireed results.
         explicit keyword is used to avoid implicit 
          explicit will not be used for implicit conversions or copy initialization.
           note-->that making a constructor explicit only prevents implicit conversions , explicit are still allowed
            e.g */
            #include <string>
            #include <iostream>
            
            class MyString
            {
            private:
                std::string m_string;
            public:
                MyString(int x) // allocate string of size x
                {
                    m_string.resize(x);
                }            
                MyString(const char *string) // allocate string to hold string value
                {
                    m_string = string;
                }            
                friend std::ostream& operator<<(std::ostream& out, const MyString &s);
            };            
            std::ostream& operator<<(std::ostream& out, const MyString &s)
            {
                out << s.m_string;
                return out;
            }            
            void printString1(const MyString &s)
            {
                std::cout << s;
            }
            
            int main8()
            {
                MyString mine = 'x'; // Will compile and use MyString(int)
                std::cout << mine << '\n';
            
                printString1('x'); // Will compile and use MyString(int)

                return 0;
            }
        //Explicit conversions (via casting) are still allowed:
           // std::cout << static_cast<MyString>(5); // Allowed: explicit cast of 5 to MyString(int)
      /*      Direct or uniform initialization will also still convert parameters to match (uniform initialization will not do      narrowing conversions, but it will happily do other types of conversions).
            MyString str{'x'}; // Allowed: initialization parameters may still be implicitly converted to match
        */
            class MyString2
            {
            private:
                std::string m_string;
            public:
                    // explicit keyword makes this constructor ineligible for implicit conversions
                explicit MyString2(int x) // allocate string of size x
                {
                    m_string.resize(x);
                }
            
                MyString2(const char *string) // allocate string to hold string value
                {
                    m_string = string;
                }            
                friend std::ostream& operator<<(std::ostream& out, const MyString2 &s);
            };            
            std::ostream& operator<<(std::ostream& out, const MyString2 &s)
            {
                out << s.m_string;
                return out;
            }            
            void printString(const MyString2 &s)
            {
                std::cout << s;
            }            
            int main6()
            {
                /* MyString2 mine = 'x'; // compile error, since MyString(int) is now explicit and nothing will match this
                std::cout << mine; */
            
                //printString('x'); // compile error, since MyString(int) can't be used for implicit conversions
                std::cout << static_cast<MyString2>(5); // Allowed: explicit cast of 5 to MyString(int)
                MyString2 str{'x'}; // Allowed: initialization parameters may still be implicitly converted to match
                return 0;
            }

/*... Rule: Consider making your constructors and user-defined conversion member functions explicit to prevent implicit conversion errors */


//.....  delete 
/*     When a function has been deleted, any use of that function is considered a compile error.
    copy constructor and overloaded operators may also be deleted in order to prevent those functions from being used.*/
        class MyString3
        {
        private:
            std::string m_string;
        
               /*  MyString3(char) // objects of type MyString(char) can't be constructed from outside the class
                {
                } */
        public:
            MyString3(char) = delete;// any use of this constructor is an error
                // explicit keyword makes this constructor ineligible for implicit conversions
            explicit MyString3(int x) // allocate string of size x /
            {
                m_string.resize(x);
            }
        
            MyString3(const char *string) // allocate string to hold string value
            {
                m_string = string;
            }
        
            friend std::ostream& operator<<(std::ostream& out, const MyString3 &s);
        
        };
        
        std::ostream& operator<<(std::ostream& out, const MyString3 &s)
        {
            out << s.m_string;
            return out;
        }
        
        int main9()
        {
            // MyString3 mine('x'); // compile error, since MyString(char) is private
            // MyString mine('x'); // compile error, since MyString(char) is deleted
            //std::cout << mine;
            return 0;
        }
//.....Overloading the assignment operator
    /* 
        The assignment operator must be overloaded as a member function.

        // A simplistic implementation of operator= 
            Fraction& Fraction::operator= (const Fraction &fraction)
            {
                // do the copy
                m_numerator = fraction.m_numerator;
                m_denominator = fraction.m_denominator;
            
                // return the existing object so we can chain this operator
                return *this;
            } */

    // Issues due to self-assignment
/*
                MyString& MyString::operator= (const MyString& str)
                {
                    // self-assignment check
                    if (this == &str)
                        return *this;
                
                    // if data exists in the current string, delete it
                    if (m_data) delete[] m_data;
                
                    m_length = str.m_length;
                
                    // copy the data from str to the implicit object
                    m_data = new char[str.m_length];
                
                    for (int i = 0; i < str.m_length; ++i)
                        m_data[i] = str.m_data[i];
                
                    // return the existing object so we can chain this operator
                    return *this;
                }

                no need to check for self-assignment in a copy-constructor. This is because the copy constructor is only called when new objects are being constructed, 

                , the self-assignment check may be omitted in classes that can naturally handle self-assignment. Consider this Fraction class assignment operator that has a self-assignment guard:
                    // A better implementation of operator=
                    Fraction& Fraction::operator= (const Fraction &fraction)
                    {
                        // self-assignment guard , doesnt impact but still good practice
                        if (this == &fraction)
                            return *this;
                    
                        // do the copy
                        m_numerator = fraction.m_numerator; // can handle self-assignment
                        m_denominator = fraction.m_denominator; // can handle self-assignment
                    
                        // return the existing object so we can chain this operator
                        return *this;
                    }
                */
//Default assignment operator -->shallow copy .. memberwise assignment
        //can be disabled by making it private or by using delete keyword
            // Overloaded assignment
	        //Fraction& operator= (const Fraction &fraction) = delete; // no copies through assignment!  

// deepcopy -->Doing deep copies requires that we write our own copy constructors and overloaded assignment operators.
        /* 
        // assumes m_data is initialized
            void MyString::deepCopy(const MyString& source)
            {
                // first we need to deallocate any value that this string is holding!
                delete[] m_data;
            
                // because m_length is not a pointer, we can shallow copy it
                m_length = source.m_length;
            
                // m_data is a pointer, so we need to deep copy it if it is non-null
                if (source.m_data)
                {
                    // allocate memory for our copy
                    m_data = new char[m_length];
            
                    // do the copy
                    for (int i{ 0 }; i < m_length; ++i)
                        m_data[i] = source.m_data[i];
                }
                else
                    m_data = nullptr;
            }
            
            // Copy constructor
            MyString::MyString(const MyString& source)
            {
                deepCopy(source);
            }
            Mystring & MyString::operator=(const & MyString source){
                 // check for self-assignment
                if(this != &source){
                    deepCopy(source);
                }
                return *this;
            }
            Our assignment operator is very similar to our copy constructor, but there are three major differences:

               1) We added a self-assignment check.
               2) We return *this so we can chain the assignment operator.
               3)We need to explicitly deallocate any value that the string is already holding (so we don’t have a memory leak when m_data is reallocated later).
        */

/* .....Summary
        > The default copy constructor and default assignment operators do shallow copies, which is fine for classes that contain no dynamically allocated variables.
        > Classes with dynamically allocated variables need to have a copy constructor and assignment operator that do a deep copy.
        > Favor using classes in the standard library over doing your own memory management. 
        
        >> If you’re overloading assignment (=), subscript ([]), function call (()), or member selection (->), do so as a member function.
        >> If you’re overloading a unary operator, do so as a member function.
        >> If you’re overloading a binary operator that modifies its left operand (e.g. operator+=), do so as a member function if you can.
        >> If you’re overloading a binary operator that does not modify its left operand (e.g. operator+), do so as a normal function or friend function. 
        
        
        */