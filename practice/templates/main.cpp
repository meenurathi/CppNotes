
#include<iostream>
#include"TemplateArray.h"
int main()
{
    // Declare an array with 10 elements
    Array<int> array(10);

    // Fill the array with numbers 1 through 10
    for (int i{ 0 }; i<10; ++i)
        array[i] = i+1;
    array.print();
     // Resize the array to 8 elements
    array.resize(8);
     array.print();
    // Insert the number 20 before element with index 5
    array.insertBefore(20, 5);
    array.print();
    // Remove the element with index 3
    array.remove(3);
    array.print();
    // Add 30 and 40 to the end and beginning
    array.insertAtEnd(30);
    array.insertAtBeginning(40);
    array.print();


//// initializer list check
    Array<double> array2{ 5.2, 4, 3, 2.3, 1.33 }; // initializer list --> compiler will search for list constructor
	array2.print();
    std::cout << '\n';
 
	array2 = { 1.11, 3.587, 5.15, 7, 9.18, 11.00 };// compiler will search for overloaded list assignment operator func
 
	array2.print(); 
	std::cout << '\n';

    return 0;
}