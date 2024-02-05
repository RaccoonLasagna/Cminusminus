#include <iostream>
#include <typeinfo>

class Base {
public:
    virtual ~Base() {}
};

class Derived : public Base {};

int main() {
    Base* basePtr = new Derived();

    // ใช้ dynamic_cast เพื่อตรวจสอบว่า basePtr ชี้ไปที่ object ที่เป็นของ Derived หรือไม่
    if (Derived* derivedPtr = dynamic_cast<Derived*>(basePtr)) {
        std::cout << "basePtr point to object for Derived" << std::endl;
    } else {
        std::cout << "basePtr not point to object for Derived" << std::endl;
    }

    // ใช้ typeid เพื่อดูชนิดของ object
    std::cout << "Type of basePtr: " << typeid(*basePtr).name() << std::endl;

    delete basePtr;

    return 0;
}
