#include <string>
#include <vector>
using namespace std;

class StatParam;
class StatParamMax;
class StatParamCoord;
class Ability;
class AbilitySlot;
class Affliction;
class AfflictionSlot;
class Stat;
class GameObject;
class Layer;
class Layers;
class Environment;
class LivingThing;
class Animal;
class Plant;
class NonLivingThing;
class Field;

#ifndef FUNDAMENTALSYSTEM_H
#define FUNDAMENTALSYSTEM_H

class StatParam {
private:
  int value;
  Stat &parent;

public:
  explicit StatParam(Stat *parent,
                     int value = 0) // รับ Stat มาเป็น pointer แล้ว dereference
                                    // จากนั้นเอามาสร้างเป็น reference.
      : parent(*parent), value(value) {}
  inline int getValue() const { return value; }
  inline Stat &getParent() const { return parent; }
  inline void setValue(const int i) { value = i; }
};

class StatParamMax : public StatParam {
private:
  int maxValue;

public:
  explicit StatParamMax(Stat *parent, int value = 0, int maxValue = 0)
      : maxValue(maxValue), StatParam(parent, value) {}
  void increse(const int);
  void decrese(const int);
  inline int getMax() const { return maxValue; }
  inline void setMax(const int i) { maxValue = i; };
};

class StatParamCoord : public StatParam {
private:
  int xValue, yValue, xMAX, yMAX, xMIN, yMIN;

public:
  explicit StatParamCoord(Stat *parent, int xValue = 0, int yValue = 0,
                          int xMAX = 0, int yMAX = 0, int xMIN = 0,
                          int yMIN = 0)
      : xValue(xValue), yValue(yValue), xMAX(xMAX), yMAX(yMAX), xMIN(xMIN),
        yMIN(yMIN), StatParam(parent) {}
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
  AbilitySlot &parent;

public:
  explicit Ability(AbilitySlot *parent, string name = "")
      : parent(*parent), name(name) {}
  int virtual decision() const = 0;
  bool virtual action() const = 0; // ตีโดนไหม
  inline string getName() { return name; }
  inline AbilitySlot &getParent() { return parent; }
  inline void setName(const string n) { name = n; }
};

class AbilitySlot {
private:
  vector<Ability *> abilities; // สร้างเป็นเวกเตอร์ของ pointer เพื่อเวลาที่ Ability
                               // มันพัฒนา จะได้สสามารถเปลี่ยนค่า pointer แทนที่ได้เลย
  Stat &parent;

public:
  explicit AbilitySlot(Stat *parent, vector<Ability *> abilities = {})
      : parent(*parent), abilities(abilities){};
  void addAbility(const Ability *);
  bool removeAbility(const Ability *); // ได้ลบออกไหม มีตั้งแต่แรกไหม
  void decisionMakeing(const int, const int, const int) const;
  inline int getAbilitySize() const { return abilities.size(); }
  inline Stat &getParent() const { return parent; }
};

class Affliction {
private:
  string name;
  int duration;
  AfflictionSlot &parent;

public:
  explicit Affliction(AfflictionSlot *parent, int duration = 0,
                      string name = "")
      : parent(*parent), duration(duration), name(name) {}
  void virtual action();
  inline string getName() const { return name; }
  inline int getDuration() const { return duration; }
  inline AfflictionSlot &getParent() { return parent; }
  inline void setName(const string n) { name = n; }
};

class AfflictionSlot {
private:
  vector<Affliction *> afflictions;
  Stat &parent;

public:
  explicit AfflictionSlot(Stat *parent, vector<Affliction *> afflictions = {})
      : parent(*parent), afflictions(afflictions){};
  // ต้องมีฟังก์ชั่นสำหรับเปิดดูและหาใน afflictions ด้วย
  void addAffliction(Affliction *);
  void removeAffliction(Affliction *);
  bool updateAffliction(); // ยังไม่ตายใช่ไหม
  inline vector<Affliction *> getAfflictions() const { return afflictions; }
  inline Stat &getParent() { return parent; }
  inline int getAfflictionSize() const { return afflictions.size(); }
  bool isInAffliction(const string) const;
};

class Stat {
private:
  StatParamMax health, stamina, hunger, vision, lifetime;
  StatParamCoord position;
  AbilitySlot ability;
  AfflictionSlot affliction;
  GameObject *parent; // ต้องเป็น pointer เพราะว่ามันสามารถถูกโอนได้

public:
  explicit Stat(GameObject *parent)
      : parent(parent), health(this), stamina(this), hunger(this), vision(this),
        lifetime(this), position(this), ability(this), Affliction(this){};
  inline int getHealth() { return health.getValue(); }
  inline int getStamina() { return stamina.getValue(); }
  inline int getHunger() { return hunger.getValue(); }
  inline int getVision() { return vision.getValue(); }
  inline int getLifetime() { return lifetime.getValue(); }
  inline int getPositonX() { return position.getX(); }
  inline int getPositonY() { return position.getY(); }
  inline AbilitySlot &getAbility() { return ability; }
  inline AfflictionSlot &getAffliction() { return affliction; }
  inline GameObject *getParent() { return parent; }
  void setParent(GameObject *p) { parent = p; }
};

class GameObject {
private:
  string name;
  Stat stat;
  string represent;
  Layer *parent;

public:
  explicit GameObject(Layer *parent, string represent = "", string name = "")
      : parent(parent), stat(this), represent(represent), name(name) {}
  Stat &getStat() { return stat; }
  string getRepresent() { return represent; }
  Layer *getParent() { return parent; }
  string getName() { return name; }
  inline void setName(const string n) { name = n; }
  void setStat(const Stat *s) {} // อย่าลืมแก้ parent ใน stat ด้วย
  inline void setRepresent(const string r) { represent = r; }
  void setParent(Layer *p) { parent = p; }
  void virtual update() = 0;
};

class Layer {
private:
  vector<vector<GameObject *>> layer;
  string name;
  Layers &parent;

public:
  explicit Layer(Layers *parent, vector<vector<GameObject *>> layer = {},
                 string name = "")
      : parent(*parent), layer(layer), name(name) {}
  void addGameObject(GameObject *object, int x,
                     int y); // ต้องมี parent เป็น nullptr
  void removeGameObject(int x,
                        int y); // ต้องทำให้ parent ของ object กลับมาเป็น nullptr
  void action();
  inline string getName() { return name; }
  inline Layers &getParent() { return parent; }
  inline void setName(const string n) { name = n; }
};

class Layers {
private:
  vector<Layer> layers;
  Environment &parent;

public:
  Layers(Environment *parent, vector<Layer> layers = {})
      : parent(*parent), layers(layers) {}
  inline Layer getLayer(int i) { return layers.at(i); };
  inline string getLayerName(int i) { return layers.at(i).getName(); };
  bool createNewLayer(string name); // ตรวจสอบชื่อซ้ำเพื่อไม่ให้มีชื่อซ้ำ
  bool removeLayer(string name); // มี layer นั้นๆให้ลบไหม
  inline Environment &getParent() { return parent; }
};

class Environment {
private:
  Layers layers;
  // vector<string>  frame;
  void printFrame(int width, int height);

public:
  explicit Environment(Layers layers = {})
      : layers(layers) {} // ตรงนี้เกิดอะไรขึ้นทำไมมีปัญหา
  void update();
  inline Layers &getLayers() { return layers; }
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
  void update(); // ที่ไม่ต้องเขียน override เพราะมันเป็น abstract method อยู่แล้ว
};

class NonLivingThing : public GameObject {
private:
  explicit NonLivingThing(Layer *parent, string represent = "",
                          string name = "")
      : GameObject(parent, represent, name) {}
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