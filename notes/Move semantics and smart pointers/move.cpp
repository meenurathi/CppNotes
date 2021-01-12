// Move construtor & move assignment


/* 
    # issue with auto ptr with using copy constr & copy assignment as move
        > it changes l-value passed as arg, which cause issues when re used some where else in the program
        > it inst intutive
        > doesnt work well with container classes
    # when move constructor & move assignment operator is called
        >  when these functions have been defined, 
           and the argument for construction or assignment is an r-value.
           Most typically, this r-value will be a literal or temporary value.
    #default move constructor & move assignment operator     
    #move ctr & mv assignment is not provided by default unless
         class doesn`t have any defined copy ctr, copy assignment, move assignment or destructor
    # std::move() 
        > if you want to call move ctr or assgnmnt optr on lvalue, use std::move() function to convert it in rvalue
        >std::move can be used whenever we want to treat an l-value like an r-value for the purpose of invoking move semantics instead of copy semantics.
        > defined in header <utility>
        >used inesp in swap functions, filling elemnts in container such as vector with lvalue (vec.push_back(std::move(str));), sorting element in array

    Rule: always write move ctr & move assignment operatot explicitly when intended
    Rule: move function should leave both objects in a well defined state ideally "null state" or uninitialzed state

 */
#if 0
    #include<iostream>
    #include<string>
    #include<utility>
    class Resource{
        std::string m_name{};
        public:
            Resource()=default;
            Resource(std::string name): m_name(name){
                    std::cout<<"Resource acquired :"<<m_name<<"\n";
            }
            void setName(std::string name) { m_name = name;}
            const std:: string getName(){ return m_name;}
            ~Resource(){
                    std::cout<<"Resource destroyed : "<<m_name<<"\n";
            }
    };
    template<class T>
    class Autoptr{
        T * m_ptr{nullptr};
        int m_size{};
    public:
        Autoptr(T * ptr = nullptr, int size=0) 
            :m_size{size}{
                m_ptr = new T[size]{};
                std::cout<<"default ctr"<<size<<"\n";
        }
        ~Autoptr(){ delete[] m_ptr;}
        //copy constructor for deep copying
        Autoptr(const Autoptr& src);// this can be deleted, to ensures copy are not made
        //copy assignment
        Autoptr & operator=(const Autoptr & src); // this can be deleted,  to ensures copy are not made
        //move construtor
        Autoptr(Autoptr&& src) noexcept;
        // move assignment
        Autoptr& operator=(Autoptr && src);

        T& operator*(){ return *m_ptr;}
        T* operator->(){ return m_ptr;}
        T& operator[](int idx){ return m_ptr[idx];}
        const T& operator[](int idx) const { return m_ptr[idx];}
        bool isNull()const { return m_ptr == nullptr;}
        int getSize()const { return m_size;}
    };
     //copy constructor for deep copying
    template<typename T>
    Autoptr<T>:: Autoptr(const Autoptr<T>& src): m_size(src.m_size){
            std::cout<<"copy ctr\n";
            if(m_size >0){
                m_ptr = new T[m_size]{};

                for(auto i{0} ;i < m_size; i++){
                    m_ptr[i] = src.m_ptr[i];
                }
            }else{
                m_ptr = nullptr;
            }
    }
    //copy assignment operator
    template<typename T>
    Autoptr<T> & Autoptr<T>::operator=(const Autoptr<T> & src){
            std::cout<<"copy assignment\n";
            if(this == &src)
                return *this;
            delete[] m_ptr;

            m_size = src.m_size;
            m_ptr = new T[m_size]{};

            for(auto i {0}; i < m_size; i++){
                m_ptr[i] = src.m_ptr[i];
            }
            return *this;
    }
    //move constructor
    template<typename T>
    Autoptr<T>::Autoptr(Autoptr<T>&& src) noexcept
        : m_ptr(src.m_ptr), m_size(src.m_size){
        std::cout<<"move ctr \n";
        src.m_size =0;
        src.m_ptr = nullptr;
    }
    template<typename T>
    Autoptr<T>& Autoptr<T>::operator=(Autoptr<T> && src){
        std::cout<<"move ctr \n";
        if(this == &src)
            return *this;
        //release current holding content
        delete[] m_ptr;
        //steal resources
        m_ptr = src.m_ptr;
        m_size = src.m_size;
        src.m_ptr = nullptr;
        src.m_size = 0;
        return *this;
    }
    template<typename T, unsigned int size>
    Autoptr<T> generateResource(){
        Autoptr<T>  ptr {Autoptr<T>{new T[size]{}, size}};
        return ptr;
    }
    //swap
    template<typename T>
    void mySwap(T& a, T& b){
        T temp {std::move(a)};
        a = std::move(b);
        b = std::move(temp);
    }
    int main(){
        Autoptr<Resource> res1;
        std::cout<<"main 1\n";
        res1 = generateResource<Resource, 1>();
        res1[0].setName("R1");
        std::cout<<"res1 name-"<<res1[0].getName()<<"\n";
        Autoptr<Resource> res2 = generateResource<Resource, 1>();
        res2[0].setName("R2");
        std::cout<<"res2 name-"<<res2[0].getName()<<"\n";
        mySwap<Autoptr<Resource>>(res1, res2);
        std::cout<<"after swap \nres1 name-"<<res1[0].getName()<<"\n";
        std::cout<<"res2 name-"<<res2[0].getName()<<"\n";
        Autoptr<int> intres = generateResource<int, 100>();
        return 0;
    }
#endif

/*     
    # noexcept exception specifier -  strong exception guarantee
        which guarantees that if a function is interrupted by an exception,
            no memory will be leaked
            and the program state will not be changed 
        
        > copy ctr: if copy fails, src variable is not harmed or alter. 
                    we can discard the failed copy & move on
                    //strong exception guarantee is upheld
        > move ctr: if tranfer of ownership fail, src var will be left in modified state
                     if src was temp var, then no issue as it will be discarded
                     but, if it was called for non-temp var, then that var is in bad state.
                     Therefore, no strong exception guarantee is provided by mv ctr.
                     To ensure this guarentee, we need to move resource back to the source, but if the first move failed, there`s no guarantee the move back will succeed either.
     */
    /*                
        How can we give move constructors the strong exception guarantee? 
        > followingis an example showing no exception guarentee by mv ctr
            // move contructor of std::pair class
            template<typename T1, typename T2>
            pair<T1, T2>::pair(pair &&old)
                : first(std::move(old.first)),
                  second(std::move(old.second))
            {}
        */
        #if 1
        #include<iostream>
        #include<stdexcept>// For std::pair, std::make_pair, std::move, std::move_if_noexcept
        #include<utility>//// std::runtime_error
        class MoveClass{
            int *m_resource{};
        public:
            MoveClass() = default;
            MoveClass(int  resource)
                : m_resource{ new int{resource}}
            {
                  std::cout <<"MoveClass ctr :"<<*this<<'\n';
            }
            //copy ctr
            MoveClass(const MoveClass& src){
                  //deep copy
                if(src.m_resource != nullptr){
                    m_resource = new int{*src.m_resource};
                }
                std::cout <<"MoveClass copy ctr :"<<*this<<'\n';
            }
            //move ctr
            MoveClass(MoveClass && src) noexcept
                :m_resource{src.m_resource}
            {
                std::cout <<"MoveClass mvctr :"<<*this<<'\n';
                src.m_resource = nullptr;
            }
            ~MoveClass(){
                std::cout << "destroying " << *this << '\n';
                delete m_resource;
            }
            friend std::ostream & operator<<(std::ostream & out, const MoveClass & mvclass){
                out<<"Moveclass(";
                if(mvclass.m_resource == nullptr){
                    out<<"empty";
                }else{
                    out<<*mvclass.m_resource;
                }
                out<<")";
                return out;
            }
        };
        class CopyClass{
        public:
            bool m_throw{};
            CopyClass()= default;
            // Copy constructor throws an exception when copying from a CopyClass object where its m_throw is 'true'
            CopyClass(const CopyClass& src)
                :m_throw{src.m_throw}
            {
                if(m_throw){
                    throw::std::runtime_error{"abort!"};
                }
            }
        };
        int main1(){
            // We can make a std::pair without any problems:
            std::pair my_pair{MoveClass{13}, CopyClass{}};
            std::cout << "my_pair.first: " << my_pair.first << '\n';
            try{
                // To trigger copy constructor exception
                my_pair.second.m_throw = true;
                  // The following line will throw an exception
                std::pair moved_pair{std::move(my_pair)};
                std::cout << "moved pair exists\n"; // Never prints
            }
            catch(const std::exception& ex){
                 std::cout << "Error found: "<<ex.what()<<'\n';
            }
            std::cout << "my_pair.first: " << my_pair.first << '\n';
            return 0;
        }
/* 
    output : using std::pair moved_pair{std::move(my_pair)};
        MoveClass ctr :Moveclass(13)
        MoveClass mvctr :Moveclass(13)
        destroying Moveclass(empty)
        my_pair.first: Moveclass(13)
        MoveClass mvctr :Moveclass(13)
        destroying Moveclass(13)
        Error found: abort!
        my_pair.first: Moveclass(empty)
        destroying Moveclass(empty)
 */
/* 
        throwing copy constructor of CopyClass. This copy constructor throw an exception, causing the creation of moved_pair to abort, and my_pair.first to be permanently damaged. The strong exception guarantee was not preserved.
 */
        //above problem can be avoided if we always call copy ctr (which doesnt afect src obj) instead of move ctr, but it has perf penalty
        //move_no_except to the resque for src obj, ensuring no exception guarantee
        
        /* 
        solution:
            1)use no except move ctr:
                 noexcept functions are no-throw/no-fail, they implicitly meet the criteria of strong exception guarantee. Thus, a no except move constructor is guarantee to succeed
            2) use std::move_if_noexcept() 
                this determine whether a move or a copy should be performed
                     It returns converted movable r-value,if compiler can tell the object has a noexcept move constructor,
                     otherwise it will return a copyable l-value ref to objt

            We can use the noexcept specifier in conjunction with std::move_if_noexcept to use move semantics only when a strong exception guarantee exists (and use copy semantics otherwise).
        
        
         */
         int main(){
            // We can make a std::pair without any problems:
            std::pair my_pair{MoveClass{13}, CopyClass{}};
            std::cout << "my_pair.first: " << my_pair.first << '\n';
            try{
            // To trigger copy constructor exception
                my_pair.second.m_throw = true;
            // The following line will throw an exception but src obj my_pair is safe
                std::pair moved_pair{std::move_if_noexcept(my_pair)};
                std::cout << "moved pair exists\n"; // Never prints
            }
            catch(const std::exception& ex){
                 std::cout << "Error found: "<<ex.what()<<'\n';
            }
            std::cout << "my_pair.first: " << my_pair.first << '\n';
            return 0;
        }

        /* 
            MoveClass ctr :Moveclass(13)
            MoveClass mvctr :Moveclass(13)
            destroying Moveclass(empty)
            my_pair.first: Moveclass(13)
            MoveClass copy ctr :Moveclass(13)
            destroying Moveclass(13)
            Error found: abort!
            my_pair.first: Moveclass(13)
            destroying Moveclass(13)

            //after the exception was thrown, the subobject my_pair.first still points to the value 13.
            CopyClass doesn’t have a noexcept move constructor, so std::move_if_noexcept returns my_pair as an l-value reference. This causes moved_pair to be created via the copy constructor (rather than the move constructor). The copy constructor can throw safely, because it doesn’t modify the source object.
         */
        #endif

    /* 
    The standard library uses std::move_if_noexcept often to optimize for functions that are noexcept. 
        For example, std::vector::resize will use move semantics if the element type has a noexcept move constructor, and copy semantics otherwise. 
        This means std::vector will generally operate faster with objects that have a noexcept move constructor 
        (reminder: move constructors are noexcept by default, unless they call a function that is noexcept(false)).

Warning

    If a type has both potentially throwing move semantics and deleted copy semantics (the copy constructor and copy assignment operator are unavailable),
    > then std::move_if_noexcept will waive the strong guarantee and invoke move semantics. 
    >This conditional waiving of the strong guarantee is ubiquitous in the standard library container classes, since they use std::move_if_noexcept often.


    */