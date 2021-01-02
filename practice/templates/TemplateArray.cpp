#include"TemplateArray.h"
#include<iostream>
    template<typename T>
    Array<T>::Array(int len ):m_len{len} {
        assert(len > 0);
        m_data = new T[m_len]{};
    }

    template<typename T>
    Array<T>::Array(std::initializer_list<T> list): 
     //no need for passing list as const reference just like string_view, as it is very light weighted
            // copies tend to be cheaper then indirection
        Array( static_cast<int>(list.size()))   //constructor delegation
     {
        assert(m_len > 0);
        m_data = new T[m_len]{};
        int idx{};
        //initializer list doesnt supports [] 
        for(auto & val: list)
            m_data[idx++] = val;
    }


    template<typename T>
   Array<T> & Array<T>::operator=(std::initializer_list<T> list){
        int size { static_cast<int>(list.size())};
        reallocate(size);
        int idx{};
        for(auto & ele : list){
            m_data[idx++] = ele;
        }
        return *this;
    }

    template<typename T>
    T& Array<T>::operator[](int idx){
        assert(idx >= 0 && idx <m_len);
        return m_data[idx];
    }

    template<typename T>
    void  Array<T>::erase(){
        delete[] m_data;
        m_data = nullptr;
        m_len = 0;
    }

    //delete old array & create new array  with new size
    template<typename T>
    void  Array<T>:: reallocate(int len){
        if(len == m_len)
            return;
        erase();
        if(len <=0)
            return;
        m_data = new T[len]{};
        m_len = len;
    }
    
    // resize resizes the array.  Any existing elements will be kept.  This function operates slowly.
    template<typename T>
    void  Array<T>:: resize(int newLen){
        if(newLen == m_len)
        return;
        if(newLen <= 0 ){
            erase();
            return;
        }
        
        T * newData = new T[newLen]{};
        if(m_len >0){
            int elementToCopy{ (newLen < m_len)? newLen: m_len};
            for(int i {}; i < elementToCopy; i++)
                newData[i] = m_data[i];
        }
        delete[] m_data;
        m_data = newData;
        m_len = newLen;

    }

    template<typename T>
    void  Array<T>:: remove(int idx){
        assert(idx >= 0 && idx < m_len);

        if(m_len == 1)
        {
            erase();
            return;
        }
        T * newData{new T[m_len -1]{}};
        for (auto before{0}; before< idx; before++){
            newData[before] = m_data[before];
        }
        for(auto after{idx + 1}; after< m_len; after ++){
            newData[after -1] = m_data[after];
        }
        delete[] m_data;
        m_data = newData;
        m_len--;
    }

    template<typename T>
    void  Array<T>::insertBefore(const T & val ,int idx){
        assert(idx >= 0 && idx <= m_len);
        
        T * newData = new T[m_len +1];
        for(auto before{0}; before < idx ; before++){
            newData[before] = m_data[before];
        }
        newData[idx] =val;
        for(auto after{idx+1} ; after <= m_len; after++){
            newData[after] = m_data[after-1];
        }
        delete[] m_data;
        m_data = newData;
        m_len++;
    }

    template<typename T>
    void  Array<T>::insertAtBeginning(const T &val){
        insertBefore(val, 0);
    }

   template<typename T>
    void  Array<T>::insertAtEnd(const T & val){
        insertBefore(val, m_len);
    }

    template< typename T>
    void Array<T>::print(){
        for (int i{ 0 }; i<m_len; ++i)
                std::cout << m_data[i] << ' ';
        std::cout << "\n";
    }