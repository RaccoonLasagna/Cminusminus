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
class Environment;
class LivingThing;
class Animal;
class Plant;
class NonLivingThing;
class Field;

#ifndef FUNDAMENTALSYSTEM_H
#define FUNDAMENTALSYSTEM_H

class StatParameter {
private:
  int value;
  StatusSystem &parent;

public:
  explicit StatParameter(StatusSystem *parent,
                         int value = 0) // รับ Stat มาเป็น pointer แล้ว dereference
                                        // จากนั้นเอามาสร้างเป็น reference.
      : parent(*parent), value(value) {}
  inline int getValue() const { return value; }
  inline StatusSystem &getParent() const { return parent; }
  inline void setValue(const int i) { value = i; }
  void increse(const int i) { value += i; };
  void decrese(const int i) { value -= i; };
};

class StatParameterMax : public StatParameter {
private:
  int maxValue;

public:
  explicit StatParameterMax(StatusSystem *parent, int value = 0,
                            int maxValue = 0)
      : maxValue(maxValue), StatParameter(parent, value) {}
  void increse(const int);
  void decrese(const int);
  inline int getMax() const { return maxValue; }
  inline void setMax(const int i) { maxValue = i; };
};

class StatParameterCoord : public StatParameter {
private:
  int xValue, yValue, xMAX, yMAX, xMIN, yMIN;

public:
  explicit StatParameterCoord(StatusSystem *parent, int xValue = 0,
                              int yValue = 0, int xMAX = 0, int yMAX = 0,
                              int xMIN = 0, int yMIN = 0)
      : xValue(xValue), yValue(yValue), xMAX(xMAX), yMAX(yMAX), xMIN(xMIN),
        yMIN(yMIN), StatParameter(parent) {}
  bool changeCoord(const int, const int); // ขยับไหม
  bool setCoord(const int, const int);    // ขยับไหม
  void setMax(const int x, const int y) { xMAX = x, yMAX = y; }
  void setMin(const int x, const int y) { xMIN = x, yMIN = y; }
  inline int getX() { return xValue; }
  inline int getY() { return yValue; }
  inline int getXMax() { return xMAX; }
  inline int getYMax() { return yMAX; }
  inline int getXMin() { return xMIN; }
  inline int getYMin() { return yMIN; }
};

class Ability { // แล้วไอตัวนี้มันจะถูกทำลายตอน stat ถูกทำลาย
private:
  string name;
  AbilitySystem &parent;

public:
  explicit Ability(AbilitySystem *parent, string name = "")
      : parent(*parent), name(name) {}
  int virtual decision(const int, const int, const int) const = 0;
  bool virtual action() const = 0; // ตีโดนไหม
  inline string getName() { return name; }
  inline AbilitySystem &getParent() { return parent; }
  inline void setName(const string n) { name = n; }
};

class AbilitySystem {
private:
  vector<Ability *> abilities; // สร้างเป็นเวกเตอร์ของ pointer เพื่อเวลาที่ Ability
                               // มันพัฒนา จะได้สสามารถเปลี่ยนค่า pointer แทนที่ได้เลย
  GameObject *parent;

public:
  explicit AbilitySystem(GameObject *parent, vector<Ability *> abilities = {})
      : parent(parent), abilities(abilities){};
  bool addAbility(Ability *);
  bool removeAbility(const string);
  void decisionMakeing(const int, const int, const int) const;
  inline int getAbilitySize() const { return abilities.size(); }
  inline GameObject *getParent() const { return parent; }
};

class Affliction {
private:
  string name;
  int duration;
  AfflictionSystem &parent;
  bool tick(); // ถ้า duration ลดลงแล้วเป็น 0 ให้ return false
  void virtual action() = 0;

public:
  explicit Affliction(AfflictionSystem *parent, int duration = 0,
                      string name = "")
      : parent(*parent), duration(duration), name(name) {}
  void update();
  inline string getName() const { return name; }
  inline int getDuration() const { return duration; }
  inline AfflictionSystem &getParent() { return parent; }
  inline void setName(const string n) { name = n; }
};

class AfflictionSystem {
private:
  vector<Affliction *> afflictions;
  GameObject *parent;
  bool checkAlive(); // ต้องมีฟังก์ชั่นสำหรับเปิดดูและหาใน afflictions ด้วย

public:
  // bool isInAffliction(const string) const;
  explicit AfflictionSystem(GameObject *parent,
                            vector<Affliction *> afflictions = {})
      : parent(parent), afflictions(afflictions){};
  bool addAffliction(Affliction *);
  bool removeAffliction(string name);
  bool updateAffliction(); // ยังไม่ตายใช่ไหม
  inline vector<Affliction *> getAfflictions() const { return afflictions; }
  inline GameObject *getParent() { return parent; }
  inline int getAfflictionSize() const { return afflictions.size(); }
  bool isInAffliction(const string name) {
    for (Affliction *a : afflictions) {
      if (a->getName() == name) {
        return true;
      }
    }
    return false;
  }
};

class StatusSystem : public AbilitySystem, public AfflictionSystem {
private:
  StatParameterMax health, stamina, hunger, vision, lifetime;
  StatParameterCoord position;
  GameObject *parent; // ต้องเป็น pointer เพราะว่ามันสามารถถูกโอนได้

public:
  explicit StatusSystem(GameObject *parent)
      : health(this), stamina(this), hunger(this), vision(this), lifetime(this),
        position(this), AbilitySystem(parent), AfflictionSystem(parent){};
  // รอแก้ constructor
  inline int getHealth() { return health.getValue(); }
  inline int getStamina() { return stamina.getValue(); }
  inline int getHunger() { return hunger.getValue(); }
  inline int getVision() { return vision.getValue(); }
  inline int getLifetime() { return lifetime.getValue(); }
  inline int getPositonX() { return position.getX(); }
  inline int getPositonY() { return position.getY(); }
  inline GameObject *getParent() { return parent; }
  void setParent(GameObject *p) { parent = p; }
};

class GameObject {
private:
  string name;
  StatusSystem *stat; // stat ต้องเป็น pointer เพราะว่ามันสามาถเปลี่ยนเป็นตัวอื่นได้
  string represent;
  Layer *parent; // stat ต้องเป็น pointer เพราะว่ามันสามาถเปลี่ยนย้าย layer ได้

public:
  explicit GameObject(Layer *parent, string represent = "", string name = "")
      : parent(parent), represent(represent), name(name) {}
  // รอแก้ constructor
  StatusSystem *getStat() { return stat; }
  string getRepresent() { return represent; }
  Layer *getParent() { return parent; }
  string getName() { return name; }
  inline void setName(const string n) { name = n; }
  void setStat(StatusSystem * statInput); // อย่าลืมแก้ parent ใน stat ด้วย
  inline void setRepresent(const string r) { represent = r; }
  void setParent(Layer *p) { parent = p; }
  void virtual update() = 0;
};

class Layer {
private:
  vector<vector<GameObject *>> layer;
  string name;
  Environment &parent;

public:
  explicit Layer(Environment *parent, vector<vector<GameObject *>> layer = {},
                 string name = "")
      : parent(*parent), layer(layer), name(name) {}
  void addGameObject(GameObject *object, int x,
                     int y); // ต้องมี parent เป็น nullptr
  void removeGameObject(int x,
                        int y); // ต้องทำให้ parent ของ object กลับมาเป็น nullptr
  void action();
  inline string getName() { return name; }
  inline Environment &getParent() { return parent; }
  inline void setName(const string n) { name = n; }
};

class Layers {
private:
  vector<Layer> layers;

public:
  explicit Layers(vector<Layer> layers = {}) : layers(layers) {}
  inline Layer getLayer(int i) { return layers.at(i); };
  inline string getLayerName(int i) { return layers.at(i).getName(); };
  bool createNewLayer(string name); // ตรวจสอบชื่อซ้ำเพื่อไม่ให้มีชื่อซ้ำ
  bool removeLayer(string name); // มี layer นั้นๆให้ลบไหม
  // inline Environment &getParent() { return parent; }
};

class Environment : public Layers {
private:
public:
  explicit Environment(vector<Layer> * = {});
  void changeLayer(GameObject*, string, string);
  void update();
};

class LivingThing : public GameObject {
private:
  int needToEnvironment, needToSurvive, needToReproduce;
  void virtual calculateNeeds() = 0;
  bool virtual evolve() = 0; // evolve รึป่าว
  void virtual decision() = 0;

public:
  explicit LivingThing(Layer *parent, string represent = "", string name = "")
      : GameObject(parent, represent, name), needToEnvironment(0),
        needToSurvive(0), needToReproduce(0) {}
  // รอแก้ constructor
  int getNeedToSurvive() { return needToSurvive; }
  int getNeedToEnvironment() { return needToEnvironment; }
  int getNeedToReproduce() { return needToReproduce; }
  void changeNeed(const int sur, const int env, const int rep) {
    needToEnvironment += env;
    needToSurvive += sur;
    needToReproduce += rep;
  }
};

class Animal : public LivingThing {
private:
  void virtual calculateNeeds() = 0; // เวลา inherit เพื่อสร้างสัตว์ชนิดใหม่ก็ไป
                                     // override
  bool virtual evolve() = 0; // เวลา inherit เพื่อสร้างสัตว์ชนิดใหม่ก็ไป override
  void virtual decision() = 0; // เวลา inherit เพื่อสร้างสัตว์ชนิดใหม่ก็ไป override

public:
  explicit Animal(Layer *parent, string represent = "", string name = "")
      : LivingThing(parent, represent, name) {}
  // รอแก้ constructor
  void update(); // ที่ไม่ต้องเขียน override เพราะมันเป็น abstract method อยู่แล้ว
};

class Plant : public LivingThing {
private:
  void virtual calculateNeeds();
  bool virtual evolve();
  void virtual decision();

public:
  explicit Plant(Layer *parent, string represent = "", string name = "")
      : LivingThing(parent, represent, name) {}
  // รอแก้ constructor
  void update(); // ที่ไม่ต้องเขียน override เพราะมันเป็น abstract method อยู่แล้ว
};

class NonLivingThing : public GameObject {
private:
  explicit NonLivingThing(Layer *parent, string represent = "",
                          string name = "")
      : GameObject(parent, represent, name) {}
  // รอแก้ constructor
  void virtual evolve() = 0;

public:
  void update(); // ที่ไม่ต้องเขียน override เพราะมันเป็น abstract method อยู่แล้ว
};

class Field : public NonLivingThing {
private:
  string name;

public:
  bool stayAble();
  string getname() { return name; }
};

#endif