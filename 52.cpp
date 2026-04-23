#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

class Base {
protected:
    string material;  
    int id;

public:
    static int totalBase;

    Base() : material("plastic") {  
        id = ++totalBase;
        cout << "Base(): создан объект id=" << id << ", материал=" << material << endl;
    }

    Base(string material_input) : material(material_input) {
        id = ++totalBase;
        cout << "Base() с параметром: создан объект id=" << id << ", материал=" << material << endl;
    }

    ~Base() {
        totalBase--;
        cout << "~Base(): удалён объект id=" << id << endl;
    }
};

int Base::totalBase = 0;


class Desc : public Base {
private:
    double radius;
    int descID; 

public:
    static int totalDescs;  

    Desc() : Base(), radius(1.0) {
        descID = ++totalDescs;
        cout << "Desc(): создан потомок id=" << descID << ", радиус=" << radius << endl;
    }

    Desc(double rad, string mat) : Base(mat), radius(rad) {  // вместо col
        descID = ++totalDescs;
        cout << "Desc(double,string): создан потомок id=" << descID << ", радиус=" << radius << endl;
    }

    ~Desc(){
        totalDescs--;
        cout << "~Desc(): удалён потомок id=" << descID << ", радиус=" << radius << endl;
    }
};
int Desc::totalDescs = 0;

int main (){
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif
    cout << "Демонстация использования невиртуального деструктора" << endl;
    Base* ptr1 = new Desc; 
    delete ptr1;
}