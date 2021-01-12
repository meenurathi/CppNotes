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
        > move ctr: if tranfer of ownership fail, src var will beleft in modified state
                     if src was temp var, then no issue
                     but, if it was called for non-temp var, then that var is in bad state.
                     Therefore, no strong exception guarantee is provided by mv ctr

        */