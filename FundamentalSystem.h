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
class Feild;

#ifndef FUNDAMENTALSYSTEM_H
#define FUNDAMENTALSYSTEM_H

class StatParameter { // เป็น paramiter ที่ไม่สามารถติดลบได้
private:
  string name; // มี Name เพราะว่าเวลาที่เราต้องการเรียกใช้เราต้องการเรียกด้วยชื่อ
  int value;
  StatusSystem *parent; // เป็นประเภทที่ไม่สามารถเปลี่ยน parent ได้

public:
  explicit StatParameter(StatusSystem *parent, string name, int value);
  //-------------------------//
  void virtual change(
      const int i); // บวก i แบบเรียบง่ายเอาไว้ให้ parameterMax inherit ไปใช้
  //-------------------------//
  inline void setValue(
      const int i); // ใช้เพื่อเปลี่ยนค่าเพราะ value เป็น private ของ statparamiter
                    // ถึงจะ inherit ไปก็ไม่สามารถ acess ถึง value ตรงๆได้
  inline void setName(const string n); // เหตุผลเดียวกัน
  //-------------------------//
  inline int getValue();   // เหตุผลเดียวกัน
  inline string getName(); // เหตุผลเดียวกัน
  inline StatusSystem *getParent(); // เหตุผลเดียวกัน
};

class StatParameterMax : public StatParameter { // ติดลบไม่ได้แต่มีค่าสไม่เกินค่าที่กำหนด
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

class StatParameterCoord : public StatParameter {
private:
  int xValue, yValue, xMAX, yMAX, xMIN, yMIN;

public:
  explicit StatParameterCoord(StatusSystem *parent, string name, int xValue,
                              int yValue, int xMAX, int yMAX, int xMIN,
                              int yMIN);
  //-------------------------//
  bool changeCoord(const int, const int); // ขยับไหม? ใส่มาอยู่ในระยะที่กำหนดไว้ไหม
  bool setCoord(const int, const int); // ขยับไหม? ใส่มาอยู่ในระยะที่กำหนดไว้ไหม
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

class Ability { // แล้วไอตัวนี้มันจะถูกทำลายตอน stat ถูกทำลาย
private:
  string name;
  AbilitySystem *parent;

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
  bool virtual canAction() = 0; // เช็คเงื่อนไขว่าสามารถทำ action นั้นๆได้ไหม
  //-------------------------//
  inline void setName(const string n);
  //-------------------------//
  inline string getName();
  inline AbilitySystem *getParent();
};

class Affliction {
private:
  string name;
  int duration, passedTime = 0;
  AfflictionSystem *parent;

  bool tick(); // ถ้า duration ลดลงแล้วเป็น 0 ให้ return false
               // และฟังก์ชั่นนนี้สามรถใช้เพื่อลดเวลาการติดได้ด้วยโดยการให้มัน tick หลายๆรอบ
  void virtual action() = 0; // ทำอะไรเมื่อมีโรคโดยที่มันจะไป acess
                             // ค่าต่างๆของสัตว์และส่งผลก่อนที่สัตว์จะได้ตัดสินใจ

public:
  explicit Affliction(AfflictionSystem *parent, int duration, string name)
      : parent(parent), duration(duration), name(name) {}
  //-------------------------//
  bool refresh();
  void
  update(); // ที่มีฟังก์ชั่น update เพราะว่าต้องการให้มันตรวจก่อนว่าเวลาที่โรคจะหมดยังไม่เท่ากับ 0
            // แล้วค่อยใช้ action
  //-------------------------//
  inline AfflictionSystem *getParent();
  inline void setName(const string n);
  inline void setDuration(const int d);
  //-------------------------//
  inline string getName();
  inline int getDuration();
};

class AbilitySystem {
private:
  vector<Ability *> abilities; // สร้างเป็นเวกเตอร์ของ pointer เพื่อเวลาที่ Ability
                               // มันพัฒนา จะได้สามารถเปลี่ยนค่า pointer แทนที่ได้เลย
  GameObject *parent;

public:
  explicit AbilitySystem(GameObject *parent, vector<Ability *> abilities);
  //-------------------------//
  bool addAbility(Ability *); // มีชื่อเดียวกันอยู่แล้วไหม
  bool removeAbility(const string); // มีให้ลบไหม
  bool isInAbility(const string);   // มีอยู่ไหม
  void decisionMakeing(const int, const int, const int) const;
  //-------------------------// ไม่ทำ set
  // เพราะมันดูไม่สมเหตุสมผลเลยที่จะย้ายโรคของคนอื่นมาเป็นของเราแทน
  inline GameObject *getParent();
  inline int getAbilitySize();
};

class AfflictionSystem {
private:
  vector<Affliction *> afflictions;
  GameObject *parent;
  bool checkAlive(); // ตายยัง?

public:
  explicit AfflictionSystem(GameObject *parent,
                            vector<Affliction *> afflictions);
  //-------------------------//
  bool addAffliction(
      Affliction *); // มีอยู่แล้วไหม ถ้ามมีก็ใส่เพิ่มรีเทิร์นจริง ไม่มีก็ใส่เพิ่มเหมือนกันแต่รีเทิร์นเท็จ
  bool removeAffliction(string name);     // มีให้ลบไหม
  bool isInAffliction(const string name); // มีอยู่ไหม
  bool updateAffliction(); // ยังไม่ตายใช่ไหม
  //-------------------------// ไม่ทำ set เหตุผลเหมือนกับ AbilitySystem
  inline vector<Affliction *> *getAfflictions();
  inline GameObject *getParent();
  inline int getAfflictionSize();
};

class StatusSystem : public AbilitySystem, public AfflictionSystem {
private:
  vector<StatParameter *>
      value; // GameObject บางแต่ละประเภทก็มี param ต่างกันไป จึงทำเช่นนี้
  StatParameterCoord positionParamiter;
  GameObject *parentParamiter; // ต้องเป็น pointer เพราะว่ามันสามารถถูกโอนได้

public:
  explicit StatusSystem(GameObject *parent, int x, int y, int xMAX, int yMAX,
                        int xMIN, int yMIN, vector<StatParameter *> *value,
                        vector<Ability *> *abilities,
                        vector<Affliction *> *afflictions);
  //-------------------------//
  bool isInStat(string name); // เอาไว้เช็คว่าอยู่ใน value ไหม
  bool addStat(string name); // ต้องชื่อไม่ซ้ำ เอาไว้เป็น StatParameterMax
  bool removeStat(string name); // มีอยู่ไหม
  void
  setParent(GameObject *p); // เปลี่ยน parent ของ stat ให้เป็น p โดยไม่สนอะไรทั้งนั้น
  //-------------------------//
  StatParameter *
  getValue(string name); // ใช้เพื่อเอา parameter object ที่มีชื่อตรงกันออกมา
  inline StatParameterCoord *getPosition(); // ใช้เพื่อเอาค่า x ออกมา
  inline int getPositonX();
  inline int getPositonY();
  inline GameObject *getParent();
};

// GameObject's parent is Layer btw, not LayerSystem.
// This means Gameobjects can't just move layers since parent can't be changed
// rn
class GameObject {
private:
  string represent;
  string name;
  StatusSystem *stat; // stat ต้องเป็น pointer เพราะว่ามันสามาถเปลี่ยนเป็นตัวอื่นได้
  Layer *parent; // stat ต้องเป็น pointer เพราะว่ามันสามาถเปลี่ยนย้าย layer ได้

public:
  explicit GameObject(Layer *, string, string, int, int, int, int, int, int,
                      vector<StatParameter *> *, vector<Ability *> *,
                      vector<Affliction *> *); // ต้องมี parent ใน stat ด้วย
  //-------------------------//
  void virtual update() = 0; // ให้ class ลูกไปเขียนเองเพราะบางอย่าง เช่น พื้นดิน
                             // ก็ไม่ต้องตัดสินใจเหมือนสัตว์
  //-------------------------//
  inline void setName(const string n);
  inline void setRepresent(const string r);
  void setStat(StatusSystem *statInput); // ไปแย่งเอา stat ของตัวอื่นมาใช้
                                         // จะเอามาใช้สำหรับกรณีสัตว์วิวัฒนาการ
  void setParent(Layer *p); // เปลี่ยนแบบไม่สนว่าที่ตรงนั้นจะไปทับใครใน layer เดียวกันไหม
  //-------------------------//
  string getRepresent();
  string getName();
  StatusSystem *getStat();
  Layer *getParent();
  //-------------------------//
  int getX();
  int getY();
};

class Feild : public GameObject {
public:
  explicit Feild(Layer *parentInput, string representInput, string nameInput,
                 int xValueInput, int yValueInput, int xMaxInput, int yMaxInput,
                 int xMinInput, int yMinInput,
                 vector<StatParameter *> *value,
                 vector<Ability *> *abilities,
                 vector<Affliction *> *afflictions);
};

class Land : public Feild {
private:
  int x, y;

public:
  explicit Land(Layer *parentInput, string representInput, string nameInput,
                int xValueInput, int yValueInput, int xMaxInput, int yMaxInput,
                int xMinInput, int yMinInput, vector<StatParameter *> *value,
                vector<Ability *> *abilities,
                vector<Affliction *> *afflictions);
  void update();
};

class Layer { // ต้องทำการเรียงให้มีตำแหน่งติดลบด้วย
private:
  string name;
  vector<vector<GameObject *>> layer;
  GameObject *default_value;
  LayerSystem *parent;

public:
  explicit Layer(LayerSystem *parent, int width, int height, string name,
                 bool ground);
  //-------------------------//
  void action();
  //-------------------------//
  inline void setName(const string n);
  //-------------------------//
  inline string getName();
  inline LayerSystem *getParent();
  void setLayer(int layer_num) {}
};
/*
Layer notes:
- addGameObject is going to be moved to layer system,
  since coordinates need to refer to the ground layer
  and size needs to be initialized by layer system
- removeGameObject as well
*/

class LayerSystem {
private:
  int width;
  int height;
  vector<Layer *> layers;

public:
  explicit LayerSystem(int width, int height, int amount);
  //-------------------------//
  bool createNewLayer(string name, bool ground); // ตรวจสอบชื่อซ้ำเพื่อไม่ให้มีชื่อซ้ำ
  bool removeLayer(string name); // มี layer นั้นๆให้ลบไหม
  //-------------------------//
  inline Layer *getLayer(string name);
  inline Layer *getLayer(int i);
  vector<Layer *> *getLayers();
  inline string getLayerName(int i);
  inline int getLayerSize();
};

// no
class Biome : public LayerSystem {
private:
public:
  explicit Biome(vector<Layer *>);
  //-------------------------//
  bool changeLayer(GameObject *, string, string); // ไปทับใครไหม
  void update();
};

#endif