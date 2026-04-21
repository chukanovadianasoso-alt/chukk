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