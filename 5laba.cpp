#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

class Base {
protected:
    string color;
    int id;

public:
    static int totalObjects;

    Base() : color("blue") {
        id = ++totalObjects;
        cout << "Base(): создан объект id=" << id << endl;
    }

    Base(string color_input) : color(color_input) {
        id = ++totalObjects;
        cout << "Base(string): создан объект id=" << id << endl;
    }

    virtual ~Base() {
        totalObjects--;
        cout << "~Base(): удалён объект id=" << id << endl;
    }

    void nonVirtualMethod() {
        cout << "Base::nonVirtualMethod() -> id=" << id << endl;
    }

    virtual void virtualMethod() {
        cout << "Base::virtualMethod() -> id=" << id << endl;
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

int Base::totalObjects = 0;

class Desc : public Base {
private:
    double value;
    int descID;

public:
    static int totalDesc;

    Desc() : Base(), value(10.0) {
        descID = ++totalDesc;
        cout << "Desc(): создан потомок id=" << descID << endl;
    }

    Desc(double val, string color) : Base(color), value(val) {
        descID = ++totalDesc;
        cout << "Desc(double,string): создан потомок id=" << descID << endl;
    }

    ~Desc() override {
        totalDesc--;
        cout << "~Desc(): удалён потомок id=" << descID << endl;
    }

    void nonVirtualMethod() {
        cout << "Desc::nonVirtualMethod() -> потомок id=" << descID
             << " значение=" << value << endl;
    }

    void virtualMethod() override {
        cout << "Desc::virtualMethod() -> потомок id=" << descID
             << " значение=" << value << endl;
    }

    void render() override {
        cout << "Desc::render() вызывает virtualMethod(): ";
        virtualMethod();
    }
};

int Desc::totalDesc = 0;

void demonstrateVirtualVsNonVirtual() {
    cout << "\n1. Виртуальный и Невиртуальный метод\n";

    Base* basePtr = new Desc(15.5, "red");
    Desc* descPtr = new Desc(10.0, "green");

    cout << "\nВызов виртуального и невиртуального метода объекта Base* = объект Desc:" << endl;
    basePtr->nonVirtualMethod();  
    basePtr->virtualMethod();     

    cout << "\nВызов виртуального и невиртуального метода объекта Desc* = объект Desc:" << endl;
    descPtr->nonVirtualMethod(); 
    descPtr->virtualMethod();    

    delete basePtr;
    delete descPtr;
}

void demonstrateCallFromMethod() {
    cout << "\n2. Вызов метода из другого метода\n";

    Base* descPtr = new Desc(20.0, "yellow");

    cout << "\ncallNonVirtual() (вызывает НЕвиртуальный метод):" << endl;
    descPtr->callNonVirtual();  

    cout << "\nrender() (вызывает виртуальный метод):" << endl;
    descPtr->render();         

    delete descPtr;
}

void demonstrateOverrideVsHide() {
    cout << "\n3. Переопределение и перекрытие\n";

    Desc* descPtr = new Desc(7.0, "pink");

    cout << "\nЧерез Desc*:" << endl;
    descPtr->nonVirtualMethod();  
    descPtr->virtualMethod();     

    cout << "\nЧерез Base* (тот же объект):" << endl;
    Base* basePtr = descPtr;
    basePtr->nonVirtualMethod();  
    basePtr->virtualMethod();     

    delete descPtr;
}

void demonstrateDirectObjectCall() {
    cout << "\n4. Вызов через объект\n";

    Desc desc(30.0, "purple");

    cout << "\nЧерез объект:" << endl;
    desc.nonVirtualMethod();  
    desc.virtualMethod();    

    Base& baseRef = desc;
    cout << "\nЧерез ссылку Base&:" << endl;
    baseRef.nonVirtualMethod();  
    baseRef.virtualMethod();     
}

int main() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif
    demonstrateVirtualVsNonVirtual();
    demonstrateCallFromMethod();
    demonstrateOverrideVsHide();
    demonstrateDirectObjectCall();

    return 0;
}
