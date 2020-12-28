//Initializer list
/* >>> header <initialization_list>
>>> int arr[]{5,6,8,9};//initializer list
>>> auto *array{ new int[5]{ 5, 4, 3, 2, 1 } }; // initializer list
>>> // What happens if we try to use an initializer list with this container class?
	IntArray array { 5, 4, 3, 2, 1 }; // this line doesn't compile

...... When a compiler sees an initializer list, it automatically converts it into an object of type std::initializer_list. Therefore, if we create a constructor that takes a std::initializer_list parameter, we can create objects using the initializer list as an input.
....... you have to tell std::initializer_list what type of data the list holds using angled brackets, unless you initialize the std::initializer_list right away.
        std::initializer_list<int> or std::initializer_list<std::string>.
....... std::initializer_list has a (misnamed) size() function which returns the number of elements in the list. 
        list.size() returns a size_t (which is unsigned) 
....... we don’t pass the list by const reference. Much like std::string_view, std::initializer_list is very lightweight and copies tend to be cheaper than an indirection.
........ brace initialization prefers list constructors.
........ std::initializer_list does not provide access to the elements of the list via subscripting (operator[]). 
        use for-each loop instead.
            The for-each loops steps through each element of the initialization list, and we can manually copy the elements into our internal array.

....imp...  Initializer lists will always favor a matching initializer_list constructor over other 
                potentially matching constructors. Thus, this variable definition:
                    IntArray array { 5 };
                would match to IntArray(std::initializer_list<int>), not IntArray(int).
                If you want to match to IntArray(int) once a list constructor has been defined, you’ll need to use copy initialization or direct initialization.
                The same happens to std::vector and other container classes that have both a list constructor and a constructor with a similar type of parameter

                    std::vector<int> array(5); // Calls std::vector::vector(std::vector::size_type), 5 value-initialized elements: 0 0 0 0 0
                    std::vector<int> array{ 5 }; // Calls std::vector::vector(std::initializer_list<int>), 1 element: 5
...... use std::initializer_list to assign new values to a class by overloading the assignment operator to take a std::initializer_list parameter. example below

>>> Rule : If you provide list construction, it’s a good idea to provide list assignment as well.

 */// implementation of array container having constructor with initialization list
#include<iostream>
#include<cassert>
#include<initializer_list>
class IntArray{
    int *m_data{};
    int m_size{};
public:
    IntArray()= default;
    IntArray(int len):m_size(len){
        assert(len >0);
        m_data = new int[m_size]{};
    }
  
    IntArray( std::initializer_list<int> iList):
            //no need for passing list as const reference just like string_view, as it is very light weighted
            // copies tend to be cheaper then indirection
            IntArray(static_cast<int>(iList.size())){  //use constructor delegation to init
            //List.size() return size_t which is unsigned
            // 2nd const is called using ()direct initialization instead of {} brace initialization
            // because brace initialization prefers list constructor
        int pos{};
        //initializer list doesnt supports [] 
        for(const auto &l : iList){
            m_data[pos++] = l;
        }
    }
    IntArray(const IntArray & src) = delete;//to avoid shallow copy
    IntArray & operator=(const IntArray & src) = delete;//to avoid shallow assigment
    int & operator[](int index){
        assert(index >= 0 && index < m_size);
        return m_data[index];      
    }
    IntArray & operator=(std::initializer_list<int> list){
        auto listSize {static_cast<int>(list.size())};
        reallocate(listSize);
        int i{};
        for(auto element : list){
            m_data[i++] = element;
        }
        return *this;
    }
    int getLength(){ return m_size;}
    void erase(){
        delete[] m_data;
        m_size=0;
    }
    void reallocate(int size){
        if(size == m_size)
            return;
        erase();
        if(size <= 0)
            return;
        m_size = size;
        m_data = new int[size]{};
    }
    void resize(int size){
        if(size == m_size)
            return;
        if(size == 0){
            erase();  
        }else{
            int newSize { (size < m_size)? size : m_size};
            int *newData{new int[newSize]{}};
            for(auto i{0}; i < newSize; ++i){
                newData[i]= m_data[i];
            }
            delete[] m_data;
            m_data = newData;
            m_size = newSize;
        }   
    }
    void insertBefore(int value, int index){
        assert(index <= m_size && index >=0);
        int *newArray{new int[m_size+1]};
        for(auto before{0};before < index; before++){
            newArray[before] = m_data[before];
        }
        newArray[index] = value;
        for(auto after{index+1}; after <= m_size; after++){
            newArray[after] = m_data[after-1];
        }
        ++m_size;
        delete[] m_data;
        m_data = newArray;
    }
    void remove( int index){
        assert(index <m_size && index >=0);
        if(m_size <= 1)
        {
            erase();
            return;
        }


        int *newArray{new int[m_size - 1]};
        for(auto before{0};before < index; before++){
            newArray[before] = m_data[before];
        }
        for(auto after{index+1}; after < m_size; after++){
            newArray[after -1] = m_data[after];
        }
        --m_size;
        delete[] m_data;
        m_data = newArray; 
    }

    void insertAtBeginning(int value){ insertBefore(value, 0); }
    void insertAtEnd(int value){ insertBefore(value, m_size); }
    ~IntArray(){
        delete[] m_data;
    }

};

int main()
{
    // Declare an array with 10 elements
    IntArray array(10);
 
    // Fill the array with numbers 1 through 10
    for (int i{ 0 }; i<10; ++i)
        array[i] = i+1;
 
    // Resize the array to 8 elements
    array.resize(8);
 
    // Insert the number 20 before element with index 5
    array.insertBefore(20, 5);
 
    // Remove the element with index 3
    array.remove(3);
 
    // Add 30 and 40 to the end and beginning
    array.insertAtEnd(30);
    array.insertAtBeginning(40);
 
    // Print out all the numbers
    for (int i{ 0 }; i<array.getLength(); ++i)
        std::cout << array[i] << ' ';
 
//// initializer list check
    IntArray array2{ 5, 4, 3, 2, 1 }; // initializer list --> compiler will search for list constructor
	for (int count{ 0 }; count < array2.getLength(); ++count)
		std::cout << array2[count] << ' ';
    std::cout << '\n';
 
	array2 = { 1, 3, 5, 7, 9, 11 };// compiler will search for overloaded list assignment operator func
 
	for (int count{ 0 }; count < array2.getLength(); ++count)
		std::cout << array2[count] << ' ';
 
	std::cout << '\n';
    return 0;
}