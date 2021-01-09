#include<iostream>
#include<exception>
#include<string>
#include<string_view>
class ArrayException : public std::exception{
    std::string m_error{};
public: 
    ArrayException(std::string_view error)
    :m_error{error}{}
    const char *what() const noexcept override{
        return m_error.c_str();
    }
};
class ArrayInt{
    int m_data[3]{};
    int m_size{};
public:
    ArrayInt():m_size(3){
    };
    int getLength(){ return 3;}
    int & operator[](int idx){
        if(idx < 0 || idx >m_size){
           // throw std::length_error("Invalid array index access");
           throw ArrayException{"Invalid index"};
        }
        return m_data[idx];
    }
    ~ArrayInt(){ }  
};

int main(){
    ArrayInt array;
    try{
        int value{array[5]};
    }
    catch(const ArrayException & exception){
        std::cerr<<"An array exception occured ("<<exception.what()<<")\n";
    }
    catch(const std::exception & exception){
        std::cerr<<"Some other exception ocuured ("<<exception.what()<<")\n";
    }
    return 0;
}