// #include "Interpolate.h"
// #include <array>
// #include <cmath>
// #include <stdexcept>
// #include <string>
// #include <utility>
// #include <vector>

// using namespace std;

// class StatParam;
// class StatParamMax;
// class Ability;
// class AbilitySystem;
// class Affliction;
// class AfflictionSystem;
// class StatusBlock;
// class GameObject;
// class Ground;
// class Layer;
// class LayerSystem;
// class Command;

// #ifndef FUNDAMENTALSYSTEM_H
// #define FUNDAMENTALSYSTEM_H

// class StatParam {
// protected:
//   string name; // ไปกำหนดเองตอนสร้าง classs
//   int targetValue; // ไปกำหนดเองตอนสร้าง class
//   int rawValue, value;
//   vector<pair<string, int>> stackInfo;
//   StatusBlock *parent;
//   void virtual action() = 0; // ส่งผลกับตัวเองยังไง
//   bool isAction(int before, int after);
//   void updateStackInfo();

// public:
//   StatParam(StatusBlock *parent, Ability *createBy, int rawValue);
//   StatParam(GameObject *target, Ability *createBy, int rawValue);
//   void virtual setRawTo(int i);      // คนใช้คือ Ability
//   bool virtual changeValueBy(int i); // คนใช้คือ Affliction
//   void pushStackInfo(pair<string, int> *target); // คนใช้คือ Ability
//   //----------------------------//
//   void resetValue();
//   inline string getName();
//   inline int getRawValue();
//   inline int getValue();
//   inline vector<pair<string, int>> getStackInfo();
//   inline StatusBlock *getParent();
// };

// class Ability { // statParam ทั้งหมดที่ Ability จะใช้ต้องถูกสร้างโดยตัว Ability เองทั้งหมด
// protected:
//   string name; // ไปกำหนดเองตอนสร้าง classs
//   Var3DGraph *data; // ไปกำหนดเองตอนสร้าง classs
//   AbilitySystem *parent;
//   void virtual createStatParam() { return; } // สร้าง Instant Class StatParam
//   // vector<GameObject*>
//   // ตามประเภทที่ ability นี้ต้องใช้ อันนี้ไป override ด้วยนะ

// public:
//   Ability(AbilitySystem *parent);
//   Ability(GameObject *target);
//   vector<GameObject *> virtual findTargetForPassive() = 0;
//   vector<GameObject *> virtual findTargetForActive() = 0;
//   bool virtual canActive(vector<GameObject *> targets) = 0;
//   void virtual passive(vector<GameObject *> targets) = 0;
//   void virtual active(vector<GameObject *> targets) = 0;
//   int decision(int sur, int env, int repro);
//   inline string getName();
//   inline AbilitySystem *getParent();
// };

// class Affliction {
// protected:
//   string name, targetValue; // ไปกำหนดเองตอนสร้าง class
//   int duration, valueIncrese, value; // ไปกำหนดเองตอนสร้าง classs
//   bool permanant; // ไปกำหนดเองตอนสร้าง classs
//   int passedTime = 0;
//   AfflictionSystem *parent;
//   bool virtual tick();

// public:
//   Affliction(AfflictionSystem *parent); // เข้าไปใน AfflictionSystem อัตโนมัติ
//   Affliction(GameObject *target);
//   bool update(); // afflictionSystem จะเรียกใช้ และเมื่อไหร่ก็จามที่มันหมดอายุของมันแล้ว
//                  // มันก็จะเอาอันนี้ออกจาก afflictionSystem
//   //----------------------------//
//   bool getPermanant();
//   string getName();
//   string getTargetValue();
//   int getDuration();
//   int getValueIncrese();
//   int getPassedTime();
//   int getValue();
//   AfflictionSystem *getParent();
// };

// class AbilitySystem {
// protected:
//   GameObject *parent;
//   bool isInAbility(string name);

// public:
//   vector<Ability *> abilityGroup;
//   AbilitySystem(GameObject *parent);
//   bool addAbility(Ability *ability);
//   void decisionMakeing(int sur, int env, int repro);
//   inline GameObject *getParent();
// };

// class AfflictionSystem {
// protected:
//   GameObject *parent;

// public:
//   vector<Affliction *> afflictionGroup;
//   AfflictionSystem(GameObject *parent);
//   void recalculateValue();
//   bool addAffliction(Affliction *affliction);
//   bool removeAffliction(string name);
//   bool removeAllAffliction(string name);
//   int amountOfAffliction(string name);
//   void updateAffliction();
//   GameObject *getParent();
// };

// class StatusBlock : public AbilitySystem, public AfflictionSystem {
// protected:
//   GameObject *parent;

// public:
//   vector<StatParam *> statParamGroup;
//   StatusBlock(GameObject *parent);
//   bool isInParam(string name);
//   void resetValue();
//   bool addStat(StatParam *statParam); // คำนวน aff value ใหม่ทั้งหมดด้วยหลังจากทำเสร็จ
//   int getParamValueRaw(string name); // ถึงแม้จะไม่มีสแตดก็รีเทิร์น 0 ออกมา
//   int getParamValue(string name); // ถึงแม้จะไม่มีสแตดก็รีเทิร์น 0 ออกมา
//   StatParam *getParam(string name);
//   inline GameObject *getParent();
// };

// class GameObject { // จะสร้างซักตัวก็ต้องสร้าง abilty, stat ของมันให้พร้อมนะ
// protected:
//   string name, represent;
//   Var3DGraph *sur, *env, *repo; // กำหนดเองตอนสร้าง class
//   StatusBlock *stat;
//   Layer *parent;

// public:
//   GameObject(Layer *parent, int x, int y);
//   int virtual getSur() = 0;
//   int virtual getEnv() = 0;
//   int virtual getRepo() = 0;
//   void virtual update() = 0; // เล่นเทิร์น
//   pair<int, int> getVectorIndex();
//   pair<int, int> getCoord();
//   inline void setName(string stringInput);
//   void setRepresent(string stringInput);
//   inline string getName();
//   inline string getRepresent();
//   inline StatusBlock *getStat();
//   inline Layer *getParent();
//   vector<GameObject*> findTargetInRange(int range);
//   // bool canAct(GameObject *);
// };

// class Layer { // ต้องทำการเรียงให้มีตำแหน่งติดลบด้วย
// private:
//   string name;
//   GameObject *default_value;
//   LayerSystem *parent;

// public:
//   vector<vector<GameObject *>> insideLayer;
//   Layer(string name, LayerSystem *parent, int width, int height);
//   void action();
//   void addToLayer(GameObject *target, int x, int y);
//   void removeFromLayer(int x, int y);
//   inline string getName();
//   inline void setName(string nameInput);
//   inline GameObject *getFromLayer(int x, int y);
//   LayerSystem *getParent();
// };

// class Land {
// private:
//   Ground *parent;
//   string represent;
//   int x, y;

// public:
//   explicit Land(Ground *parentInput, string representInput, int xValueInput = 0,
//                 int yValueInput = 0);
//   int getX();
//   int getY();
// };

// class Ground {
// private:
//   LayerSystem *parent;

// public:
// Ground(LayerSystem *p = nullptr, int width = 0, int height = 0);
//   vector<vector<Land>> insideLayer;
// };

// class LayerSystem {
// private:
//   int width;
//   int height;
//   Ground ground;
//   bool isInLayer(string name);

// public:
//   vector<Layer *> layers;
//   LayerSystem(int width, int height, int amount);
//   LayerSystem(int width, int height);
//   //-------------------------//
//   void update();
//   bool createNewLayer(string name); // ตรวจสอบชื่อซ้ำเพื่อไม่ให้มีชื่อซ้ำ
//   bool removeLayer(string name); // มี layer นั้นๆให้ลบไหม
//   double getDistance(GameObject *a, GameObject *b);
//   double getDistance(int x1, int y1, int x2, int y2);
//   //-------------------------//
//   Layer *getLayer(string name);
//   inline Layer *getLayer(int i);
//   inline Ground getGround();
//   inline string getLayerName(int i);
//   vector<vector<GameObject *>> getOverAllLayer();
// };

// #endif