#ifndef __TEMPLATEARRAY_H
#define __TEMPLATEARRAY_H

    #include<initializer_list>
    #include<cassert>
    template<typename T>
    class Array{
        T *m_data{nullptr};
        int m_len{};

    public:
        Array() = default;
        Array(int len );
        Array(std::initializer_list<T> list);
        //delete copy constructor, & assignement operator to avoid shallow copy
        Array( const Array & src) = delete;
        Array & operator=(const Array & src) = delete;
        Array & operator=(std::initializer_list<T> list);
        T & operator[](int ix);
        void erase();
        //delete old array & create new array  with new size
        void reallocate(int len);    
        // resize resizes the array.  Any existing elements will be kept.  This function operates slowly.
        void resize(int newLen);
        void remove(int idx);
        void insertBefore( const T & val ,int idx);
        void insertAtBeginning(const T &val);
        void insertAtEnd(const T & val);
        int getLength(){ return m_len;}
        ~Array(){ delete [] m_data;}
        void print();
    };
#endif