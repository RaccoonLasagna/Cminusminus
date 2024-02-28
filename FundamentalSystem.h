#include "Interpolate.h"
#include <array>
#include <cmath>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class StatParam;
class StatParamMax;
class Ability;
class AbilitySystem;
class Affliction;
class AfflictionSystem;
class StatusBlock;
class GameObject;
class Layer;
class LayerSystem;
class Command;
class Feild;

#ifndef FUNDAMENTALSYSTEM_H
#define FUNDAMENTALSYSTEM_H

class StatParam
{
protected:
  static const string name;     // ไปกำหนดเองตอนสร้าง classs
  static const int targetValue; // ไปกำหนดเองตอนสร้าง class
  vector<pair<Ability *, int> *> stackInfo;
  int rawValue, value;
  Ability *createBy;
  StatusBlock *parent;
  void virtual action() = 0; // ส่งผลกับตัวเองยังไง
  bool isAction(int fristScope, int secondScope);
  void updateAddBy();

public:
  StatParam(StatusBlock *parent, Ability *createBy, int rawValue);
  StatParam(GameObject *target, Ability *createBy, int rawValue);
  void virtual setRawTo(int i);                     // คนใช้คือ Ability
  bool virtual changeValueBy(int i);                // คนใช้คือ Affliction
  void pushStackInfo(pair<Ability *, int> *target); // คนใช้คือ Ability
  //----------------------------//
  void resetValue() { value = rawValue; }
  inline string getName() { return name; }
  inline int getRawValue() { return rawValue; }
  inline int getValue() { return value; }
  inline vector<pair<Ability *, int> *> getStackInfo() { return stackInfo; }
  inline Ability *getCreateBy() { return createBy; }
  inline StatusBlock *getParent() { return parent; }
};

class Ability
{ // statParam ทั้งหมดที่ Ability จะใช้ต้องถูกสร้างโดยตัว Ability เองทั้งหมด
protected:
  static const string name; // ไปกำหนดเองตอนสร้าง classs
  static Var3DGraph data;   // ไปกำหนดเองตอนสร้าง classs
  AbilitySystem *parent;
  void virtual passiveAction(vector<GameObject *> *) = 0;
  bool virtual canActiveAction(vector<GameObject *> *) = 0;
  vector<GameObject *> virtual *findTargetForPassive() = 0;
  void virtual produceAffliction() = 0; // สร้าง aff ใส่ศรัทตรูพร้อมคำนวน %
                                        // ที่ศรัทตรูจะโดนด้วย
  vector<StatParam *> virtual *
  createStatParam(StatusBlock *parent,
                  Ability *owner); // สร้าง Instant Class StatParam
                                   // ตามประเภทที่กำหนด อันนี้ไป override ด้วยนะ

public:
  Ability(AbilitySystem *parent);
  Ability(GameObject *target);
  vector<GameObject *> virtual *findTargetForActive() = 0;
  void virtual activeAction(vector<GameObject *> *) = 0;
  int decision(int sur, int env, int repro);
  inline string getName() { return name; }
  inline AbilitySystem *getParent() { return parent; }
};

class Affliction
{
protected:
  static const string name, targetValue; // ไปกำหนดเองตอนสร้าง class
  static const Var3DGraph data;          // ไปกำหนดเองตอนสร้าง classs
  static const int duration,
      valueIncrese;      // ไปกำหนดเองตอนสร้าง classs valueIncrese ถ้าเป็น 0
                         // คือการทำงานประเภทครั้งเดียว
  static bool permanant; // ไปกำหนดเองตอนสร้าง classs
  int passedTime, value = 0;
  AfflictionSystem *parent;
  bool virtual tick();

public:
  Affliction(AfflictionSystem *parent); // เข้าไปใน AfflictionSystem อัตโนมัติ
  Affliction(GameObject *target);
  bool update(); // afflictionSystem จะเรียกใช้ และเมื่อไหร่ก็จามที่มันหมดอายุของมันแล้ว
                 // มันก็จะเอาอันนี้ออกจาก afflictionSystem
  //----------------------------//
  bool getPermanant() { return permanant; }
  string getName() { return name; }
  string getTargetValue() { return targetValue; }
  int getDuration() { return duration; }
  int getValueIncrese() { return valueIncrese; }
  int getPassedTime() { return passedTime; }
  int getValue() { return value; }
  AfflictionSystem *getParent() { return parent; }
};

class AbilitySystem
{
protected:
  GameObject *parent;
  bool isInAbility(string name);

public:
  vector<Ability *> abilityGroup;
  AbilitySystem(GameObject *parent);
  bool addAbility(Ability *ability);
  void decisionMakeing(int sur, int env, int repro);
  inline GameObject *getParent() { return parent; };
};

class AfflictionSystem
{
protected:
  GameObject *parent;

public:
  vector<Affliction *> afflictionGroup;
  void reClaCulateValue();
  AfflictionSystem(GameObject *parent);
  bool addAffliction(Affliction *affliction);
  bool removeAffliction(string name);
  bool removeAllAffliction(string name);
  int amountOfAffliction(string name);
  void updateAffliction();
  GameObject *getParent() { return parent; };
};

class StatusBlock : public AbilitySystem, public AfflictionSystem
{
protected:
  GameObject *parent;

public:
  vector<StatParam *> statParamGroup;
  StatusBlock(GameObject *parent);
  StatusBlock(StatusBlock *copy);
  bool isInParam(string name);
  void resetValue();
  bool addStat(StatParam *statParam); // คำนวน aff value ใหม่ทั้งหมดด้วยหลังจากทำเสร็จ
  int getParamValueRaw(string name);  // ถึงแม้จะไม่มีสแตดก็รีเทิร์น 0 ออกมา
  int getParamValue(string name);     // ถึงแม้จะไม่มีสแตดก็รีเทิร์น 0 ออกมา
  StatParam *getParam(string name);
  inline GameObject *getParent() { return parent; };
};

class GameObject
{ // จะสร้างซักตัวก็ต้องสร้าง abilty, stat ของมันให้พร้อมนะ
protected:
  static Var3DGraph sur, env, repo; // กำหนดเองตอนสร้าง class
  string name, represent;
  int x, y = 0;
  StatusBlock *stat;
  Layer *parent;

public:
  GameObject(string name, string represent, Layer *parent, int x, int y);
  GameObject(string name, Layer *parent, int x, int y);
  GameObject(string name, Layer *parent);
  GameObject(GameObject *copy);
  GameObject virtual *createItSelf() { return this; } // เอาไปเขียนใหม่ด้วย
  int virtual getSur() = 0;
  int virtual getEnv() = 0;
  int virtual getRepo() = 0;
  void virtual update(); // เล่นเทิร์น
  inline void setName(string stringInput) { name = stringInput; }
  void setRepresent(string stringInput) { represent = stringInput; }
  inline string getName() { return name; }
  inline string getRepresent() { return represent; }
  inline StatusBlock *getStat() { return stat; }
  inline Layer *getParent() { return parent; }
  pair<int, int> getVectorIndex();
  pair<int, int> getCoord();
  bool canAct(GameObject *);
};

class Feild : public GameObject
{
public:
  Feild(Layer *parent, string name = "Feild") : GameObject(name, parent) {}
  GameObject virtual *createItSelf() override
  {
    return new Feild(parent, "Feild");
  }
  int virtual getSur() override { return 0; }
  int virtual getEnv() override { return 0; }
  int virtual getRepo() override { return 0; }
  void virtual update() override { return; }
};

class Land : public Feild
{
public:
  Land(Layer *parent, string name = "Land") : Feild(parent, name) {}
  GameObject virtual *createItSelf() override
  {
    return new Land(parent, "Land");
  }
  void virtual update() override { return; }
};

class Layer
{ // ต้องทำการเรียงให้มีตำแหน่งติดลบด้วย
private:
  vector<vector<GameObject *>> layer;
  GameObject *default_value;
  LayerSystem &parent;
  string name;

public:
  explicit Layer(LayerSystem *parent, int width, int height, string name);
  //-------------------------//
  void action();
  inline LayerSystem &getParent();
  inline string getName();
  void setLayer(int layer_num);
  vector<vector<GameObject *>> getLayer();
};

class Ground;

class Land
{
private:
  Ground *parent;
  string represent;
  int x, y;

public:
  explicit Land(Ground *parentInput, string representInput,
                int xValueInput = 0, int yValueInput = 0);
  void update();
  int getX();
  int getY();
};

class Ground
{
private:
  vector<vector<Land>> layer;
  LayerSystem &parent;

public:
  explicit Ground(LayerSystem *p, int width, int height);
  inline vector<vector<Land>> getLayer();
};

class LayerSystem
{
private:
  int width;
  int height;
  vector<Layer *> layers;
  Ground ground;

public:
  explicit LayerSystem(int width, int height, int amount);
  //-------------------------//
  bool createNewLayer(string name); // ตรวจสอบชื่อซ้ำเพื่อไม่ให้มีชื่อซ้ำ
  bool createGround();
  bool removeLayer(string name); // มี layer นั้นๆให้ลบไหม
  //-------------------------//
  inline Layer *getLayer(string name);
  inline Layer *getLayer(int i);
  inline Ground getGround();
  vector<Layer *> *getLayers();
  inline string getLayerName(int i);
  inline int getLayerSize();
};

class Command
{
};

#endif