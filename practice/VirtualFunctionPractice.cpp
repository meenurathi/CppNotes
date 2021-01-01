/* 
1)  Create an abstract class named Shape. This class should have three functions: a pure virtual print function that takes and returns a std::ostream, an overloaded operator<< and an empty virtual destructor.
2) Derive two classes from Shape: a Triangle, and a Circle. The Triangle should have 3 Points as members. The Circle should have one center Point, and an integer radius. Overload the print() function so the following program runs:

int main()
{
    Circle c{ Point{ 1, 2, 3 }, 7 };
    std::cout << c << '\n';
 
    Triangle t{Point{1, 2, 3}, Point{4, 5, 6}, Point{7, 8, 9}};
    std::cout << t << '\n';
 
    return 0;
}
This should print:

Circle(Point(1, 2, 3), radius 7)
Triangle(Point(1, 2, 3), Point(4, 5, 6), Point(7, 8, 9))
3) Given the above classes (Point, Shape, Circle, and Triangle), finish the following program:
#include <vector>
#include <iostream>
 
int main()
{
	std::vector<Shape*> v{
      new Circle{Point{1, 2, 3}, 7},
      new Triangle{Point{1, 2, 3}, Point{4, 5, 6}, Point{7, 8, 9}},
      new Circle{Point{4, 5, 6}, 3}
    };
 
	// print each shape in vector v on its own line here
 
       	std::cout << "The largest radius is: " << getLargestRadius(v) << '\n'; // write this function
 
	// delete each element in the vector here
 
	return 0;
}
 */

#include<iostream>
class Point{
    int m_x{};
    int m_y{};
    int m_z{};
public:
    Point(int x=0, int y=0, int z=0): m_x{x}, m_y{y}, m_z{z}{}
    friend std::ostream& operator<<(std::ostream & out, const Point & p) {
        out<<"Point ("<<p.m_x<<", "<<p.m_y<<", "<<p.m_z<<") ";
        return out;
    }
};
class Shape{
public:
    virtual std::ostream& print(std::ostream& out) const = 0;
    friend std::ostream & operator<<(std::ostream & out, const Shape  & shape){
       return shape.print(out);
    }
    virtual ~Shape(){}
};

class Circle : public Shape{
    Point m_center{};
    int m_radius{};
public:
    Circle(const Point & center , int r):m_center{center}, m_radius{r}{};
    virtual std::ostream& print(std::ostream & out) const override{
        out<<"Circle ( "<<m_center<<", radius "<<m_radius<<"\n";
        return out;
    }
    int getRadius(){ return m_radius;}
};
class Triangle : public Shape{
    Point m_p1{};
    Point m_p2{};
    Point m_p3{};
public:
    Triangle(const Point & p1, const Point & p2, const Point & p3): m_p1{p1}, m_p2{p2}, m_p3{p3}{}
     virtual std::ostream & print(std::ostream & out) const override{
        out<<"Triangle ( "<<m_p1<<", "<<m_p2<<", "<<m_p3<<" )\n";
        return out;
     }
};

#include <vector>
 int getLargestRadius(const std :: vector<Shape *> &v ){
     int max{};
     Circle *pCircle{nullptr};
     for(auto & ele : v){
         pCircle = dynamic_cast<Circle *>(ele);
         if(pCircle && pCircle->getRadius() > max)
            max = pCircle->getRadius();
     }
     return max;
 }
int main()
{
	std::vector<Shape*> v{
      new Circle{Point{1, 2, 3}, 7},
      new Triangle{Point{1, 2, 3}, Point{4, 5, 6}, Point{7, 8, 9}},
      new Circle{Point{4, 5, 6}, 3}
    };
 
	// print each shape in vector v on its own line here
    for( auto * shape : v){
        std::cout<<*shape;
    }
    std::cout << "The largest radius is: " << getLargestRadius(v) << '\n'; // write this function
 
	// delete each element in the vector here
    for(auto * ele : v){
        delete ele;
        ele = nullptr;
    }
    
	return 0;
}