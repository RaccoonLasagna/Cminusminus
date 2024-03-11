#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

class StatParameter;
class StatParameterMax;
class StatParameterCoord;
class Ability;
class AbilitySystem;
class Affliction;
class AfflictionSystem;
class StatusSystem;
class GameObject;
class Layer;
class LayerSystem;
class Biome;
class LivingThing;
class Animal;
class Plant;
class NonLivingThing;
class Field;

#ifndef FUNDAMENTALSYSTEM_H
#define FUNDAMENTALSYSTEM_H

class StatParameter
{ // เป็น paramiter ที่ไม่สามารถติดลบได้
private:
  string name; // มี Name เพราะว่าเวลาที่เราต้องการเรียกใช้เราต้องการเรียกด้วยชื่อ
  int value;
  StatusSystem &parent; // เป็นประเภทที่ไม่สามารถเปลี่ยน parent ได้

public:
  explicit StatParameter(StatusSystem *parent, string name, int value);
  //-------------------------//
  void virtual change(
      const int i); // บวก i แบบเรียบง่ายเอาไว้ให้ parameterMax inherit ไปใช้
  //-------------------------//
  inline void setValue(
      const int i);                    // ใช้เพื่อเปลี่ยนค่าเพราะ value เป็น private ของ statparamiter
                                       // ถึงจะ inherit ไปก็ไม่สามารถ acess ถึง value ตรงๆได้
  inline void setName(const string n); // เหตุผลเดียวกัน
  //-------------------------//
  inline int getValue();            // เหตุผลเดียวกัน
  inline string getName();          // เหตุผลเดียวกัน
  inline StatusSystem &getParent(); // เหตุผลเดียวกัน
};

class StatParameterMax : public StatParameter
{ // ติดลบไม่ได้แต่มีค่าสไม่เกินค่าที่กำหนด
private:
  int maxValue;

public:
  explicit StatParameterMax(StatusSystem *parent, string name, int value,
                            int maxValue);
  //-------------------------//
  void change(const int); // ถ้าเพิ่มหรือลดเกิน max ให้เป็น max
  //-------------------------//
  inline int getMax();
  inline void setMax(const int i);
};

class StatParameterCoord : public StatParameter
{
private:
  int xValue, yValue, xMAX, yMAX, xMIN, yMIN;

public:
  explicit StatParameterCoord(StatusSystem *parent, string name, int xValue,
                              int yValue, int xMAX, int yMAX, int xMIN,
                              int yMIN);
  //-------------------------//
  bool changeCoord(const int, const int); // ขยับไหม? ใส่มาอยู่ในระยะที่กำหนดไว้ไหม
  bool setCoord(const int, const int);    // ขยับไหม? ใส่มาอยู่ในระยะที่กำหนดไว้ไหม
  void setMax(const int x, const int y);
  void setMin(const int x, const int y);
  //-------------------------//
  inline int getX();
  inline int getY();
  inline int getXMax();
  inline int getYMax();
  inline int getXMin();
  inline int getYMin();
};

<<<<<<< Updated upstream
class Ability
{ // แล้วไอตัวนี้มันจะถูกทำลายตอน stat ถูกทำลาย
=======
class AbilitySystem {
protected:
  GameObject *parent;

public:
  bool isInAbility(string name);
  vector<Ability *> abilityGroup;
  AbilitySystem(GameObject *parent);
  bool addAbility(Ability *ability);
  void decisionMaking(int sur, int env, int repro);
  inline GameObject *getParent();
};

class AfflictionSystem {
protected:
  GameObject *parent;

public:
  vector<Affliction *> afflictionGroup;
  AfflictionSystem(GameObject *parent);
  void recalculateValue();
  bool addAffliction(Affliction *affliction);
  bool removeAffliction(string name);
  bool removeAllAffliction(string name);
  int amountOfAffliction(string name);
  void updateAffliction();
  GameObject *getParent();
  bool isInAffliction(string name);
};

class StatusBlock : public AbilitySystem, public AfflictionSystem {
protected:
  GameObject *parent;

public:
  vector<StatParam *> statParamGroup;
  StatusBlock(GameObject *parent);
  bool isInParam(string name);
  void resetValue();
  bool addStat(StatParam *statParam); // คำนวน aff value ใหม่ทั้งหมดด้วยหลังจากทำเสร็จ
  int getParamValueDefault(string name); // ถึงแม้จะไม่มีสแตดก็รีเทิร์น 0 ออกมา
  int getParamValue(string name); // ถึงแม้จะไม่มีสแตดก็รีเทิร์น 0 ออกมา
  StatParam *getParam(string name);
  inline GameObject *getParent();
};

class GameObject { // จะสร้างซักตัวก็ต้องสร้าง abilty, stat ของมันให้พร้อมนะ
protected:
  string name, represent;
  StatusBlock *stat;
  Layer *parent;

public:
  GameObject(Layer *parent, int x, int y);
  int getSur();
  int getEnv();
  int getRepo();
  void update(); // เล่นเทิร์น
  pair<int, int> getVectorIndex();
  pair<int, int> getCoord();
  inline void setName(string stringInput);
  void setRepresent(string stringInput);
  inline string getName();
  inline string getRepresent() { return represent; }
  inline StatusBlock *getStat();
  inline Layer *getParent();
  vector<GameObject *> findTargetInRange(int range, bool allLayers);
  vector<string> getDetail() { return {}; }
  // bool canAct(GameObject *);
};

class Layer {
>>>>>>> Stashed changes
private:
  string name;
  AbilitySystem &parent;

public:
  explicit Ability(AbilitySystem *parent, string name);
  //-------------------------//
  int virtual decision(const int, const int, const int)
      const = 0; // ออกมาเป็นค่า desition point ให้สัตว์เลือก ability
                 // ที่แต้มมากที่สุดไปใช้งานในเทิร์นนั้นๆ
  bool virtual action()
      const = 0; // ตีโดนไหม ถ้าถามว่าทำไม action มาอยู่ในนี้แล้วไม่มีฟังก์ชั่น update
                 // เพราะว่าเราจะสั่งให้คำสั่ง action ทำงานจากตัวของสัตว์ ไม่ใช่จากตัวของ
                 // ability เอง
  //-------------------------//
  inline void setName(const string n);
  //-------------------------//
  inline string getName();
  inline AbilitySystem &getParent();
};

class Affliction
{
private:
  string name;
  int duration, passedTime = 0;
  AfflictionSystem &parent;

  bool tick();               // ถ้า duration ลดลงแล้วเป็น 0 ให้ return false
                             // และฟังก์ชั่นนนี้สามรถใช้เพื่อลดเวลาการติดได้ด้วยโดยการให้มัน tick หลายๆรอบ
  void virtual action() = 0; // ทำอะไรเมื่อมีโรคโดยที่มันจะไป acess
                             // ค่าต่างๆของสัตว์และส่งผลก่อนที่สัตว์จะได้ตัดสินใจ

public:
  explicit Affliction(AfflictionSystem *parent, int duration, string name)
      : parent(*parent), duration(duration), name(name) {}
  //-------------------------//
  bool refresh();
  void
  update(); // ที่มีฟังก์ชั่น update เพราะว่าต้องการให้มันตรวจก่อนว่าเวลาที่โรคจะหมดยังไม่เท่ากับ 0
            // แล้วค่อยใช้ action
  //-------------------------//
  inline AfflictionSystem &getParent();
  inline void setName(const string n);
  inline void setDuration(const int d);
  //-------------------------//
  inline string getName();
  inline int getDuration();
};

class AbilitySystem
{
private:
  vector<Ability *> abilities; // สร้างเป็นเวกเตอร์ของ pointer เพื่อเวลาที่ Ability
                               // มันพัฒนา จะได้สามารถเปลี่ยนค่า pointer แทนที่ได้เลย
  GameObject *parent;

public:
  explicit AbilitySystem(GameObject *parent, vector<Ability *> abilities);
  //-------------------------//
  bool addAbility(Ability *);       // มีชื่อเดียวกันอยู่แล้วไหม
  bool removeAbility(const string); // มีให้ลบไหม
  bool isInAbility(const string);   // มีอยู่ไหม
  void decisionMakeing(const int, const int, const int) const;
  //-------------------------// ไม่ทำ set
  // เพราะมันดูไม่สมเหตุสมผลเลยที่จะย้ายโรคของคนอื่นมาเป็นของเราแทน
  inline GameObject *getParent();
  inline int getAbilitySize();
};

class AfflictionSystem
{
private:
  vector<Affliction *> afflictions;
  GameObject *parent;
  bool checkAlive(); // ตายยัง?

public:
  explicit AfflictionSystem(GameObject *parent,
                            vector<Affliction *> afflictions);
  //-------------------------//
  bool addAffliction(
      Affliction *);                      // มีอยู่แล้วไหม ถ้ามมีก็ใส่เพิ่มรีเทิร์นจริง ไม่มีก็ใส่เพิ่มเหมือนกันแต่รีเทิร์นเท็จ
  bool removeAffliction(string name);     // มีให้ลบไหม
  bool isInAffliction(const string name); // มีอยู่ไหม
  bool updateAffliction();                // ยังไม่ตายใช่ไหม
  //-------------------------// ไม่ทำ set เหตุผลเหมือนกับ AbilitySystem
  inline vector<Affliction *> &getAfflictions();
  inline GameObject *getParent();
  inline int getAfflictionSize();
};

class StatusSystem : public AbilitySystem, public AfflictionSystem
{
private:
  vector<StatParameterMax *>
      value; // GameObject บางแต่ละประเภทก็มี param ต่างกันไป จึงทำเช่นนี้
  StatParameterCoord positionParamiter;
  GameObject *parentParamiter; // ต้องเป็น pointer เพราะว่ามันสามารถถูกโอนได้

public:
  explicit StatusSystem(GameObject *parent, int x, int y, int xMAX, int yMAX,
                        int xMIN, int yMIN, vector<StatParameterMax *> *value,
                        vector<Ability *> *abilities,
                        vector<Affliction *> *afflictions);
  //-------------------------//
  bool isInStat(string name);   // เอาไว้เช็คว่าอยู่ใน value ไหม
  bool addStat(string name);    // ต้องชื่อไม่ซ้ำ เอาไว้เป็น StatParameterMax
  bool removeStat(string name); // มีอยู่ไหม
  void
  setParent(GameObject *p); // เปลี่ยน parent ของ stat ให้เป็น p โดยไม่สนอะไรทั้งนั้น
  //-------------------------//
  StatParameterMax *
  getValue(string name);                    // ใช้เพื่อเอา parameter object ที่มีชื่อตรงกันออกมา
  inline StatParameterCoord &getPosition(); // ใช้เพื่อเอาค่า x ออกมา
  inline int getPositonX();
  inline int getPositonY();
  inline GameObject *getParent();
};

// GameObject's parent is Layer btw, not LayerSystem.
// This means Gameobjects can't just move layers since parent can't be changed rn
class GameObject
{
private:
  string represent;
  string name;
  StatusSystem *stat; // stat ต้องเป็น pointer เพราะว่ามันสามาถเปลี่ยนเป็นตัวอื่นได้
  Layer *parent;      // stat ต้องเป็น pointer เพราะว่ามันสามาถเปลี่ยนย้าย layer ได้

public:
  explicit GameObject(Layer *, string, string, int, int, int, int, int, int,
                      vector<StatParameterMax *> *, vector<Ability *> *,
                      vector<Affliction *> *); // ต้องมี parent ใน stat ด้วย
  //-------------------------//
  void virtual update() = 0; // ให้ class ลูกไปเขียนเองเพราะบางอย่าง เช่น พื้นดิน
                             // ก็ไม่ต้องตัดสินใจเหมือนสัตว์
  //-------------------------//
  inline void setName(const string n);
  inline void setRepresent(const string r);
  void setStat(StatusSystem *statInput); // ไปแย่งเอา stat ของตัวอื่นมาใช้
                                         // จะเอามาใช้สำหรับกรณีสัตว์วิวัฒนาการ
  void setParent(Layer *p);              // เปลี่ยนแบบไม่สนว่าที่ตรงนั้นจะไปทับใครใน layer เดียวกันไหม
  //-------------------------//
  string getRepresent();
  string getName();
  StatusSystem &getStat();
  Layer *getParent();
  //-------------------------//
  int getX();
  int getY();
};

class Land : public GameObject
{
private:
  int x, y;

public:
  explicit Land(Layer *parentInput, string representInput,
                string nameInput = "", int xValueInput = 0,
                int yValueInput = 0, int xMaxInput = 0,
                int yMaxInput = 0, int xMinInput = 0, int yMinInput = 0,
                vector<StatParameterMax *> *value = {},
                vector<Ability *> *abilities = {},
                vector<Affliction *> *afflictions = {});
  void update();
};

class Layer
{ // ต้องทำการเรียงให้มีตำแหน่งติดลบด้วย
private:
  string name;
  vector<vector<GameObject *>> layer;
  GameObject *default_value;
  LayerSystem &parent;

public:
  explicit Layer(LayerSystem *parent, int width, int height, string name, bool ground);
  //-------------------------//
  void action();
  //-------------------------//
  inline void setName(const string n);
  //-------------------------//
  inline string getName();
  inline LayerSystem &getParent();
  void setLayer(int layer_num)
  {
  }
};
/*
Layer notes:
- addGameObject is going to be moved to layer system,
  since coordinates need to refer to the ground layer
  and size needs to be initialized by layer system
- removeGameObject as well
*/

class LayerSystem
{
private:
  int width;
  int height;
  vector<Layer *> layers;

public:
  explicit LayerSystem(int width, int height, int amount);
  //-------------------------//
  bool createNewLayer(string name, bool ground); // ตรวจสอบชื่อซ้ำเพื่อไม่ให้มีชื่อซ้ำ
  bool removeLayer(string name);    // มี layer นั้นๆให้ลบไหม
  //-------------------------//
  inline Layer *getLayer(string name);
  inline Layer *getLayer(int i);
  vector<Layer *> *getLayers();
  inline string getLayerName(int i);
  inline int getLayerSize();
};

// no
class Biome : public LayerSystem
{
private:
public:
  explicit Biome(vector<Layer *>);
  //-------------------------//
  bool changeLayer(GameObject *, string, string); // ไปทับใครไหม
  void update();
};

// //------------------------------------------------------------------------------//
// class LivingThing : public GameObject {
// private:
//   int needToEnvironment, needToSurvive, needToReproduce;
//   void virtual calculateNeeds() = 0;
//   bool virtual evolve() = 0; // evolve รึป่าว
//   void virtual decision() = 0;

// public:
//   explicit LivingThing(Layer *, string, string, int, int, int, int, int, int,
//                        int, int, int, int, int, int, int, int, int, int);
//   // รอแก้ constructor
//   int getNeedToSurvive() { return needToSurvive; }
//   int getNeedToEnvironment() { return needToEnvironment; }
//   int getNeedToReproduce() { return needToReproduce; }
//   void changeNeed(const int sur, const int env, const int rep) {
//     needToEnvironment += env;
//     needToSurvive += sur;
//     needToReproduce += rep;
//   }
// };

// class Animal : public LivingThing {
// private:
//   void virtual calculateNeeds() = 0; // เวลา inherit เพื่อสร้างสัตว์ชนิดใหม่ก็ไป
//                                      // override
//   bool virtual evolve() = 0; // เวลา inherit เพื่อสร้างสัตว์ชนิดใหม่ก็ไป override
//   void virtual decision() = 0; // เวลา inherit เพื่อสร้างสัตว์ชนิดใหม่ก็ไป override

// public:
//   explicit Animal(Layer *, string, string, int, int, int, int, int, int, int,
//                   int, int, int, int, int, int, int, int, int);
//   // รอแก้ constructor
//   void update(); // ที่ไม่ต้องเขียน override เพราะมันเป็น abstract method อยู่แล้ว
// };

// class Plant : public LivingThing {
// private:
//   void virtual calculateNeeds();
//   bool virtual evolve();
//   void virtual decision();

// public:
//   explicit Plant(Layer *, string, string, int, int, int, int, int, int, int,
//                  int, int, int, int, int, int, int, int, int);
//   // รอแก้ constructor
//   void update(); // ที่ไม่ต้องเขียน override เพราะมันเป็น abstract method อยู่แล้ว
// };

// class NonLivingThing : public GameObject {
// private:
//   // รอแก้ constructor
//   void virtual evolve() = 0;

// public:
//   explicit NonLivingThing(Layer *, string, string, int, int, int, int, int, int,
//                           int, int, int, int, int, int, int, int, int, int);
//   void update(); // ที่ไม่ต้องเขียน override เพราะมันเป็น abstract method อยู่แล้ว
// };

// class Field : public NonLivingThing {
// private:
//   string name;

// public:
//   explicit Field(Layer *, string, string, int, int, int, int, int, int, int,
//                  int, int, int, int, int, int, int, int, int);
//   bool stayAble();
//   string getname() { return name; }
// };

#endif