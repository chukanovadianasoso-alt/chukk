#include <iostream>
#include <string>
#include <memory>

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
        cout << "Base(string): создан объект id=" << id << ", материал=" << material << endl;
    }
    
    Base(const Base& other) : material(other.material) {
        id = ++totalBase;
        cout << "Base(Base&): создан объект id=" << id 
             << " (копия из id=" << other.id << ")" << endl;
    }
    
    Base(Base* other) : material(other->material) {
        id = ++totalBase;
        cout << "Base(Base*): создан объект id=" << id 
             << " из указателя на id=" << other->id << endl;
    }
    
    virtual ~Base() {
        totalBase--;
        cout << "~Base(): удалён объект id=" << id << ", материал=" << material << endl;
    }
    
    virtual void display() const {
        cout << "Base::display(): материал=" << material << endl;
    }

    virtual Base* copy() const {
        return new Base(*this);
    }
    
    virtual string getClassName() const {
        return "Base";
    }
    
    virtual bool isType(const string& name) const {
        return name == "Base";
    }
    
    int getId() const { return id; }
    string getMaterial() const { return material; }
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
    
    Desc(double rad, string mat) : Base(mat), radius(rad) {
        descID = ++totalDescs;
        cout << "Desc(double,string): создан потомок id=" << descID 
             << ", радиус=" << radius << endl;
    }
    
    Desc(const Desc& other) : Base(other), radius(other.radius) {
        descID = ++totalDescs;
        cout << "Desc(Desc&): создан потомок id=" << descID 
             << " (копия из id=" << other.descID << ")" << endl;
    }
    
    Desc(Desc* other) : Base(other), radius(other->radius) {
        descID = ++totalDescs;
        cout << "Desc(Desc*): создан потомок id=" << descID 
             << " из указателя на потомка id=" << other->descID << endl;
    }
    
    ~Desc() override {
        totalDescs--;
        cout << "~Desc(): удалён потомок id=" << descID << ", радиус=" << radius << endl;
    }
    
    void display() const override {
        cout << "Desc::display(): радиус=" << radius << ", материал=" << getMaterial() << endl;
    }

    Base* copy() const override {
        return new Desc(*this);
    }

    string getClassName() const override {
        return "Desc";
    }
    
    bool isType(const string& name) const override {
        if (name == "Desc") return true;
        return Base::isType(name);
    }
    
    double getRadius() const { return radius; }
    void setRadius(double r) { radius = r; }
};

int Desc::totalDescs = 0;


void receiveByValue(Base obj) {
    cout << "  receiveByValue: получила объект по значению, id=" << obj.getId() << endl;
    obj.display();
}

void receiveByPointer(Base* obj) {
    cout << "  receiveByPointer: получила указатель, id=" << obj->getId() << endl;
    obj->display();
}

void receiveByReference(Base& obj) {
    cout << "  receiveByReference: получила ссылку, id=" << obj.getId() << endl;
    obj.display();
}


Base returnLocalByValue() {
    Desc local(5.0, "wood");
    cout << "  returnLocalByValue: возвращаем локальный объект по значению" << endl;
    return local;
}

Base* returnLocalByPointer() {
    Desc local(7.0, "metal");
    cout << "  returnLocalByPointer: возвращаем указатель на локальный объект" << endl;
    return &local;
}

Base& returnLocalByReference() {
    Desc local(10.0, "glass");
    cout << "  returnLocalByReference: возвращаем ссылку на локальный объект" << endl;
    return local;
}

Base returnDynamicByValue() {
    Desc* ptr = new Desc(15.0, "ceramic");
    cout << "  returnDynamicByValue: возвращаем динамический объект по значению" << endl;
    return *ptr;
}

Base* returnDynamicByPointer() {
    Desc* ptr = new Desc(20.0, "rubber");
    cout << "  returnDynamicByPointer: возвращаем указатель на динамический объект" << endl;
    return ptr;
}

Base& returnDynamicByReference() {
    Desc* ptr = new Desc(25.0, "leather");
    cout << "  returnDynamicByReference: возвращаем ссылку на динамический объект" << endl;
    return *ptr;
}


void demoUniquePtr() {
    cout << "\n1. unique_ptr" << endl;
    
    unique_ptr<Desc> ptr1 = make_unique<Desc>(30.0, "carbon");
    ptr1->display();
    
    unique_ptr<Desc> ptr2 = move(ptr1);
    
    if (!ptr1) {
        cout << "ptr1 теперь пуст" << endl;
    }
    
    cout << "ptr2-> ";
    ptr2->display();
    
}

void demoSharedPtr() {
    cout << "\n2. shared_ptr" << endl;
    
    shared_ptr<Desc> ptr1 = make_shared<Desc>(40.0, "stone");
    cout << "Счётчик ссылок: " << ptr1.use_count() << endl;
    
    {
        shared_ptr<Desc> ptr2 = ptr1;
        cout << "Счётчик ссылок (внутри блока): " << ptr1.use_count() << endl;
    }
    
    cout << "Счётчик ссылок (после выхода из блока): " << ptr1.use_count() << endl;
    cout << "Объект удалится когда счётчик станет 0" << endl;
}


void showPassing() {
    cout << "\n1. Передача объектов в функции" << endl;
    
    Desc item(25.0, "bronze");
    cout << "\nСоздан объект Desc id=" << item.getId() << ", радиус=" << item.getRadius() << endl;
    
    cout << "\nПередача по значению (receiveByValue)" << endl;
    cout << "(вызывается конструктор копирования)" << endl;
    receiveByValue(item);
    
    cout << "\nПередача по указателю (receiveByPointer)" << endl;
    cout << "(копия не создаётся)" << endl;
    receiveByPointer(&item);
    
    cout << "\nПередача по ссылке (receiveByReference)" << endl;
    cout << "(копия не создаётся)" << endl;
    receiveByReference(item);
}


void showReturn() {
    cout << "\n2. Возврат объектов из функции" << endl;
    
    cout << "\nФункция 1: возврат локального объекта по значению" << endl;
    Base obj1 = returnLocalByValue();
    
    cout << "\nФункция 2: возврат указателя на локальный объект" << endl;
    Base* badPtr = returnLocalByPointer();

    cout << "\nФункция 3: возврат ссылки на локальный объект" << endl;
    Base& badRef = returnLocalByReference();

    
    cout << "\nФункция 4: возврат динамического объекта по значению" << endl;
    Base obj2 = returnDynamicByValue();

    cout << "\nФункция 5: возврат указателя на динамический объект" << endl;
    Base* goodPtr = returnDynamicByPointer();
    goodPtr->display();
    delete goodPtr;
    
    cout << "\nФункция 6: возврат ссылки на динамический объект" << endl;
    Base& goodRef = returnDynamicByReference();
    goodRef.display();
    delete &goodRef;
}


void showSmartPointers() {
    cout << "\n3. Умные указатели" << endl;
    
    demoUniquePtr();
    demoSharedPtr();
}


void runCopyWithSmartPtr() {
    cout << "\n4. Умные указатели и виртуальное копирование" << endl;

    cout << "Создание unique_ptr<Base> ptrA = Desc" << endl;
    unique_ptr<Base> ptrA = make_unique<Desc>(12.5, "titanium");

    cout << "Вызов ptrA->copy() (виртуальный метод)" << endl;
    unique_ptr<Base> ptrB(ptrA->copy());

    cout << "\nСравнение объектов после copy" << endl;
    cout << "ptrA className = " << ptrA->getClassName() << endl;
    cout << "ptrB className = " << ptrB->getClassName() << endl;

    cout << "Адрес ptrA = " << ptrA.get() << endl;
    cout << "Адрес ptrB = " << ptrB.get() << endl;

    cout << "\nПроверка isType:" << endl;
    cout << "ptrA isType('Base')? " << (ptrA->isType("Base") ? "да" : "нет") << endl;
    cout << "ptrA isType('Desc')? " << (ptrA->isType("Desc") ? "да" : "нет") << endl;

}


int main() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    showPassing();
    showReturn();
    showSmartPointers();
    runCopyWithSmartPtr();

    return 0;
}