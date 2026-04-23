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
    
    virtual string classname() const {
        return "Base"; 
    }
    
    virtual bool isA(const string& name) const {
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
    
    Desc(double rad, string mat) : Base(mat), radius(rad) {  // вместо col
        descID = ++totalDescs;
        cout << "Desc(double,string): создан потомок id=" << descID << ", радиус=" << radius << endl;
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
    
    string classname() const override {
        return "Desc";
    }
    
    bool isA(const string& name) const override {
        if (name == "Desc") return true;
        return Base::isA(name);
    }
    
    double getRadius() const { return radius; }
    void setRadius(double r) { radius = r; }
};

int Desc::totalDescs = 0;


void demonstrateClassnameAndIsA() {
    cout << "\n1. classname() и isA()\n";
    
    Base* basePtr = new Desc(5.0, "steel"); 
    
    cout << "\nИнформация об объекте" << endl;
    cout << "  classname(): " << basePtr->classname() << endl;
    cout << "  isA(\"Base\"): " << (basePtr->isA("Base") ? "true" : "false") << endl;
    cout << "  isA(\"Desc\"): " << (basePtr->isA("Desc") ? "true" : "false") << endl;
    cout << "  isA(\"Rectangle\"): " << (basePtr->isA("Rectangle") ? "true" : "false") << endl;
    
    delete basePtr;
}


void demonstrateDangerousCast() {
    cout << "\n2. Опасное приведение C-style\n";
    
    Base* basePtr = new Desc(10.0, "aluminum"); 
    cout << "\nСоздан Desc, хранится в указателе на Base" << endl;

    Desc* badCast = (Desc*)basePtr;
    cout << "C-style cast к Desc*: ";
    badCast->setRadius(25.0);
    cout << "Новый радиус: " << badCast->getRadius() << endl;

    delete basePtr;
}


void demonstrateSafeCastWithIsA() {
    Base* basePtr = new Desc(7.0, "copper"); 
    
    cout << "\nТип объекта: " << basePtr->classname() << endl;

    if (basePtr->isA("Desc")) {
        cout << "isA(\"Desc\") вернул true -> можно безопасно привести к Desc*" << endl;
        Desc* descPtr = (Desc*)basePtr;
        cout << "Текущий радиус = " << descPtr->getRadius() << endl;
        descPtr->setRadius(12.5);
        cout << "Новый радиус = " << descPtr->getRadius() << endl;
    } else {
        cout << "isA(\"Desc\") вернул false -> нельзя приводить!" << endl;
    }

    delete basePtr;

    cout << "\nПопытка приведения неподходящего объекта" << endl;
    Base* justBase = new Base("wood");
    cout << "Создан Base, classname() = " << justBase->classname() << endl;

    if (justBase->isA("Desc")) {
        cout << "Можно приводить к Desc" << endl;
    } else {
        cout << "Нельзя приводить к Desc! isA() вернул false" << endl;
    }

    delete justBase;
}


void demonstrateDynamicCast() {
    cout << "\n4. dynamic_cast для указателей" << endl;
    Base* basePtr = new Desc(15.0, "titanium");
    cout << "Создан Desc, указатель на Base" << endl;

    Desc* descPtr = dynamic_cast<Desc*>(basePtr);
    if (descPtr) {
        cout << "dynamic_cast<Desc*> привел тип" << endl;
        cout << "Радиус = " << descPtr->getRadius() << endl;
    } else {
        cout << "dynamic_cast<Desc*> вернул nullptr" << endl;
    }

    delete basePtr;

    cout << "\ndynamic_cast для неподходящего типа" << endl;
    Base* justBase = new Base("bronze"); 
    cout << "Создан Base (не Desc)" << endl;

    Desc* failedCast = dynamic_cast<Desc*>(justBase);
    if (failedCast) {
        cout << "dynamic_cast<Desc*> привел тип" << endl;
    } else {
        cout << "dynamic_cast<Desc*> вернул nullptr" << endl;
    }

    delete justBase;

    cout << "\n5. dynamic_cast для ссылок" << endl;
    Desc circle(20.0, "gold");
    Base& baseRef = circle;
    cout << "Создан Desc, ссылка на Base" << endl;

    try {
        Desc& descRef = dynamic_cast<Desc&>(baseRef);
        cout << "dynamic_cast<Desc&> привел тип" << endl;
        cout << "Радиус через ссылку = " << descRef.getRadius() << endl;
    }
    catch (const bad_cast& e) {
        cout << "dynamic_cast<Desc&> выбросил исключение bad_cast" << endl;
    }

    cout << "\ndynamic_cast для ссылок (неудачный случай)" << endl;
    Base onlyShape("silver");
    Base& baseRef2 = onlyShape;
    cout << "Создан Base (не Desc), ссылка на Base" << endl;

    try {
        Desc& descRef = dynamic_cast<Desc&>(baseRef2);
        cout << "" << endl;
    }
    catch (const bad_cast& e) {
        cout << "dynamic_cast<Desc&> выбросил исключение bad_cast: невозможно привести Base к Desc" << endl;
    }
}

int main() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    demonstrateClassnameAndIsA();
    demonstrateDangerousCast();
    demonstrateSafeCastWithIsA();
    demonstrateDynamicCast();
    
    return 0;
}