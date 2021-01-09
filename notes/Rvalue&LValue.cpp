

/* 
    Lvalue --> locator value
        > has memory address
        > suitable to be on the left hand side of an assignment expression 
        > is an object or a function ( or expresssion taht evaluate as object or function
        Types:
            Modifiable L value --> non const
            Non Modifible L value --> const
    R value
            > doesnt have memory address
            > is not suitable to be on the left Handside of an assignment expression
            > Are evaluated for their value
            > has expression scope
            > it includes
                >> literal (eg. 5, "fff")
                >> temporary values( e.g x+5)
                >> anonymous object (e.g Fraction(5,2) )

    In order to support move semantics, C++11 introduces 3 new value categories: pr-values, x-values, and gl-values.
     If you’re interested, cppreference.com has an extensive list of expressions that qualify for each of the various value categories, as well as more detail about them.
 */

/* 
    Refernce types
        l-value reference - can only be initialized by modifiable lvalue
                            L-value reference	            Can be initialized with	    Can modify
                            Modifiable l-values                 	Yes                     Yes
                            Non-modifiable l-values             	No                     	No
                            R-values	                            No                  	No

        l-value referenceto const : can be initialized with lvalue, const lvalue & rvalue , butis non modifiable
                            L-value reference to const  	Can be initialized with	    Can modify
                            Modifiable l-values	                    Yes	                    No
                            Non-modifiable l-values	                Yes	                    No
                            R-values	                            Yes	                    No
            Note: L-value references to const objects are particularly useful 
            because they allow us to pass any type of argument (l-value or r-value) into a function without making a copy of the argument.

        r-value reference - can be initialized only with  r value not lvalue
                            created using a double ampersand:

                            int x{ 5 };
                            int &lref{ x }; // l-value reference initialized with l-value x
                            int &&rref{ 5 }; // r-value reference initialized with r-value 5
                            
                            R-value reference	            Can be initialized with	    Can modify
                            Modifiable l-values                 	No                      No
                            Non-modifiable l-values             	No	                    No
                            R-values	                            Yes                 	Yes

                            R-value reference to const	    Can be initialized with	    Can modify
                            Modifiable l-values	                    No	                    No
                            Non-modifiable l-values             	No	                    No
                            R-values	                            Yes	                    No


                            They have two properties that are useful. 
                                First, r-value references extend the lifespan of the object they are initialized with to the lifespan of the r-value reference (l-value references to const objects can do this too).*/
                               #if 0
                                #include <iostream>
                                class Fraction
                                {
                                private:
                                    int m_numerator;
                                    int m_denominator;
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
                            /* 
                                As an anonymous object, Fraction(3, 5) would normally go out of scope at the end of the expression in which it is defined. However, since we’re initializing an r-value reference with it, its duration is extended until the end of the block. We can then use that r-value reference to print the Fraction’s value.
                            */
                                int main()
                                {
                                    auto &&rref{ Fraction{ 3, 5 } }; // r-value reference to temporary Fraction
                                    // f1 of operator<< binds to the temporary, no copies are created.
                                    /std::cout << rref << '\n';/3/5
                                    return 0;
                                } // rref (and the temporary Fraction) goes out of scope here
                            #endif

  /*                        Second, non-const r-value references allow you to modify the r-value!
                            when initializing an r-value with a literal, a temporary is constructed from the literal so 
                            that the reference is referencing a temporary object, not a literal value. */
                            #if 0
                                #include <iostream>
                                
                                int main()
                                {
                                    int &&rref{ 5 }; // because we're initializing an r-value reference with a literal, a temporary with value 5 is created here
                                    rref = 10;
                                    std::cout << rref << '\n';
                                
                                    return 0;
                                }
                            #endif

                            /* 
                            R-value references as function parameters
                            This is most useful for function overloads when you want to have different behavior for l-value and r-value arguments.
                              */
                            #if 0
                                void fun(const int &lref) // l-value arguments will select this function
                                {
                                    std::cout << "l-value reference to const\n";
                                }
                                
                                void fun(int &&rref) // r-value arguments will select this function
                                {
                                    std::cout << "r-value reference\n";
                                }
                                
                                int main()
                                {
                                    int x{ 5 };
                                    fun(x); // l-value argument calls l-value version of function
                                    fun(5); // r-value argument calls r-value version of function
                                
                                    return 0;
                                }

                                /* 
                                    l-value reference to const
                                    r-value reference
                                 */
                            #endif


                            /* 
                            
                            Imp Note
                            
                                int &&ref{ 5 };
	                            fun(ref);
                                It calls the l-value version of the function! 
                                Although variable ref has type r-value reference to an integer, 
                                it is actually an l-value itself (as are all named variables).
     */
    #if 0
            // State which of the following lettered statements will not compile:
                    int main()
                {
                    int x{};
                
                    // l-value references
                    int &ref1{ x }; // A
                    int &ref2{ 5 }; // B 
                
                    const int &ref3{ x }; // C
                    const int &ref4{ 5 }; // D
                
                    // r-value references
                    int &&ref5{ x }; // E
                    int &&ref6{ 5 }; // F
                
                    const int &&ref7{ x }; // G
                    const int &&ref8{ 5 }; // H
                    
                    return 0;
                }
                //ans: B, E, G
    #endif