//Relationships:
/* 
Terminology:
    object--> parentcontains part/component
1)Object Composition :"has-a" /"part-of"
     process of building complex objts from simpler ones
     e.g struct & car are composite types
    sub types
        ---composition
        ---aggregation */

/*>>>>>>Composition
    To qualify as a composition, an object and a part must have the following relationship:
        The part (member) is part of the object (class)
        The part (member) can only belong to one object (class) at a time
            single owner
        The part (member) has its existence managed by the object (class)
            death relationship
        The part (member) does not know about the existence of the object (class)
            unidirectional relationship, because the body knows about the heart, but not the other way around
    --
    
        > A composition may defer creation of some parts until they are needed. For example, a string class may not create a dynamic array of characters until the user assigns the string some data to hold.
        > A composition may opt to use a part that has been given to it as input rather than create the part itself.
        > A composition may delegate destruction of its parts to some other object (e.g. to a garbage collection routine).
    The key point here is that the composition should manage its parts without the user of the composition needing to manage anything.

    That task should either be the storage and manipulation of some kind of data (e.g. Point2D, std::string), OR the coordination of subclasses (e.g. Creature). Ideally not both.
 */
  /* >>>>>  Aggregation "has-a"

To qualify as an aggregation, a whole object and its parts must have the following relationship:
    The part (member) is part of the object (class)                             (SAME AS COMPOSITION)
            part-whole relationship
    The part (member) can belong to more than one object (class) at a time      (opposite of Composition)
            stores as reference or pointer , which are provided during construction or by member functions
    The part (member) does not have its existence managed by the object (class) (opposite of Composition)
            Part may exist before creating containing object or may persist after containing obj destruction
    The part (member) does not know about the existence of the object (class)   (SAME AS COMPOSTION)
            unidirection relationship
*/
/*
    Compositions: e.g body - heart (heart cannot exist without body}
        Typically use normal member variables
        Can use pointer members if the class handles object allocation/deallocation itself
        Responsible for creation/destruction of parts
    Aggregations: e.g bag-pebble, car-engine (engine can exist without car)
        Typically use pointer or reference members that point to or reference objects that live outside the scope of the aggregate class
        Not responsible for creating/destroying parts
        >this can cause memory leak, if part objt is not destroyed by external identity
        >so, composition should be pereferred over aggregation
  Note:  It is worth noting that the concepts of composition and aggregation are not mutually exclusive, and can be mixed freely within the same class.
*/

//....... reference_wrapper
/* 
    1) std::reference_wrapper lives in the <functional> header.
    2) When you create your std::reference_wrapper wrapped object, the object can’t be an anonymous object 
        (since anonymous objects have expression scope would leave the reference dangling).
        >allow us to create vector of references
    3) When you want to get your object back out of std::reference_wrapper, you use the get() member function.
    The good news is that you don’t really need to understand how it works to use it. All you need to know are three things:
 */
        #include <functional> // std::reference_wrapper
        #include <iostream>
        #include <vector>
        #include <string>
        
        int main1()
        {
            std::string tom{ "Tom" };
            std::string berta{ "Berta" };
            //std::vector<const std::string&> names {tom, berta}; //illegal
            //List elements cannot be references because references have to be initialized and cannot be reassigned
            // Vector of const references to std::string
            std::vector<std::reference_wrapper<std::string>> names {tom, berta};
            std::string james{"james"};
            names.push_back(james);
            for(auto name: names){
                name.get()+=" Bean";
            }
            for(auto name: names)
                std::cout<<name.get()<<", ";
            return 0;
        }
        /*
        Would you be more likely to implement the following as a composition or an aggregation?
            a) A ball that has a color
            b) An employer that is employing multiple people
            c) The departments in a university
            d) Your age
            e) A bag of marbles
        ans:
            a) Composition: Color is an intrinsic property of a ball.
            b) Aggregation: An employer doesn’t start with any employees and hopefully doesn’t destroy all its employees when it goes bankrupt.
            c) Composition: Departments can’t exist in absence of a university.
            d) Composition: Your age is an intrinsic property of you.
            e) Aggregation: The bag and the marbles inside have independent existences.
          */
         //e.g.
                 #include <functional> // std::reference_wrapper
                #include <iostream>
                #include <string>
                #include <vector>
                
                class Teacher
                {
                    private:
                    std::string m_name{};                    
                    public:
                    Teacher(const std::string& name)
                        : m_name{ name }{ }
                    const std::string& getName() const { return m_name; }
                };
                    
                class Department
                {
                    private:
                    std::vector<std::reference_wrapper<const Teacher>> m_teachers{};
                    
                    public:
                    // Pass by regular reference. The user of the Department class shouldn't care
                    // about how it's implemented.
                    void add(const Teacher& teacher)
                    {
                        m_teachers.push_back(teacher);
                    }
                    
                    friend std::ostream& operator<<(std::ostream& out, const Department& department)
                    {
                        out << "Department: ";
                    
                        for (const auto& teacher : department.m_teachers)
                        {
                        out << teacher.get().getName() << ' ';
                        }
                    
                        out << '\n';
                    
                        return out;
                    }
                };
                
                int main2()
                {
                    // Create a teacher outside the scope of the Department
                    Teacher t1{ "Bob" };
                    Teacher t2{ "Frank" };
                    Teacher t3{ "Beth" };
                    
                    {
                        // Create a department and add some Teachers to it
                        Department department{}; // create an empty Department
                    
                        department.add(t1);
                        department.add(t2);
                        department.add(t3);
                    
                        std::cout << department;
                    
                    } // department goes out of scope here and is destroyed
                    
                    std::cout << t1.getName() << " still exists!\n";
                    std::cout << t2.getName() << " still exists!\n";
                    std::cout << t3.getName() << " still exists!\n";
                    
                    return 0;
                }


//Association : relationship between two unrelated objects
            //"uses-a " relationship
/* 
    To qualify as an association, an object and another object must have the following relationship:
        The associated object (member) is otherwise unrelated to the object (class)
        The associated object (member) can belong to more than one object (class) at a time
        The associated object (member) does not have its existence managed by the object (class)
        The associated object (member) may or may not know about the existence of the object (class)    
            >unidirection (preferred)/ bidirectional relationship(shoulb be avoided)
        eg. dr patient rleationship >> dr uses patient to earn money, patient uses dr for health
        Associations may be implemented via pointer or reference, or by a more indirect means (such as holding the index or key of the associated object).
 */
    #include<string>
    #include<vector>
    class Patient;
    class Doctor{
        std::string m_name{};
        std::vector<std::reference_wrapper<const Patient>> m_patient;
    public:
        Doctor(const std::string &name):m_name(name){};
        void addPatient( Patient & patient);
        const std::string & getName()const {
            return m_name;
        }
        friend std::ostream& operator<<(std::ostream &out, const Doctor &doctor);
    };
    class Patient{
        std::string m_name{};
        std::vector<std::reference_wrapper<const Doctor>> m_doctor{};
    public:
        Patient(const std::string & name):m_name{name}{};
        void addDoctor(const Doctor & dr){
            m_doctor.push_back(dr);
        }
        const std::string &getName()const {
            return m_name;
        }
        friend std::ostream& operator<<(std::ostream &out, const Patient &patient);
    };
    void Doctor::addPatient( Patient & patient){
        m_patient.push_back(patient);
        patient.addDoctor(*this);
    }
    std::ostream& operator<<(std::ostream &out, const Doctor &doctor)
    {
        if (doctor.m_patient.empty())
        {
            out << doctor.m_name << " has no patients right now";
            return out;
        }
    
        out << doctor.m_name << " is seeing patients: ";
        for (const auto& patient : doctor.m_patient)
            out << patient.get().getName() << ", ";
    
        return out;
    }
    
    std::ostream& operator<<(std::ostream &out, const Patient &patient)
    {
        if (patient.m_doctor.empty())
        {
            out << patient.getName() << " has no doctors right now";
            return out;
        }
    
        out << patient.m_name << " is seeing doctors: ";
        for (const auto& doctor : patient.m_doctor)
            out << doctor.get().getName() << ", ";
    
        return out;
    }
    
    int main()
    {
        // Create a Patient outside the scope of the Doctor
        Patient dave{ "Dave" };
        Patient frank{ "Frank" };
        Patient betsy{ "Betsy" };
    
        Doctor james{ "James" };
        Doctor scott{ "Scott" };
    
        james.addPatient(dave);
    
        scott.addPatient(dave);
        scott.addPatient(betsy);
    
        std::cout << james << '\n';
        std::cout << scott << '\n';
        std::cout << dave << '\n';
        std::cout << frank << '\n';
        std::cout << betsy << '\n';
    
        return 0;
    }

//.. Reflexive association
 //objects may have a relationship with other objects of the same type. 
         #include <string>
        class Course
        {
        private:
            std::string m_name;
            const Course *m_prerequisite;//reflexive relationship        
        public:
            Course(const std::string &name, const Course *prerequisite = nullptr):
                m_name{ name }, m_prerequisite{ prerequisite } { }       
        };
//This can lead to a chain of associations (a course has a prerequisite, which has a prerequisite, etc…)
//Associations can be indirect :
    /* Any kind of data that allows you to link two objects together suffices. 
    a Driver class can have a unidirectional association with a Car USING CARID ,without actually including a Car pointer or reference member . using cardid it can search in carLot for getting car of the carid */
/* 
Composition vs aggregation vs association vs dependecy summary
    Property	                         Composition   Aggregation	  Association            Dependency
    Relationship type	                 Whole/part	    Whole/part    Otherwise unrelated     Otherwise unrelated
    Members can belong to multiple classes	 No	           Yes           Yes                    Yes
    Members existence managed by class       Yes	       No	         No                     No  Directionality	                    Unidirectional	Unidirectional	Unidirectional          Unidirectional
                                                                        or bidirectional        or bidirectional
    Relationship verb	                    Part-of	       Has-a          Uses-a                Depends-on
 */