#include<iostream>
#include<cmath>
using std::cin;
using std::cout;
class Point2d{
    double m_x{};
    double m_y{};
public:
    Point2d(double x =0.0, double y= 0.0 ):m_x(x), m_y(y){}
    void print() const{
        cout<<"Point2d("<<m_x<<", "<<m_y<<")\n";
    }
    double distanceTo(const Point2d & other)const{
        return (std::sqrt((m_x - other.m_x)*(m_x - other.m_x) + (m_y - other.m_y)*(m_y - other.m_y)));
    }
    friend double distanceFrom(const Point2d & first, const Point2d & second);
};
double distanceFrom(const Point2d & first, const Point2d & second){
    return (std::sqrt((first.m_x - second.m_x)*(first.m_x - second.m_x) + (first.m_y - second.m_y)*(first.m_y - second.m_y)));
}
int main(){
    Point2d first{};
    Point2d second{3.0, 4.0};
    first.print();
    second.print();
    std::cout << "Distance between two points: " << first.distanceTo(second) << '\n';
    std::cout << "Distance between two points: " << distanceFrom(first, second) << '\n';

    return 0;
}