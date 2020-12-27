#include <iostream>
 class Fraction{
     int m_num{};
     int m_deno{};
public:
    Fraction(int num =0, int deno =1):m_num{num}, m_deno{deno} {
    }
    void print() const{
        std::cout<<m_num<<"/"<<m_deno<<"\n";
    }
    friend Fraction operator*(const Fraction & f1, const Fraction &f2);
    friend Fraction operator*(const Fraction & f1, int mul);
    friend Fraction operator*( int mul,const Fraction & f1);
    friend std::ostream & operator<<(std::ostream & out , const Fraction  & fr);
    friend std::istream & operator>>(std::istream & in ,  Fraction  & fr);
 };
    std::ostream & operator<<(std::ostream & out , const Fraction  & fr){
        out<<fr.m_num<<"/"<<fr.m_deno;
        return out;
    }
    std::istream & operator>>(std::istream & in ,  Fraction  & fr){
        in>>fr.m_num;
        in.ignore(std::numeric_limits<std::streamsize>::max(),'/');
        in>>fr.m_deno;
        return in;
    }
    Fraction operator*(const Fraction & f1, const Fraction &f2){
        return {f1.m_num *f2.m_num, f1.m_deno *f2.m_deno};
    }
    Fraction operator*(const Fraction & f1, int mul){
        return {f1.m_num *mul, f1.m_deno};
    }
    Fraction operator*( int mul,const Fraction & f1){
        return {f1 * mul};
    }
int main1()
{
    Fraction f1{2, 5};
    f1.print();
 
    Fraction f2{3, 8};
    f2.print();
 
    Fraction f3{ f1 * f2 };
    f3.print();
 
    Fraction f4{ f1 * 2 };
    f4.print();
 
    Fraction f5{ 2 * f2 };
    f5.print();
 
    Fraction f6{ Fraction{1, 2} * Fraction{2, 3} * Fraction{3, 4} };
    f6.print();

 	Fraction f7{};
	std::cout << "Enter fraction 1: ";
	std::cin >> f7;
 
	Fraction f8{};
	std::cout << "Enter fraction 2: ";
	std::cin >> f8;
 
	std::cout << f7 << " * " << f8 << " is " << f7 * f8<< '\n'; // note: The result of f1 * f2 is an r-value
 
	return 0;
    return 0;
}
#include<string>
#include<vector>
#include<algorithm>
struct StudentGrade {
    std::string m_name{};
    char m_grade{}      ;
};
class GradeMap {
    std::vector<StudentGrade> m_map{};
public:
    char & operator[](const std::string & name){
        auto found { std::find_if(m_map.begin(), m_map.end(), [name](auto const &s){
            return (s.m_name == name);
        })};
        if(found == m_map.end()){
            m_map.push_back({name});
            return m_map.back().m_grade;
        }else{
            return found->m_grade;
        }
    }
    
};
int main2()
{
	GradeMap grades{};
 
	grades["Joe"] = 'A';
	grades["Frank"] = 'B';
    auto gJoe{grades["Joe"]};       // undefined behavior , holding & to vector element
    auto gFrank{grades["Frank"]};   // bz the vector can get resize any time
    grades["Harry"] = 'C';
	std::cout << "Joe has a grade of " << grades["Joe"] << '\n';
	std::cout << "Frank has a grade of " << grades["Frank"] << '\n';
	std::cout << "Joe has a grade of " << gJoe << '\n';//not prefered
	std::cout << "Frank has a grade of " << gFrank << '\n';//not prefered;
 
	return 0;
}

//e.g3
class Point{
    int m_x{};
    int m_y{};
public:
    Point(int x=0, int y=0):m_x{x}, m_y{y}{};
    Point operator-(){
        m_x = -m_x;
        m_y = -m_y;
        return *this;
    }
    Point & operator=(const int x){
           m_x = x;
           m_y = 0;
           return *this;
    }
    friend Point operator +(const Point &p1, const Point &p2);
    friend std::ostream& operator<<(std::ostream & out, const Point &p){
        out<<p.m_x<<" "<<p.m_y<<"\n";
        return out;
    }
};
Point operator+(const Point &p1, const Point &p2){
    return {p1.m_x + p2.m_x, p1.m_y+p2.m_y};
}
int main3(){
    Point p1{9,6};
    Point p2(8,2);
    std::cout<<p1+p2<<" "<<-p1;
    p2=5;
    std::cout<<p2;
    return 0;
}

//eg.4
class Average{
    std::int_least32_t m_sum{};
    std::int_least8_t m_count{};
    public:
    Average(int sum=0, int count=0):m_sum(sum), m_count(count){};
    /*Average(const Average & src){
        *this = src;
    }*///default is enough
    Average & operator+=(int x){
        m_count++;
        m_sum +=x;
        return *this;
    }
    friend std::ostream & operator<<(std::ostream & out, const Average & avg){
        out<<static_cast<double>(avg.m_sum)/avg.m_count<<"\n";
        return out;
    }
};
int main4()
{
	Average avg{};
	
	avg += 4;
	std::cout << avg << '\n'; // 4 / 1 = 4
	
	avg += 8;
	std::cout << avg << '\n'; // (4 + 8) / 2 = 6
 
	avg += 24;
	std::cout << avg << '\n'; // (4 + 8 + 24) / 3 = 12
 
	avg += -10;
	std::cout << avg << '\n'; // (4 + 8 + 24 - 10) / 4 = 6.5
 
	(avg += 6) += 10; // 2 calls chained together
	std::cout << avg << '\n'; // (4 + 8 + 24 - 10 + 6 + 10) / 6 = 7
 
	Average copy{ avg };
	std::cout << copy << '\n';
 
	return 0;
}

//e.g 5
#include <iostream>
#include<cassert>
 class IntArray{
     int m_size{0};
     int *m_arr{nullptr};

public:
    IntArray() = delete;
    IntArray(int size):m_size{size}{
        assert(size>0);
        m_arr= new int[size]{};
    };
    void deepcopy(const IntArray & src) {
        if(m_arr!= nullptr){
            delete[] m_arr;
            m_arr = nullptr;
        }
        m_size = src.m_size;
        if(src.m_arr){
            m_arr = new int[src.m_size]{};
            for(int i =0; i <src.m_size; i++){
                m_arr[i]= src.m_arr[i];
            }
        }
    }
    IntArray(const IntArray &src){
        deepcopy(src);
    }
    IntArray & operator=(const IntArray &src){
        if(this == &src)
            return *this;
        deepcopy(src);
        return *this;
    }
    int& operator[](int index){
        assert((index >= 0 )&& (index< m_size));
        return m_arr[index];
    }
    friend std::ostream & operator <<(std::ostream &out, const IntArray & arr){
        for(int i =0; i < arr.m_size; i++)
            out<<arr.m_arr[i]<<" ";
        out<<"\n";
        return out;
    }
    ~IntArray(){
        delete[] m_arr;
    }
 };
IntArray fillArray()
{
	IntArray a(5);
 
	a[0] = 5;
	a[1] = 8;
	a[2] = 2;
	a[3] = 3;
	a[4] = 6;
 
	return a;
}
 
int main()
{
	IntArray a{ fillArray() };
	std::cout << a << '\n';
 
	auto &ref{ a }; // we're using this reference to avoid compiler self-assignment errors
	a = ref;
 
	IntArray b(1);
	b = a;
 
	std::cout << b << '\n';
 
	return 0;
}