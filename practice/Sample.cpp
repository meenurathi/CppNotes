#include<iostream>
using std::cout;
using std::cin;

int main(){

    double d{1};
    float f{1.0f};
    cout<<d<<" ,"<<f<<", ";
    cout<<static_cast<double>(d);


    return 0;
}