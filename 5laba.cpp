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

    virtual ~Base() {
        totalBase--;
        cout << "~Base(): удалён объект id=" << id << endl;
    }

    void nonVirtualMethod() {
        cout << "Base::nonVirtualMethod() id=" << id << endl;
    }

    virtual void virtualMethod() {
        cout << "Base::virtualMethod() id=" << id << endl;
    }

    virtual void render() {
        cout << "Base::render() вызывает virtualMethod(): ";
        virtualMethod();
    }

    void callNonVirtual() {
        cout << "Base::callNonVirtual() вызывает nonVirtualMethod(): ";
        nonVirtualMethod();
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

    ~Desc() override {
        totalDescs--;
        cout << "~Desc(): удалён потомок id=" << descID << ", радиус=" << radius << endl;
    }

    void nonVirtualMethod() {
        cout << "Desc::nonVirtualMethod() -> потомок id=" << descID
             << ", радиус=" << radius << endl;
    }

    void virtualMethod() override {
        cout << "Desc::virtualMethod() -> потомок id=" << descID
             << ", радиус=" << radius << endl;
    }

    void render() override {
        cout << "Desc::render() вызывает virtualMethod(): ";
        virtualMethod();
    }
};

int Desc::totalDescs = 0;


void demonstrateVirtualVsNonVirtual() {
    cout << "\n1. Виртуальные и невирутуальные методы\n";

    Base* basePtr = new Desc(15.5, "steel");   
    Desc* descPtr = new Desc(10.0, "aluminum"); 

    cout << "\nВызов через указатель на базовый класс (Base*)" << endl;
    cout << "  nonVirtualMethod (невиртуальный): ";
    basePtr->nonVirtualMethod();  

    cout << "  virtualMethod (виртуальный): ";
    basePtr->virtualMethod();

    cout << "\nВызов через указатель на класс-потомок (Desc*)" << endl;
    cout << "  nonVirtualMethod: ";
    descPtr->nonVirtualMethod(); 

    cout << "  virtualMethod: ";
    descPtr->virtualMethod();  

    cout << "\nВызов метода из другого метода" << endl;
    cout << "  callNonVirtual():" << endl;
    basePtr->callNonVirtual();

    cout << "  render():  " << endl;
    basePtr->render();       

    delete basePtr;
    delete descPtr;
}


void demonstrateOverrideVsHide() {
    cout << "\n2. Переопределение и перекрытие\n";

    Desc* descPtr = new Desc(7.0, "copper"); 
    Base* basePtr = descPtr;

    cout << "\nЧерез указатель на потомка (Desc*)" << endl;
    descPtr->nonVirtualMethod(); 
    descPtr->virtualMethod(); 

    cout << "\nЧерез указатель на базовый класс (Base*)" << endl;
    basePtr->nonVirtualMethod();   
    basePtr->virtualMethod();    
    delete descPtr;
}


int main() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    demonstrateVirtualVsNonVirtual();
    demonstrateOverrideVsHide();
    return 0;
}