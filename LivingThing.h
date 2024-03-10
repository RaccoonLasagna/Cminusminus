#include "FundamentalSystem.h"
// #include "FundamentalSystem.cpp"
using namespace std;

#ifndef LIVINGTHING_H
#define LIVINGTHING_H

// class GameObject {
// private:
//   string represent;
//   string name;
//   StatusSystem *stat; // stat ต้องเป็น pointer เพราะว่ามันสามาถเปลี่ยนเป็นตัวอื่นได้
//   Layer *parent; // stat ต้องเป็น pointer เพราะว่ามันสามาถเปลี่ยนย้าย layer ได้

// public:
//   explicit GameObject(Layer *, string, string, int, int, int, int, int, int,
//                       vector<StatParameterMax *> *, vector<Ability *> *,
//                       vector<Affliction *> *); // ต้องมี parent ใน stat ด้วย
//   //-------------------------//
//   void virtual update() = 0; // ให้ class ลูกไปเขียนเองเพราะบางอย่าง เช่น พื้นดิน
//                              // ก็ไม่ต้องตัดสินใจเหมือนสัตว์
//   //-------------------------//
//   inline void setName(const string n);
//   inline void setRepresent(const string r);
//   void setStat(StatusSystem *statInput); // ไปแย่งเอา stat ของตัวอื่นมาใช้
//                                          // จะเอามาใช้สำหรับกรณีสัตว์วิวัฒนาการ
//   void setParent(Layer *p); // เปลี่ยนแบบไม่สนว่าที่ตรงนั้นจะไปทับใครใน layer เดียวกันไหม
//   //-------------------------//
//   string getRepresent();
//   string getName();
//   StatusSystem *getStat();
//   Layer *getParent();
//   //-------------------------//
//   int getX();
//   int getY();
// };

class LivingThing : public GameObject{
    private:
    public:
};

class TestLivingThing: public GameObject{
    private:
    public:
    TestLivingThing(Layer *parent, int x, int y);
    int getSur();
    int getEnv();
    int getRepo();
    void update();
};

// method_test.cpp(11, 9): pure virtual function "GameObject::getSur" has no overrider
// method_test.cpp(11, 9): pure virtual function "GameObject::getEnv" has no overrider
// method_test.cpp(11, 9): pure virtual function "GameObject::getRepo" has no overrider
// method_test.cpp(11, 9): pure virtual function "GameObject::update" has no overrider

#endif