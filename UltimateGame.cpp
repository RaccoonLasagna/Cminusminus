#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/Window.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>

#define FONT_PATH "C:/Users/ADMIN/Documents/GitHub/Cminusminus/Font/f.ttf"
#define COLOR1 Color(194, 168, 135, 255)
#define COLOR2 Color(223, 175, 94, 255)
#define COLOR3 Color(97, 113, 77, 255)
#define COLOR4 Color(143, 174, 147, 255)
#define COLOR5 Color(109, 21, 43, 255)

using namespace std;
using namespace sf;

class Var3DGraph;
class StatParam;
class Ability;
class AbilitySystem;
class Affliction;
class AfflictionSystem;
class StatusBlock;
class GameObject;
class Layer;
class Land;
class Ground;
class LayerSystem;
class Command;

class Var3DGraph {
protected:
  std::vector<std::array<int, 4>> exampleData;

public:
  Var3DGraph(const std::vector<std::array<int, 4>> &exampleData) {
    // กำหนด exampleData โดยเรียงจากน้อยไปมากตาม x, y, z
    this->exampleData = exampleData;
    sort(this->exampleData.begin(), this->exampleData.end(),
         [](const auto &a, const auto &b) {
           if (a[0] != b[0]) // เรียงตาม x ถ้าไม่เท่ากัน
             return a[0] < b[0];
           else if (a[1] != b[1]) // เรียงตาม y ถ้า x เท่ากันและ y ไม่เท่ากัน
             return a[1] < b[1];
           else // เรียงตาม z ถ้า x และ y เท่ากัน
             return a[2] < b[2];
         });
  }

  int interpolate(int x, int y, int z) {
    // ตรวจสอบว่าจุดอยู่นอกขอบเขตข้อมูลหรือไม่
    if (x < exampleData.front()[0] || x > exampleData.back()[0] ||
        y < exampleData.front()[1] || y > exampleData.back()[1] ||
        z < exampleData.front()[2] || z > exampleData.back()[2]) {
      return 0;
    }

    // หาตำแหน่งข้อมูลที่อยู่ระหว่าง x, y, z
    size_t i = 0;
    while (i < exampleData.size() - 1 && exampleData[i][0] < x) {
      ++i;
    }

    // ทำ interpolation แยกตามแต่ละมิติ
    int x1 = exampleData[i][0], y1 = exampleData[i][1], z1 = exampleData[i][2],
        val1 = exampleData[i][3];
    int x2 = exampleData[i + 1][0], y2 = exampleData[i + 1][1],
        z2 = exampleData[i + 1][2], val2 = exampleData[i + 1][3];

    // คำนวณอัตราส่วนของ interpolation สำหรับแต่ละมิติ
    double alpha_x = static_cast<double>(x - x1) / (x2 - x1);
    double alpha_y = static_cast<double>(y - y1) / (y2 - y1);
    double alpha_z = static_cast<double>(z - z1) / (z2 - z1);

    // ทำ interpolation แยกตามแต่ละมิติ
    int val_x1 = val1 + alpha_x * (val2 - val1);
    int val_x2 = val1 + alpha_x * (val2 - val1);
    int val_y1 = val1 + alpha_y * (val2 - val1);
    int val_y2 = val1 + alpha_y * (val2 - val1);
    int val_z1 = val1 + alpha_z * (val2 - val1);
    int val_z2 = val1 + alpha_z * (val2 - val1);

    // ทำ interpolation สุดท้าย
    int val_xy1 = val_x1 + alpha_y * (val_x2 - val_x1);
    int val_xy2 = val_y1 + alpha_y * (val_y2 - val_y1);
    int val_xyz1 = val_xy1 + alpha_z * (val_xy2 - val_xy1);

    return val_xyz1;
  }
};

class StatParam {
protected:
  std::string name; // ไปกำหนดเองตอนสร้าง classs
  int targetValue; // ไปกำหนดเองตอนสร้าง class
  int defaultValue, value;
  std::vector<std::pair<std::string, int>> stackInfo;
  StatusBlock *parent;
  void virtual action() = 0; // ส่งผลกับตัวเองยังไง
  bool isAction(int before, int after);
  void updateStackInfo();

public:
  StatParam(StatusBlock *parent, Ability *createBy, int defaultValue);
  StatParam(GameObject *target, Ability *createBy, int defaultValue);
  void virtual setDefaultTo(int i);  // คนใช้คือ Ability
  bool virtual changeValueBy(int i); // คนใช้คือ Affliction
  void pushStackInfo(std::pair<std::string, int> *target); // คนใช้คือ Ability
  //----------------------------//
  void resetValue();
  inline std::string getName();
  inline int getDefaultValue();
  inline int getValue();
  inline std::vector<std::pair<std::string, int>> getStackInfo();
  inline StatusBlock *getParent();
};

class Ability { // statParam ทั้งหมดที่ Ability จะใช้ต้องถูกสร้างโดยตัว Ability เองทั้งหมด
protected:
  std::string name = "Ability"; // ไปกำหนดเองตอนสร้าง classs
  Var3DGraph *data; // ไปกำหนดเองตอนสร้าง classs
  AbilitySystem *parent;
  void virtual createStatParam() { return; } // สร้าง Instant Class StatParam
  // vector<GameObject*>
  // ตามประเภทที่ ability นี้ต้องใช้ อันนี้ไป override ด้วยนะ

public:
  Ability(AbilitySystem *parent);
  Ability(GameObject *target);
  std::vector<GameObject *> virtual findTargetForPassive() = 0;
  std::vector<GameObject *> virtual findTargetForActive() = 0;
  bool virtual canActive(std::vector<GameObject *> targets) = 0;
  void virtual passive(std::vector<GameObject *> targets) = 0;
  void virtual active(std::vector<GameObject *> targets) = 0;
  int decision(int sur, int env, int repro);
  inline std::string getName();
  inline AbilitySystem *getParent();
};

class Affliction {
protected:
  std::string name, targetValue; // ไปกำหนดเองตอนสร้าง class
  int duration, valueIncrese, value; // ไปกำหนดเองตอนสร้าง classs
  bool permanant; // ไปกำหนดเองตอนสร้าง classs
  int passedTime = 0;
  AfflictionSystem *parent;
  bool virtual tick();

public:
  Affliction(AfflictionSystem *parent); // เข้าไปใน AfflictionSystem อัตโนมัติ
  Affliction(GameObject *target);
  bool update(); // afflictionSystem จะเรียกใช้ และเมื่อไหร่ก็จามที่มันหมดอายุของมันแล้ว
                 // มันก็จะเอาอันนี้ออกจาก afflictionSystem
  //----------------------------//
  bool getPermanant();
  std::string getName();
  std::string getTargetValue();
  int getDuration();
  int getValueIncrese();
  int getPassedTime();
  int getValue();
  AfflictionSystem *getParent();
};

class AbilitySystem {
protected:
  GameObject *parent;

public:
  std::vector<Ability *> abilityGroup;
  bool isInAbility(std::string name);
  AbilitySystem(GameObject *parent);
  bool addAbility(Ability *ability);
  void decisionMaking(int sur, int env, int repro);
  inline GameObject *getParent();
};

class AfflictionSystem {
protected:
  GameObject *parent;

public:
  vector<Affliction *> afflictionGroup = {};
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
  string name;
  string represent = " ";
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
private:
  string name;
  GameObject *default_value;
  LayerSystem *parent;

public:
  vector<vector<GameObject *>> insideLayer;
  Layer(string name, LayerSystem *parent, int width, int height);
  void action();
  void setLayer(GameObject *target, int x, int y);
  void removeFromLayer(int x, int y);
  inline string getName();
  inline void setName(string nameInput);
  inline GameObject *getFromLayerIndex(int x, int y);
  GameObject *getFromLayerCoord(int x, int y);
  LayerSystem *getParent();
};

class Land {
private:
  Ground *parent;
  string represent;
  int x, y;

public:
  explicit Land(Ground *parentInput, string representInput, int xValueInput = 0,
                int yValueInput = 0);
  int getX();
  int getY();
};

class Ground {
private:
  LayerSystem *parent;

public:
  Ground(LayerSystem *p = nullptr, int width = 0, int height = 0);
  vector<vector<Land>> insideLayer;
  pair<int, int> getVectorIndex(int x, int y);
};

class LayerSystem {
private:
  int width;
  int height;
  Ground ground;
  bool isInLayer(string name);

public:
  vector<Layer *> layers;
  vector<string> logActionData;
  LayerSystem(int width, int height, int amount);
  LayerSystem(int width, int height);
  //-------------------------//
  void update();
  bool createNewLayer(string name); // ตรวจสอบชื่อซ้ำเพื่อไม่ให้มีชื่อซ้ำ
  bool removeLayer(string name); // มี layer นั้นๆให้ลบไหม
  double getDistance(GameObject *a, GameObject *b);
  double getDistance(int x1, int y1, int x2, int y2);
  //-------------------------//
  inline Layer *getLayer(string name);
  inline Layer *getLayer(int i);
  inline Layer *getRandomLayer();
  inline Ground getGround();
  void printLayer();
  int getLayersWidth() { return width; }
  int getLayersHeight() { return height; }
  int getLayersAmount();
  inline string getLayerName(int i);
  void randomGenerateLayers();
  vector<vector<GameObject *>> getOverAllLayer() {
    vector<vector<GameObject *>> temp;
    for (int row = 0; row < height; row++) {
      vector<GameObject *> tempRow;
      for (int col = 0; col < width; col++) {
        for (int i = 0; i < layers.size(); i++) {
          if (layers[i]->insideLayer[row][col] != nullptr) {
            tempRow.push_back(layers[i]->insideLayer[row][col]);
            break;
          }
          if (i == layers.size() - 1) {
            tempRow.push_back(nullptr);
          }
        }
      }
      temp.push_back(tempRow);
    }
    return temp;
  }
};

class Health_Aff : public Affliction {
protected:
  string name = "Health_Aff";
  string targetValue = "Health";
  int duration, valueIncrease, value;
  bool permanent = true;

public:
  Health_Aff(StatusBlock *parent, int duration, int valueIncrease, int value);
  Health_Aff(GameObject *target, int duration, int valueIncrease, int value);
};

// Hunger is permanent, it is removed when an animal eats
class Hunger_Aff : public Affliction {
protected:
  string name = "Hunger_Aff";
  string targetValue = "Hunger";
  int duration, valueIncrease, value;
  bool permanent = true;

public:
  Hunger_Aff(StatusBlock *parent, int duration, int valueIncrease, int value);
  Hunger_Aff(GameObject *target, int duration, int valueIncrease, int value);
};

class Attack_Aff : public Affliction {
protected:
  string name = "Attack_Aff";
  string targetValue = "Atk";
  int duration, valueIncrease, value;
  bool permanent = false;

public:
  Attack_Aff(StatusBlock *parent, int duration, int valueIncrease, int value);
  Attack_Aff(GameObject *target, int duration, int valueIncrease, int value);
};

// sight shouldn't be incremented alot so remove valIncrease
class Sight_Aff : public Affliction {
protected:
  string name = "Sight_Aff";
  string targetValue = "Sight";
  int duration, value;
  bool permanent = false;

public:
  Sight_Aff(StatusBlock *parent, int duration, int value);
  Sight_Aff(GameObject *target, int duration, int value);
};

class Speed_Aff : public Affliction {
protected:
  string name = "Speed_Aff";
  string targetValue = "Speed";
  int duration, valueIncrease, value;
  bool permanent = false;

public:
  Speed_Aff(StatusBlock *parent, int duration, int valueIncrease, int value);
  Speed_Aff(GameObject *target, int duration, int valueIncrease, int value);
};

class MateCooldown : public Affliction {
protected:
  string name = "MateCooldown";
  string targetValue = "";
  int duration, valueIncrease = 0, value = 0;
  bool permanent = false;

public:
  MateCooldown(StatusBlock *parent, int duration);
  MateCooldown(GameObject *target, int duration);
};

class EatTimes : public Affliction {
protected:
  string name = "FruitionCooldown";
  string targetValue = "";
  int duration, valueIncrease = 0, value = 0;
  bool permanent = true;

public:
  EatTimes(StatusBlock *parent);
  EatTimes(GameObject *target);
};

class FruitionCooldown : public Affliction {
protected:
  string name = "FruitionCooldown";
  string targetValue = "";
  int duration, valueIncrease = 0, value = 0;
  bool permanent = false;

public:
  FruitionCooldown(StatusBlock *parent, int duration);
  FruitionCooldown(GameObject *target, int duration);
};

class Poisonous : public Affliction {
protected:
  string name = "Poisonous";
  string targetValue = "";
  int duration, valueIncrease = 0, value = 0;
  bool permanent = false;

public:
  Poisonous(StatusBlock *parent);
  Poisonous(GameObject *target);
};

class Hunger : public StatParam {
protected:
  string name = "Hunger";
  int targetValue = 0;
  void virtual action() override;

public:
  Hunger(StatusBlock *parent, Ability *createBy, int rawValue);
  Hunger(GameObject *target, Ability *createBy, int rawValue);
};

// No abilities create this, include for all living things
class Health : public StatParam {
protected:
  string name = "Health";
  int targetValue = 0;
  void virtual action() override;

public:
  Health(StatusBlock *parent, Ability *createBy, int rawValue);
  Health(GameObject *target, Ability *createBy, int rawValue);
};

class Atk : public StatParam {
protected:
  string name = "Atk";
  int targetValue = 0;
  void virtual action() override;

public:
  Atk(StatusBlock *parent, Ability *createBy, int rawValue);
  Atk(GameObject *target, Ability *createBy, int rawValue);
};

class Sight : public StatParam {
protected:
  string name = "Sight";
  int targetValue = 0;
  void virtual action() override;

public:
  Sight(StatusBlock *parent, Ability *createBy, int rawValue);
  Sight(GameObject *target, Ability *createBy, int rawValue);
};

class Speed : public StatParam {
protected:
  string name = "Speed";
  int targetValue = 0;
  void virtual action() override;

public:
  Speed(StatusBlock *parent, Ability *createBy, int rawValue);
  Speed(GameObject *target, Ability *createBy, int rawValue);
};

class Eat : public Ability {
protected:
  string name = "Eat";
  int max_hunger;
  bool eatPlants, eatEarthworms, eatAnimals;
  Var3DGraph data = Var3DGraph({{0, 0, 0, 0},
                                {0, 100, 0, 100},
                                {100, 100, 0, 40},
                                {0, 0, 100, 10},
                                {30, 80, 5, 70},
                                {100, 100, 100, 25},
                                {50, 10, 100, 10}}); // เดี๋ยวค่อยคิดใหม่
  void createStatParam() override;

public:
  Eat(AbilitySystem *parent, int max_hunger, bool eatPlants, bool eatEarthworms,
      bool eatAnimals);
  Eat(GameObject *target, int max_hunger, bool eatPlants, bool eatEarthworms,
      bool eatAnimals);
  vector<GameObject *> virtual findTargetForPassive() override;
  vector<GameObject *> virtual findTargetForActive() override;
  bool virtual canActive(vector<GameObject *> targets) override;
  void virtual passive(vector<GameObject *> targets) override;
  void virtual active(vector<GameObject *> targets) override;
};

//--------------------------- Attack ---------------------------

class Attack : public Ability {
protected:
  string name = "Attack";
  int atk_amount;
  Var3DGraph data = Var3DGraph({{0, 0, 0, 0},
                                {0, 100, 0, 100},
                                {100, 100, 0, 40},
                                {0, 0, 100, 10},
                                {30, 80, 5, 70},
                                {100, 100, 100, 25}}); // remember to change pls
  void createStatParam() override;

public:
  Attack(AbilitySystem *parent, int atk_amount);
  Attack(GameObject *target, int atk_amount);
  vector<GameObject *> virtual findTargetForPassive() override;
  vector<GameObject *> virtual findTargetForActive() override;
  bool virtual canActive(vector<GameObject *> targets) override;
  void virtual passive(vector<GameObject *> targets) override;
  void virtual active(vector<GameObject *> targets) override;
};

//--------------------------- Walk ---------------------------

class Walk : public Ability {
protected:
  string name = "Walk";
  void createStatParam() override;

public:
  Walk(AbilitySystem *parent);
  Walk(GameObject *target);
  vector<GameObject *> virtual findTargetForPassive() override;
  vector<GameObject *> virtual findTargetForActive() override;
  bool virtual canActive(vector<GameObject *> targets) override;
  void virtual passive(vector<GameObject *> targets) override;
  void virtual active(vector<GameObject *> targets) = 0;
};

class WalkSeek : public Walk {
protected:
  Var3DGraph data = Var3DGraph({{0, 0, 0, 0},
                                {0, 100, 0, 100},
                                {100, 100, 0, 40},
                                {0, 0, 100, 10},
                                {30, 80, 5, 70},
                                {100, 100, 100, 25}});

public:
  WalkSeek(AbilitySystem *parent);
  WalkSeek(GameObject *target);
  void virtual active(vector<GameObject *> targets) override;
};

class WalkEscape : public Walk {
protected:
  Var3DGraph data = Var3DGraph({{0, 0, 0, 0},
                                {0, 100, 0, 100},
                                {100, 100, 0, 40},
                                {0, 0, 100, 10},
                                {30, 80, 5, 70},
                                {100, 100, 100, 25}});

public:
  WalkEscape(AbilitySystem *parent);
  WalkEscape(GameObject *target);
  void virtual active(vector<GameObject *> targets) override;
};

//--------------------------- Mate ---------------------------

class Mate : public Ability {
protected:
  string name = "Mate";
  void createStatParam() override;

public:
  Mate(AbilitySystem *parent);
  Mate(GameObject *target);
  vector<GameObject *> virtual findTargetForPassive() override;
  vector<GameObject *> virtual findTargetForActive() override;
  bool virtual canActive(vector<GameObject *> targets) override;
  void virtual passive(vector<GameObject *> targets) override;
  void virtual active(vector<GameObject *> targets) override;
};

// ===================================== CORPSES
// =====================================

//--------------------------- Rot ---------------------------

class Rot : public Ability {
protected:
  string name = "Mate";
  int rot_timer = 0;
  void createStatParam() override;

public:
  Rot(AbilitySystem *parent);
  Rot(GameObject *target);
  vector<GameObject *> virtual findTargetForPassive() override;
  vector<GameObject *> virtual findTargetForActive() override;
  bool virtual canActive(vector<GameObject *> targets) override;
  void virtual passive(vector<GameObject *> targets) override;
  void virtual active(vector<GameObject *> targets) override;
};

// ===================================== PLANTS
// =====================================

// --------------------------- Fruition ---------------------------

class Fruition : public Ability {
protected:
  string name = "Fruition";
  void createStatParam() override;

public:
  Fruition(AbilitySystem *parent);
  Fruition(GameObject *target);
  vector<GameObject *> virtual findTargetForPassive() override;
  vector<GameObject *> virtual findTargetForActive() override;
  bool virtual canActive(vector<GameObject *> targets) override;
  void virtual passive(vector<GameObject *> targets) override;
  void virtual active(vector<GameObject *> targets) override;
};

class SingleEat : public Ability {
protected:
  string name = "SingleEat";
  void createStatParam() override;

public:
  SingleEat(AbilitySystem *parent);
  SingleEat(GameObject *target);
  vector<GameObject *> virtual findTargetForPassive() override;
  vector<GameObject *> virtual findTargetForActive() override;
  bool virtual canActive(vector<GameObject *> targets) override;
  void virtual passive(vector<GameObject *> targets) override;
  void virtual active(vector<GameObject *> targets) override;
};

class Animal : public GameObject {
protected:
public:
  Animal(Layer *parent, int x, int y);
};

class Plant : public GameObject {

protected:
public:
  Plant(Layer *parent, int x, int y);
};

class Corpse : public GameObject {

protected:
  string name = "Corpse";

public:
  Corpse(Layer *parent, int x, int y);
};

class Lion : public Animal {
protected:
  string name = "Lion";

public:
  Lion(Layer *parent, int x, int y);
};

class Bear : public Animal {
protected:
  string name = "Bear";

public:
  Bear(Layer *parent, int x, int y);
};

class Cheetah : public Animal {
protected:
  string name = "Cheetah";

public:
  Cheetah(Layer *parent, int x, int y);
};

class Giant_Snake : public Animal {
protected:
  string name = "Giant Snake";

public:
  Giant_Snake(Layer *parent, int x, int y);
};

class Rabbit : public Animal {
protected:
  string name = "Rabbit";

public:
  Rabbit(Layer *parent, int x, int y);
};

class Goat : public Animal {
protected:
  string name = "Goat";

public:
  Goat(Layer *parent, int x, int y);
};

class Deer : public Animal {
protected:
  string name = "Deer";

public:
  Deer(Layer *parent, int x, int y);
};

class Chicken : public Animal {
protected:
  string name = "Chicken";

public:
  Chicken(Layer *parent, int x, int y);
};

class Poison_Frog : public Animal {
protected:
  string name = "Poison Frog";

public:
  Poison_Frog(Layer *parent, int x, int y);
};

class Bush : public Plant {
protected:
  string name = "Bush";

public:
  Bush(Layer *parent, int x, int y);
};

class Mushroom : public Plant {
protected:
  string name = "Mushroom";

public:
  Mushroom(Layer *parent, int x, int y);
};

class Apple_tree : public Plant {
protected:
  string name = "Apple tree";
  string represent = "";

public:
  Apple_tree(Layer *parent, int x, int y);
};

// not even considered an animal, since it should only be able to walk
class EarthWorm : public GameObject {
protected:
  string name = "EarthWorm";

public:
  EarthWorm(Layer *parent, int x, int y);
};

class Grid {
private:
  RectangleShape shape;
  GameObject *object;

public:
  Grid(Vector2f size, Vector2f position, GameObject *object);
  void draw(RenderWindow &window); /*not require just have*/
  GameObject *getObject();
};

class Map { // แสดงเฉพาะส่วน
private:
  Font font;
  Text tempText;
  float width, height; /*จำเป็นในการตีตาราง*/
  int row, column; /*จำเป็นในการตีตาราง*/
  Vector2i center; /*จุดกึ่งกลางของการแสดงผล*/
  RectangleShape border;        /*shape ที่กำหนด position*/
  vector<vector<Grid>> gridMap; /*เวลาซูม เคลียแล้วสร้างใหม่หมดเลยง่ายกว่า*/
  vector<vector<GameObject *>> dataMap; /*ต้องไปดึงมาทุกเทิร์น*/
  vector<vector<Text>> showText;

public:
  void update();
  Map(Vector2f size, Vector2f position, vector<vector<GameObject *>> dataMap);
  void draw(RenderWindow &window);
  void setDataMap(vector<vector<GameObject *>> dataMap);
  void setRowColumn(Vector2i size);
  void changeCenter(Vector2i center);
  GameObject *getObject(Vector2f position);
  Vector2i getRowColumn();
  Vector2i getSize();
  Vector2i getCenter();
  vector<vector<GameObject *>> getDataMap();
  RectangleShape getBorder() { return border; }
};

class ZoomIn {
protected:
  CircleShape shape;
  Font font;
  Text text;

public:
  ZoomIn(Vector2f position, int radius);
  void draw(RenderWindow &window);
  CircleShape getShape();
  void zoomIn(Map &map);
};

class ZoomOut : public ZoomIn {
public:
  ZoomOut(Vector2f position, int radius);
  void zoomOut(Map &map);
};

class LayerButton {
private:
  RectangleShape button;
  Font font;
  Text text;
  vector<vector<GameObject *>> dataMap;

public:
  LayerButton(Vector2f position, Vector2f size, string text,
              vector<vector<GameObject *>> dataMap);
  void draw(RenderWindow &window);
  void setDataMap(vector<vector<GameObject *>> layer);
  void pushToMap(Map &map);
  RectangleShape getButton() { return button; }
};

class LayerSheet {
private:
  RectangleShape sheet;
  LayerSystem *data;
  Font font;
  vector<Text> text;
  void dumbToButton(vector<vector<GameObject *>> DataMap, LayerButton &button);

public:
  LayerButton *selectedbutton;
  vector<LayerButton> buttons;
  LayerSheet(LayerSystem *data, Vector2f position, Vector2f size);
  void draw(RenderWindow &window);
  void setSelectedButton(int i);
  LayerSystem *getData() { return data; }
  void update(); /*จะทำการเล่นเทิร์นพร้อมกับ dumb ข้อมูลทั้งหมดออกมา*/
};

class StopButton {
protected:
  RectangleShape shape;
  Font font;
  Text text;

public:
  StopButton(Vector2f position, Vector2f size);
  void draw(RenderWindow &window);
  RectangleShape getShape();
};

class PlayButton : public StopButton {
public:
  PlayButton(Vector2f position, Vector2f size);
};

class RunButton : public StopButton {
public:
  RunButton(Vector2f position, Vector2f size);
};

class TextBox {
protected:
  RectangleShape shape, up, down;
  vector<Text> showText; // จำนวนจะเปลี่ยนไปตามขนาดของ TextBox
  vector<string> allText;
  Font font;
  int showTextIndex, showTextCapacity, textCapacity;
  Text textUp, textDown;

public:
  TextBox(Vector2f position, Vector2f size, vector<string> defaultVec,
          int textCapacity = 50,
          int showTextCapacity = 10); // ข้อควรระวัง defaultVec
                                      // ต้องมีขนาดเท่ากับหรือมากกว่า ShowTextCapacity
  void update();
  void draw(RenderWindow &window);
  void dumbText(string text);
  void dumbText(vector<string> text);
  RectangleShape getShape();
  RectangleShape getUp() { return up; }
  RectangleShape getDown() { return down; }
  void shiftShowTextUp();
  void shiftShowTextDown();
  void setAllText(vector<string> text) { allText = text; }
};

class Detail : public TextBox {
protected:
  GameObject *object;
  CircleShape deleteButton;
  void update();

public:
  Detail(Vector2f position, Vector2f size);
  void draw(RenderWindow &window);
  void setObject(GameObject *object);
  CircleShape getDeleteButton();
};

class Log : public TextBox {
protected:
  float DefaultHeight;

public:
  void update();
  Log(Vector2f position, Vector2f size);
};

class SelectedList {
  Text tempText;
  RectangleShape tempShape;
  RectangleShape shape, up, down;
  vector<Text> showText; // จำนวนจะเปลี่ยนไปตามขนาดของ TextBox
  vector<RectangleShape> showShape;
  Text textUp, textDown;
  Font font;
  int showTextIndex, showTextCapacity;

public:
  vector<GameObject *> allObject;
  SelectedList(Vector2f position, Vector2f size,
               vector<GameObject *> allObject = {}, int showTextCapacity = 5);
  void draw(RenderWindow &window);
  void dumbObject(GameObject *object);
  RectangleShape getShape();
  RectangleShape getUp() { return up; }
  RectangleShape getDown() { return down; }
  void shiftShowTextUp();
  void shiftShowTextDown();
  GameObject *click(Vector2f position);
  void update();
  void clear();
  void setAllObject(vector<GameObject *> object) { allObject = object; }
};

class CommandList {
protected:
  RectangleShape shape, up, down;
  vector<Text> showText; // จำนวนจะเปลี่ยนไปตามขนาดของ TextBox
  vector<RectangleShape> showShape;
  Text textUp, textDown;
  Font font;
  int showTextIndex, textCapacity;

public:
  vector<string> allObject;
  CommandList(Vector2f position, Vector2f size, vector<string> allObject = {},
              int textCapacity = 5, int showTextCapacity = 5);
  void draw(RenderWindow &window);
  RectangleShape getShape() { return shape; }
  RectangleShape getUp() { return up; }
  RectangleShape getDown() { return down; }
  void shiftShowTextUp();
  void shiftShowTextDown();
  string click(Vector2f position);
};

class Gui {
private:
  // ส่วนของการสร้างหน้าต่าง
  RenderWindow *window;
  Text turn;
  Font font;
  Sprite sprite;
  Texture spriteTexture;
  void renderAny();

  // ส่วนของการรับข้อมูลจากผู้ใช้
  Event event;
  void pollEvent();

  // ส่วนของการเช็คการคลิก
  bool clickAble;
  Vector2i mousePosition;
  Vector2f mousePositionView;
  void updateMousePosition();
  bool clickAbleCheck();
  void updateClickAble();

  // ส่วนของการเช็คการกดปุ่ม
  bool pressAble;
  Keyboard::Key key;
  void updateKeyboard();
  bool pressAbleCheck();
  void updatePressAble();

  // ส่วนของการรันเกม
  StopButton *stopButton;
  PlayButton *playButton;
  RunButton *runButton;
  bool play, run = false;
  void updateUpdateButton();
  void updateLayerSystem();
  void renderUpdateButton();

  // ส่วนของการดึงข้อมูลจาก FundamentalSystem
  int turnCount = 0;
  vector<LayerButton> layerButtons;
  LayerSheet *layerSheet;
  void updateLayerSheet();
  void renderLayerSheet();

  // ส่วนของการสร้างและแสดงข้อมูล
  Map *map;
  void updateMapCenter();
  void renderMap();

  // ส่วนของการซูม
  ZoomIn *zoomIn;
  ZoomOut *zoomOut;
  void updateZoomButton();
  void renderZoomButton();

  // ส่วนของการแสดงข้อมูล
  bool detailShow = false;
  Detail *detail;
  void updateDetail();
  void renderDetail();

  // ส่วนของการแสดงข้อมูลทั้งหมด
  Log *log;
  void updateLog();
  void renderLog();

  // ส่วนของการเลือก
  SelectedList *selectedList;
  void updateSelectedList();
  void renderSelectedList();

  // ส่วนของการคำสั่ง
  CommandList *commandList;
  void updateCommandList();
  void renderCommandList();

public:
  Gui();
  bool isOpen();
  void update();
  void render();
};

//------------------------------------------------------------------------------------//
bool StatParam::isAction(int before, int after) {
  if (before > targetValue && targetValue >= after) {
    action();
    return true;
  }
  return false;
}

void StatParam::updateStackInfo() {
  int highest = 0;
  for (pair<string, int> pair : stackInfo) {
    int value = pair.second;
    if (value > highest) {
      highest = value;
    }
  }
  setDefaultTo(highest);
}

StatParam::StatParam(StatusBlock *parent, Ability *createBy, int defaultValue)
    : parent(parent) {
  cout << "start statparam" << endl;
  cout << createBy->getName() << endl;
  pair<string, int> pair = make_pair(createBy->getName(), defaultValue);
  stackInfo.push_back(pair);
  updateStackInfo();
  parent->addStat(this);
}

StatParam::StatParam(GameObject *target, Ability *createBy, int defaultValue)
    : StatParam(target->getStat(), createBy, defaultValue) {}

void StatParam::setDefaultTo(int i) {
  if (i < 0) {
    defaultValue = 0;
  } else {
    defaultValue = i;
  }
  value = defaultValue;
}

bool StatParam::changeValueBy(int i) {
  int before = value;
  value + i < 0 ? value = 0 : value += i;
  return isAction(before, value);
}

void StatParam::pushStackInfo(pair<string, int> *target) {
  for (pair<string, int> pair : stackInfo) {
    if (pair.first == target->first) {
      return;
    }
  }
  stackInfo.push_back(*target);
  updateStackInfo();
}

void StatParam::resetValue() { value = defaultValue; }
string StatParam::getName() { return name; }
int StatParam::getDefaultValue() { return defaultValue; }
int StatParam::getValue() { return value; }
vector<pair<string, int>> StatParam::getStackInfo() { return stackInfo; }
StatusBlock *StatParam::getParent() { return parent; }

Ability::Ability(AbilitySystem *parent) : parent(parent) {
  cout << "Ability::Ability(AbilitySystem *parent) : parent(parent)\n";
  // parent->addAbility(this);
  cout
      << "Ability::Ability(AbilitySystem *parent) : parent->addAbility(this)\n";
  createStatParam();
  cout << "Ability::Ability(AbilitySystem *parent) : createStatParam()\n";
}

Ability::Ability(GameObject *target) : Ability(target->getStat()) {}

int Ability::decision(int sur, int env, int repo) {
  return data->interpolate(sur, env, repo);
}

string Ability::getName() { return name; }
AbilitySystem *Ability::getParent() { return parent; }

// ------------------Affliction--------------------//

bool Affliction::tick() {
  if (duration <= passedTime) {
    if (!permanant) {
      return false;
    }
  } else {
    passedTime += 1;
  }
  value += valueIncrese;
  return true;
}

Affliction::Affliction(AfflictionSystem *parent) : parent(parent) {
  // parent->addAffliction(this);
}

Affliction::Affliction(GameObject *target) : Affliction(target->getStat()) {}

bool Affliction::update() {
  if (tick()) {
    return true;
  }
  return false;
}

bool Affliction::getPermanant() { return permanant; }
string Affliction::getName() { return name; }
string Affliction::getTargetValue() { return targetValue; }
int Affliction::getDuration() { return duration; }
int Affliction::getValueIncrese() { return valueIncrese; }
int Affliction::getPassedTime() { return passedTime; }
int Affliction::getValue() { return value; }
AfflictionSystem *Affliction::getParent() { return parent; }

// ------------------AbilitySystem--------------------//

bool AbilitySystem::isInAbility(string name) {
  cout << "start isinability\n";
  cout << "ACess ability groub\n";
  abilityGroup.clear();
  cout << "yes";
  cout << "ACess ability groub\n";

  for (Ability *ability : abilityGroup)
  { cout << "start condition\n";
    if (ability->getName() == name)
    {
      cout << "find same\n";
      return true;
    }
  }
  cout << "finish loop\n";
  return false;
}

AbilitySystem::AbilitySystem(GameObject *parent)
    : parent(parent), abilityGroup({}) {}

bool AbilitySystem::addAbility(Ability *target) {
  cout << "AbilitySystem::addAbility(Ability *target)\n";
  cout << target->getName() << endl;
  if (!isInAbility(target->getName())) {
    cout << "add ability passed\n";
    abilityGroup.push_back(target);
    cout << "pushback finish\n";
    return true;
  }
  cout << "add ability failed\n";
  return false;
}

void AbilitySystem::decisionMaking(int sur, int env, int repo) {
  int maxValue, indexMax, index, value;
  maxValue = index = value = 0;
  indexMax = -1;
  for (Ability *ability : abilityGroup) {
    index++;
    ability->passive(ability->findTargetForPassive());
    if (ability->canActive(ability->findTargetForActive())) {
      value = ability->decision(sur, env, repo);
      if (maxValue < value) {
        maxValue = value;
        indexMax = index - 1;
      }
    }
  }
  if (indexMax > -1) {
    Ability *choosenAction = abilityGroup[indexMax];
    choosenAction->active(choosenAction->findTargetForActive());
  }
}

GameObject *AbilitySystem::getParent() { return parent; }

// ------------------Affliction--------------------//

AfflictionSystem::AfflictionSystem(GameObject *parent)
    : parent(parent) {}

void AfflictionSystem::recalculateValue() {
  StatusBlock *statBlock = getParent()->getStat();
  string affName;
  StatParam *paramTarget;
  statBlock->resetValue();
  for (Affliction *aff : afflictionGroup) {
    affName = aff->getTargetValue();
    paramTarget = statBlock->getParam(affName);
    if (paramTarget != nullptr) {
      paramTarget->changeValueBy(aff->getValue());
    }
  }
}

bool AfflictionSystem::addAffliction(Affliction *target) {
  afflictionGroup.push_back(target);
  recalculateValue();
  return true;
}

bool AfflictionSystem::removeAffliction(string name) {
  int index = 0;
  for (Affliction *aff : afflictionGroup) {
    index++;
    if (aff->getName() == name) {
      afflictionGroup.erase(afflictionGroup.begin() + index - 1);
      return true;
    }
  }
  return false;
}

bool AfflictionSystem::removeAllAffliction(string name) {
  int index = 0;
  bool have = false;
  for (Affliction *aff : afflictionGroup) {
    index++;
    if (aff->getName() == name) {
      afflictionGroup.erase(afflictionGroup.begin() + index - 1);
      have = true;
    }
  }
  return have;
}

int AfflictionSystem::amountOfAffliction(string name) {
  int amount = 0;
  for (Affliction *aff : afflictionGroup) {
    if (aff->getName() == name) {
      amount++;
    }
  }
  return amount;
}

void AfflictionSystem::updateAffliction() {
  int index = 0;
  for (Affliction *aff : afflictionGroup) {
    if (!(aff->update())) { // เหตุผลที่ไม่ใช้ remove ของตัว class เองก็เพราะว่าการ
      // remove ของตัว class เองจะ remove ตัวแรกที่มันเจอ
      // ไม่ใช่ตัวที่เราต้องการ
      afflictionGroup.erase(afflictionGroup.begin() + index - 1);
      delete aff;
    }
  }
  recalculateValue();
}

GameObject *AfflictionSystem::getParent() { return parent; }

bool AfflictionSystem::isInAffliction(string name) {
  for (Affliction *aff : afflictionGroup) {
    if (aff->getName() == name) {
      return true;
    }
  }
  return false;
}

// ------------------StatusBlock--------------------//

StatusBlock::StatusBlock(GameObject *parent)
    : parent(parent), AbilitySystem(parent), AfflictionSystem(parent) {}

bool StatusBlock::isInParam(string name) {
  for (StatParam *param : statParamGroup) {
    if (param->getName() == name) {
      return true;
    }
  }
  return false;
}

void StatusBlock::resetValue() {
  for (StatParam *stat : statParamGroup) {
    stat->resetValue();
  }
}

bool StatusBlock::addStat(StatParam *target) {
  string targetName = target->getName();
  StatParam *oldStat = getParam(targetName);
  if (oldStat != nullptr) {
    oldStat->pushStackInfo(&(target->getStackInfo()[0]));
    recalculateValue();
    return false;
  } else {
    statParamGroup.push_back(target);
    recalculateValue();
    return true;
  }
}

int StatusBlock::getParamValueDefault(string name) {
  for (StatParam *stat : statParamGroup) {
    if (name == stat->getName()) {
      return stat->getDefaultValue();
    }
  }
  return 0;
}

int StatusBlock::getParamValue(string name) {
  for (StatParam *stat : statParamGroup) {
    if (name == stat->getName()) {
      return stat->getValue();
    }
  }
  return 0;
}

StatParam *StatusBlock::getParam(string name) {
  for (StatParam *param : statParamGroup) {
    if (param->getName() == name) {
      return param;
    }
  }
  return nullptr;
}

GameObject *StatusBlock::getParent() { return parent; }

// ------------------GameObject--------------------//

GameObject::GameObject(Layer *parent, int x, int y) : parent(parent) {
  parent->setLayer(this, x, y);
}

int GameObject::getSur() {
  int current_health = stat->getParamValue("Health");
  int health_percentage =
      current_health * 100 / stat->getParamValueDefault("Health");
  int inverted_percentage = 100 - health_percentage;

  if (!stat->isInParam("Sight")) {
    return inverted_percentage > 100 ? 100 : inverted_percentage;
  }

  int sight = stat->getParamValue("Sight");
  vector<GameObject *> animals_in_range = findTargetInRange(sight, false);
  int sur = animals_in_range.size() > 0 ? inverted_percentage - 20
                                        : inverted_percentage;
  if (sur > 100) {
    sur = 100;
  }
  if (sur < 0) {
    sur = 0;
  }
  return sur;
}

int GameObject::getEnv() {
  int current_hunger = stat->getParamValue("Hunger");
  int hunger_percentage =
      current_hunger * 100 / stat->getParamValueDefault("Hunger");
  int inverted_percentage = 100 - hunger_percentage;
  return inverted_percentage > 100 ? 100 : inverted_percentage;
}

int GameObject::getRepo() {
  // If can't mate or on cooldown, get out
  if (!stat->isInAbility("Mate")) {
    return 0;
  }
  if (stat->isInAffliction("MateCooldown")) {
    return 0;
  }

  return 70;
}

void GameObject::update() {
  stat->decisionMaking(getSur(), getEnv(), getRepo());
}

pair<int, int> GameObject::getVectorIndex() {
  if (parent != nullptr) {
    const vector<vector<GameObject *>> layerVectors = parent->insideLayer;
    for (size_t i = 0; i < layerVectors.size(); ++i) {
      const vector<GameObject *> &currentVector = layerVectors[i];
      auto it = find(currentVector.begin(), currentVector.end(), this);
      if (it != currentVector.end()) {
        size_t j = distance(currentVector.begin(), it);
        return {static_cast<int>(j), static_cast<int>(i)};
      }
    }
  }
  // not found
  return {-2147483648, -2147483648};
}
pair<int, int> GameObject::getCoord() {
  if (parent) {
    pair<int, int> vector_index = getVectorIndex();
    LayerSystem *ls = parent->getParent();
    Ground ground = ls->getGround();
    Land floor = ground.insideLayer[vector_index.second][vector_index.first];
    return make_pair(floor.getX(), floor.getY());
  }
  // not found
  return make_pair(-2147483648, -2147483648);
}

void GameObject::setName(string stringInput) { name = stringInput; }
void GameObject::setRepresent(string stringInput) { represent = stringInput; }
string GameObject::getName() { return name; }
// inline string GameObject::getRepresent() { return represent; }
StatusBlock *GameObject::getStat() { return stat; }
Layer *GameObject::getParent() { return parent; }

// returns only GameObjects
vector<GameObject *> GameObject::findTargetInRange(int range, bool allLayers) {
  vector<GameObject *> target;
  pair<int, int> selfIndex = getVectorIndex();
  for (double i{0}; i <= range; i++) {
    for (double j{0}; j <= range; j++) {
      // if the selected tile is within range and isn't the same tile as the
      // GameObject:
      if (ceil(sqrt(pow(i, 2.) + pow(j, 2.))) <= range && (i != 0 || j != 0)) {
        // if the selected tile is more than or equal to 0 (not out of range)
        if (selfIndex.first - i >= 0 && selfIndex.second - j >= 0) {
          if (allLayers) // all layers in the same layer system
          {
            LayerSystem *layersystem = parent->getParent();
            for (size_t k = 0; k < layersystem->getLayersAmount(); k++) {
              Layer *layer = layersystem->getLayer(0);
              GameObject *targetObject = layer->getFromLayerIndex(
                  selfIndex.first - i, selfIndex.second - j);
              if (targetObject != nullptr) {
                target.push_back(targetObject);
              }
            }
          } else // GameObject's own layer only
          {
            GameObject *targetObject = parent->getFromLayerIndex(
                selfIndex.first - i, selfIndex.second - j);
            if (targetObject != nullptr) {
              target.push_back(targetObject);
            }
          }
        }
        // if the selected tile is less than the size (not out of range)
        if (selfIndex.first + i < parent->insideLayer.size() &&
            selfIndex.second + j < parent->insideLayer.size()) {
          if (allLayers) // all layers in the same layer system
          {
            LayerSystem *layersystem = parent->getParent();
            for (size_t k = 0; k < layersystem->getLayersAmount(); k++) {
              Layer *layer = layersystem->getLayer(0);
              GameObject *targetObject = layer->getFromLayerIndex(
                  selfIndex.first + i, selfIndex.second + j);
              if (targetObject != nullptr) {
                target.push_back(targetObject);
              }
            }
          } else // GameObject's own layer only
          {
            GameObject *targetObject = parent->getFromLayerIndex(
                selfIndex.first + i, selfIndex.second + j);
            if (targetObject != nullptr) {
              target.push_back(targetObject);
            }
          }
        }
      }
    }
  }
  return target;
}

// ------------------Layer--------------------//

Layer::Layer(string name, LayerSystem *parent, int width, int height)
    : parent(parent), name(name) {
  for (int y = 0; y < height; y++) {
    vector<GameObject *> row;
    for (int x = 0; x < width; x++) {
      row.push_back(nullptr);
    }
    insideLayer.push_back(row);
  }
}

// update everything in the layer
void Layer::action() {
  for (int i = 0; i < insideLayer.size(); i++) {
    for (int j = 0; j < insideLayer.at(i).size(); j++) {
      if (insideLayer.at(i).at(j) != nullptr) {
        insideLayer.at(i).at(j)->update();
      }
    }
  }
}

void Layer::setLayer(GameObject *target, int x, int y) {
  insideLayer.at(y).at(x) = target;
}

void Layer::removeFromLayer(int x, int y) { insideLayer.at(y).at(x) = nullptr; }
inline string Layer::getName() { return name; }
void Layer::setName(string nameInput) { name = nameInput; }
GameObject *Layer::getFromLayerIndex(int x, int y) {
  return insideLayer.at(y).at(x);
}

GameObject *Layer::getFromLayerCoord(int x, int y) {
  pair<int, int> targetIndex = parent->getGround().getVectorIndex(x, y);
  return getFromLayerIndex(targetIndex.first, targetIndex.second);
}

LayerSystem *Layer::getParent() { return parent; }

// ------------------Land--------------------//

Land::Land(Ground *parent, string represent, int x, int y)
    : parent(parent), represent(represent), x(x), y(y) {}

inline int Land::getX() { return x; }
inline int Land::getY() { return y; }

// ------------------Ground--------------------//

Ground::Ground(LayerSystem *p, int width, int height) : parent(p) {
  int width_min = static_cast<int>(-floor(width / 2));
  int height_min = static_cast<int>(-floor(height / 2));

  for (int y = height_min; y < height_min + height; y++) {
    vector<Land> row;
    for (int x = width_min; x < width_min + width; x++) {
      // casts ground pointer to layer pointer so that Land can be constructed
      row.push_back(Land(this, "#", x, y));
    }
    Ground::insideLayer.push_back(row);
  }
}

pair<int, int> Ground::getVectorIndex(int x, int y) {
  for (int i = 0; i < insideLayer.size(); ++i) {
    for (int j = 0; j < insideLayer[i].size(); ++j) {
      if (insideLayer[i][j].getX() == x && insideLayer[i][j].getY() == y) {
        return make_pair(j, i);
      }
    }
  }

  // -1 = not found
  return make_pair(-1, -1);
}

//-------------------LayerSystem-------------------//

bool LayerSystem::isInLayer(string name) {
  for (Layer *l : layers) {
    if (l->getName() == name) {
      return true;
    }
  }
  return false;
}

LayerSystem::LayerSystem(int width, int height)
    : width(width), height(height), ground(Ground(this, width, height)) {
  createNewLayer("Food");
  createNewLayer("Animal");
  createNewLayer("Plant");
  cout << "start random generate\n";
  randomGenerateLayers();
  cout << "finish random generate\n";
}

bool LayerSystem::createNewLayer(string name) {
  if (isInLayer(name)) {
    return false;
  }
  Layer *layer = new Layer(name, this, width, height);
  layers.push_back(layer);
  return true;
}

bool LayerSystem::removeLayer(string name) {
  for (int i = 0; i < layers.size(); i++) {
    if (layers.at(i)->getName() == name) {
      layers.erase(layers.begin() + i);
      return true;
    }
  }
  return false;
}

double LayerSystem::getDistance(GameObject *a, GameObject *b) {
  pair<int, int> aCoord = a->getCoord();
  pair<int, int> bCoord = b->getCoord();
  return getDistance(aCoord.first, aCoord.second, bCoord.first, bCoord.second);
}

double LayerSystem::getDistance(int x1, int y1, int x2, int y2) {
  return floor(abs(x1 - x2) + abs(y1 - y2));
}

inline Layer *LayerSystem::getLayer(string name) {
  for (Layer *l : layers) {
    if (l->getName() == name) {
      return l;
    }
  }
  return nullptr;
}

void LayerSystem::update() {
  for (Layer *l : layers) {
    l->action();
  }
}

inline Layer *LayerSystem::getLayer(int i) { return layers.at(i); }

inline Layer *LayerSystem::getRandomLayer() {
  return layers.at(rand() % layers.size());
}

Ground LayerSystem::getGround() { return ground; }

string inline LayerSystem::getLayerName(int i) {
  return layers.at(i)->getName();
}

int inline LayerSystem::getLayersAmount() { return layers.size(); }

void LayerSystem::printLayer() {
  Layer *env_layer = getLayer("Environment");
  Layer *animal_layer = getLayer("Animal");
  Layer *food_layer = getLayer("Food");

  for (int column = 0; column < height; column++) {
    for (int row = 0; row < width; row++) {
      if (env_layer->getFromLayerIndex(column, row) != nullptr) {
        cout << env_layer->getFromLayerIndex(column, row) << ' ';
      } else if (animal_layer->getFromLayerIndex(column, row) != nullptr) {
        cout << animal_layer->getFromLayerIndex(column, row) << ' ';
      } else if (food_layer->getFromLayerIndex(column, row) != nullptr) {
        cout << food_layer->getFromLayerIndex(column, row) << ' ';
      } else {
        cout << "x ";
      }
    }
    cout << endl;
  }
}

void LayerSystem::randomGenerateLayers() {

  Layer *env_layer = getLayer("Environment");
  Layer *animal_layer = getLayer("Animal");
  Layer *food_layer = getLayer("Food");

  cout << "finish get layer\n";

  for (int column = 0; column < height; column++) {
    for (int row = 0; row < width; row++) {
      int rand_animal = rand() % 100;
      int rand_food = rand() % 100;
      int rand_env = rand() % 100;

      cout << "finish get random\n";

      if (rand_animal < 10) {
        cout << "create obj\n";
        new Deer(animal_layer, row, column);
        cout << "create obj finished\n";
      }
      // if (rand_food < 1){
      //   cout << "create obj\n";
      //   new Corpse(food_layer, row, column);
      //   cout << "create obj finished\n";
      // }
      // if (rand_env < 10){
      //   int rand_plant = rand() % 2;
      //   if (rand_plant == 0){
      //     cout << "create obj\n";
      //     new Bush(env_layer, row, column);
      //     cout << "create obj finished\n";
      //   } else {
      //     cout << "create obj\n";
      //     new Apple_tree(env_layer, row, column);
      //     cout << "create obj finished\n";
      //   }
      // }
    }
  }
}

Health_Aff::Health_Aff(StatusBlock *parent, int duration, int valueIncrease,
                       int value)
    : Affliction(parent), duration(duration), valueIncrease(valueIncrease),
      value(value) {
        parent->addAffliction(this);
      }
Health_Aff::Health_Aff(GameObject *target, int duration, int valueIncrease,
                       int value)
    : Affliction(target), duration(duration), valueIncrease(valueIncrease),
      value(value) {}

Hunger_Aff::Hunger_Aff(StatusBlock *parent, int duration, int valueIncrease,
                       int value)
    : Affliction(parent), duration(duration), valueIncrease(valueIncrease),
      value(value) {
        parent->addAffliction(this);
      }
Hunger_Aff::Hunger_Aff(GameObject *target, int duration, int valueIncrease,
                       int value)
    : Affliction(target), duration(duration), valueIncrease(valueIncrease),
      value(value) {}

Attack_Aff::Attack_Aff(StatusBlock *parent, int duration, int valueIncrease,
                       int value)
    : Affliction(parent), duration(duration), valueIncrease(valueIncrease),
      value(value) {
        parent->addAffliction(this);
      }
Attack_Aff::Attack_Aff(GameObject *target, int duration, int valueIncrease,
                       int value)
    : Affliction(target), duration(duration), valueIncrease(valueIncrease),
      value(value) {}

Sight_Aff::Sight_Aff(StatusBlock *parent, int duration, int value)
    : Affliction(parent), duration(duration), value(value) {
      parent->addAffliction(this);
    }
Sight_Aff::Sight_Aff(GameObject *target, int duration, int value)
    : Affliction(target), duration(duration), value(value) {}

Speed_Aff::Speed_Aff(StatusBlock *parent, int duration, int valueIncrease,
                     int value)
    : Affliction(parent), duration(duration), valueIncrease(valueIncrease),
      value(value) {
        parent->addAffliction(this);
      }
Speed_Aff::Speed_Aff(GameObject *target, int duration, int valueIncrease,
                     int value)
    : Affliction(target), duration(duration), valueIncrease(valueIncrease),
      value(value) {}

MateCooldown::MateCooldown(StatusBlock *parent, int duration)
    : Affliction(parent), duration(duration) {
      parent->addAffliction(this);
    }
MateCooldown::MateCooldown(GameObject *target, int duration)
    : Affliction(target), duration(duration) {}

EatTimes::EatTimes(StatusBlock *parent) : Affliction(parent) {
  parent->addAffliction(this);
}
EatTimes::EatTimes(GameObject *target) : Affliction(target) {}

FruitionCooldown::FruitionCooldown(StatusBlock *parent, int duration)
    : Affliction(parent), duration(duration) {
      parent->addAffliction(this);
    }

FruitionCooldown::FruitionCooldown(GameObject *target, int duration)
    : Affliction(target), duration(duration) {}

Poisonous::Poisonous(StatusBlock *parent) : Affliction(parent) {
  parent->addAffliction(this);
}

Poisonous::Poisonous(GameObject *target) : Affliction(target) {}

// ================== stat ================== //

Hunger::Hunger(StatusBlock *parent, Ability *createBy, int rawValue)
    : StatParam(parent, createBy, rawValue) {}

Hunger::Hunger(GameObject *target, Ability *createBy, int rawValue)
    : StatParam(target, createBy, rawValue) {}

void Hunger::action() {
  // Animal dies here
  Layer *currentLayer = parent->getParent()->getParent();
  Layer *foodLayer = currentLayer->getParent()->getLayer("Food");
  pair<int, int> currentIndex = parent->getParent()->getVectorIndex();
  currentLayer->insideLayer[currentIndex.second][currentIndex.first] = nullptr;
  if (foodLayer->insideLayer[currentIndex.second][currentIndex.first] ==
      nullptr) {
    GameObject *corpseptr =
        new Corpse(foodLayer, currentIndex.first, currentIndex.second);
    // if animal is poisonous, give corpseptr poisonous too
    if (parent->isInAffliction("Poisonous")) {
      new Poisonous(corpseptr);
    }
  }
  // kill this animal :(
  delete parent->getParent();
  return;
};

Health::Health(StatusBlock *parent, Ability *createBy, int rawValue)
    : StatParam(parent, createBy, rawValue) {}

Health::Health(GameObject *target, Ability *createBy, int rawValue)
    : StatParam(target, createBy, rawValue) {}

void Health::action() {
  // Animal dies here
  Layer *currentLayer = parent->getParent()->getParent();
  Layer *foodLayer = currentLayer->getParent()->getLayer("Food");
  pair<int, int> currentIndex = parent->getParent()->getVectorIndex();
  currentLayer->insideLayer[currentIndex.second][currentIndex.first] = nullptr;
  if (foodLayer->insideLayer[currentIndex.second][currentIndex.first] ==
      nullptr) {
    GameObject *corpseptr =
        new Corpse(foodLayer, currentIndex.first, currentIndex.second);
    // if animal is poisonous, give corpseptr poisonous too
    if (parent->isInAffliction("Poisonous")) {
      new Poisonous(corpseptr);
    }
  }
  // kill this animal :(
  delete parent->getParent();
  return;
  return;
}

Atk::Atk(StatusBlock *parent, Ability *createBy, int rawValue)
    : StatParam(parent, createBy, rawValue) {}

Atk::Atk(GameObject *target, Ability *createBy, int rawValue)
    : StatParam(target, createBy, rawValue) {}

void Atk::action() { return; }

Sight::Sight(StatusBlock *parent, Ability *createBy, int rawValue)
    : StatParam(parent, createBy, rawValue) {}

Sight::Sight(GameObject *target, Ability *createBy, int rawValue)
    : StatParam(target, createBy, rawValue) {}

void Sight::action() { return; }

Speed::Speed(StatusBlock *parent, Ability *createBy, int rawValue)
    : StatParam(parent, createBy, rawValue) {}

Speed::Speed(GameObject *target, Ability *createBy, int rawValue)
    : StatParam(target, createBy, rawValue) {}

void Speed::action() { return; }

Eat::Eat(AbilitySystem *parent, int max_hunger, bool eatPlants,
         bool eatEarthworms, bool eatAnimals)
    : Ability(parent), max_hunger(max_hunger), eatPlants(eatPlants),
      eatEarthworms(eatEarthworms), eatAnimals(eatAnimals) {
        parent->addAbility(this);
      }

Eat::Eat(GameObject *target, int max_hunger, bool eatPlants, bool eatEarthworms,
         bool eatAnimals)
    : Ability(target), max_hunger(max_hunger), eatPlants(eatPlants),
      eatEarthworms(eatEarthworms), eatAnimals(eatAnimals) {}

void Eat::createStatParam() {
  new Hunger(getParent()->getParent(), this, max_hunger);
  new Hunger_Aff(getParent()->getParent(), max_hunger, -1, -1);
}

// Get earthworms
vector<GameObject *> Eat::findTargetForPassive() {
  if (!eatEarthworms) {
    return {};
  }
  vector<GameObject *> targetInRange =
      parent->getParent()->findTargetInRange(1, false);
  while (targetInRange.size() > 0) {
    if (targetInRange[0]->getName() != "Earthworm") {
      targetInRange.erase(targetInRange.begin());
    } else {
      break;
    }
  }
  return {targetInRange[0]};
}

// get corpses or plant
vector<GameObject *> Eat::findTargetForActive() {
  vector<GameObject *> target = {};
  GameObject *self = getParent()->getParent();
  pair<int, int> selfIndex = self->getVectorIndex();
  if (eatAnimals) {
    Layer *foodLayer =
        getParent()->getParent()->getParent()->getParent()->getLayer("Food");
    for (int coordX :
         {selfIndex.first - 1, selfIndex.first, selfIndex.first + 1}) {
      for (int coordY :
           {selfIndex.second - 1, selfIndex.second, selfIndex.second + 1}) {
        if (foodLayer->insideLayer[coordY][coordX]->getName() == "Corpse") {
          target.push_back(foodLayer->insideLayer[coordY][coordX]);
        }
      }
    }
  }

  if (eatPlants) {
    Layer *envLayer =
        getParent()->getParent()->getParent()->getParent()->getLayer(
            "Environment");
    for (int coordX :
         {selfIndex.first - 1, selfIndex.first, selfIndex.first + 1}) {
      for (int coordY :
           {selfIndex.second - 1, selfIndex.second, selfIndex.second + 1}) {
        GameObject *currentObj = envLayer->insideLayer[coordY][coordX];
        if (currentObj != nullptr) {
          if (currentObj->getName() == "Bush" ||
              currentObj->getName() == "Apple tree" ||
              currentObj->getName() == "Mushroom") {
            target.push_back(currentObj);
          }
        }
      }
    }
  }

  return target;
}

bool Eat::canActive(vector<GameObject *> target) {
  return findTargetForActive().size() > 0;
}

// Eating earthworms is passive
void Eat::passive(vector<GameObject *> target) {
  int increase_amount = 20;
  if (target.size() > 0) {
    GameObject *worm = target[0];
    pair<int, int> targetPosition = worm->getVectorIndex();
    Layer *currentLayer = parent->getParent()->getParent();
    // kill the earthworm
    currentLayer->insideLayer[targetPosition.second][targetPosition.first] =
        nullptr;
    delete worm;
    // increase hunger points
    new Hunger_Aff(parent->getParent(), 1, 20, 0);
  }
  return;
}

void Eat::active(vector<GameObject *> target) {
  if (target.size() <= 0) {
    return;
  }
  GameObject *realTarget = target[rand() % target.size()];
  Layer *foodLayer =
      getParent()->getParent()->getParent()->getParent()->getLayer("Food");
  // reset hunger
  getParent()->getParent()->getStat()->removeAffliction("Hunger_Aff");
  new Hunger_Aff(getParent()->getParent(), max_hunger, -1, -1);
  // heal
  new Health_Aff(getParent()->getParent(), 10, 5, 5);
  // if the target is poisonous, reduce health
  if (realTarget->getStat()->isInAffliction("Poisonous")) {
    new Health_Aff(parent->getParent(), 20, -10, -10);
  }
  // remove a stack of EatTimes
  realTarget->getStat()->removeAffliction("EatTimes");
}

//--------------------------- Attack ----------------------------//

Attack::Attack(AbilitySystem *parent, int atk_amount)
    : Ability(parent), atk_amount(atk_amount){
      parent->addAbility(this);
    };

Attack::Attack(GameObject *target, int atk_amount)
    : Ability(parent), atk_amount(atk_amount){};

// you need atk to attack
void Attack::createStatParam() {
  new Atk(getParent()->getParent(), this, atk_amount);
}

// no passives
vector<GameObject *> Attack::findTargetForPassive() { return {}; }

vector<GameObject *> Attack::findTargetForActive() {
  vector<GameObject *> possible_targets =
      parent->getParent()->findTargetInRange(1, false);
  return {possible_targets[rand() % possible_targets.size()]};
}

bool Attack::canActive(vector<GameObject *> targets) {
  vector<GameObject *> possible_targets =
      parent->getParent()->findTargetInRange(1, false);
  return (possible_targets.size() > 0);
}

// no passives yet
void Attack::passive(vector<GameObject *> targets) { return; }

// deal damage by inflicting bleed to all targets
void Attack::active(vector<GameObject *> targets) {
  for (GameObject *target : targets) {
    int atkValue = parent->getParent()->getStat()->getParamValue("Atk");
    new Health_Aff(target, atkValue, -atkValue, -atkValue);
  }
}

// --------------------------- Walk ---------------------------

Walk::Walk(AbilitySystem *parent) : Ability(parent) {
  // parent->addAbility(this);
}

Walk::Walk(GameObject *target) : Ability(target) {}

// walk doesn't scale with any stats, no stat creation needed
void Walk::createStatParam() { return; }

// Walking has no passives
vector<GameObject *> Walk::findTargetForPassive() { return {}; }

// Walk direction target is the closest target in sight
vector<GameObject *> Walk::findTargetForActive() {
  int visionRange = parent->getParent()->getStat()->getParamValue("Vision");
  vector<GameObject *> targets =
      parent->getParent()->findTargetInRange(visionRange, false);
  GameObject *closestObj = targets[0];
  for (GameObject *obj : targets) {
    int xDiff = parent->getParent()->getCoord().first - obj->getCoord().first;
    int yDiff = parent->getParent()->getCoord().second - obj->getCoord().second;
    int distance = sqrt(xDiff * xDiff + yDiff * yDiff);
    if (distance <
        sqrt(closestObj->getCoord().first * closestObj->getCoord().first +
             closestObj->getCoord().second * closestObj->getCoord().second)) {
      closestObj = obj;
    }
  }
  return {closestObj};
}

// if you have 8 or more in range, you can't walk
bool Walk::canActive(vector<GameObject *> targets) {
  int unwalkableTileCount = 0;
  Layer *selfLayer = parent->getParent()->getParent();
  pair<int, int> selfCoord = parent->getParent()->getVectorIndex();
  int currentX = selfCoord.first;
  int currentY = selfCoord.second;
  // check 8 tiles around the animal
  for (int coordY : {currentY - 1, currentY, currentY + 1}) {
    for (int coordX : {currentX - 1, currentX, currentX + 1}) {
      // if the coordinate is within bounds,
      if (coordX >= 0 && coordX < selfLayer->getParent()->getLayersWidth() &&
          coordY >= 0 && coordY < selfLayer->getParent()->getLayersHeight()) {
        // if the tile isn't nullptr and the tile isnt the same as the animal
        if (selfLayer->insideLayer[coordY][coordX] != nullptr &&
            (coordX != currentX || coordY != currentY)) {
          unwalkableTileCount++;
        }
      } else // if the coordinate is out of bounds, it is unwalkable
      {
        unwalkableTileCount++;
      }
    }
  }

  return (unwalkableTileCount < 8);
}

// no passives
void Walk::passive(vector<GameObject *> targets) { return; }

// --------------------------- WalkSeek ---------------------------

WalkSeek::WalkSeek(AbilitySystem *parent) : Walk(parent) {
  parent->addAbility(this);
}

WalkSeek::WalkSeek(GameObject *target) : Walk(parent) {}

// walk by setting another unoccupied tile to be this object's pointer and set
// the previous tile to nullptr
void WalkSeek::active(vector<GameObject *> targets) {
  Layer *currentLayer = parent->getParent()->getParent();
  int currentX = parent->getParent()->getVectorIndex().first;
  int currentY = parent->getParent()->getVectorIndex().second;
  // code to choose a target
  int targetX, targetY;
  if (targets.size() >
      0) // if there's a target, set target coords to the variables
  {
    GameObject *target = targets[0];
    targetX = target->getVectorIndex().first;
    targetY = target->getVectorIndex().second;
  } else // else, set target coords a random 8 tile around the parent
  {
    // all coordinates around the parent that isn't out of bounds
    vector<pair<int, int>> possibleTargets;

    for (int coordY : {currentY - 1, currentY, currentY + 1}) {
      for (int coordX : {currentX - 1, currentX, currentX + 1}) {
        // if x and y are in range, if the tile is empty, and isn't the same as
        // the parent
        if (coordX >= 0 &&
            coordX < currentLayer->getParent()->getLayersWidth() &&
            coordY >= 0 &&
            coordY < currentLayer->getParent()->getLayersHeight() &&
            currentLayer->insideLayer[coordY][coordX] == nullptr &&
            (coordX != currentX || coordY != currentY)) {
          possibleTargets.push_back({coordX, coordY});
        }
      }
    }

    // randomize target coords
    if (possibleTargets.size() > 0) {
      pair<int, int> targetPosition =
          possibleTargets[rand() % possibleTargets.size()];
      targetX = targetPosition.first;
      targetY = targetPosition.second;
    } else {
      targetX = currentX;
      targetY = currentY;
    }
  }

  // code to walk towards the selected target
  int xDiff = targetX - currentX;
  int yDiff = targetY - currentY;
  int nextX = currentX;
  int nextY = currentY;
  // walk in the target's direction
  if (xDiff > 0) // target to the right
  {
    nextX = currentX + 1; // walk right
  }
  if (xDiff < 0) {
    nextX = currentX - 1;
  }
  if (yDiff > 0) // target below (go down = increased Y)
  {
    nextY = currentY + 1; // walk down
  }
  if (yDiff < 0) {
    nextY = currentY - 1;
  }
  // if the next position is empty, set the next position to be the parent's
  // pointer
  if (currentLayer->insideLayer[nextY][nextX] == nullptr) {
    currentLayer->setLayer(parent->getParent(), nextX, nextY);
    currentLayer->setLayer(nullptr, currentX, currentY);
  }

  return;
}

// --------------------------- WalkEscape ---------------------------

WalkEscape::WalkEscape(AbilitySystem *parent) : Walk(parent) {
  parent->addAbility(this);
}

WalkEscape::WalkEscape(GameObject *target) : Walk(parent) {}

void WalkEscape::active(vector<GameObject *> targets) {
  Layer *currentLayer = parent->getParent()->getParent();
  int currentX = parent->getParent()->getVectorIndex().first;
  int currentY = parent->getParent()->getVectorIndex().second;
  // code to choose a target
  int targetX, targetY;
  if (targets.size() >
      0) // hopefully there's always going to be a target to escape from?
  {
    GameObject *target = targets[0];
    targetX = target->getVectorIndex().first;
    targetY = target->getVectorIndex().second;
  } else // else, set target coords a random 8 tile around the parent
  {
    // all coordinates around the parent that isn't out of bounds
    vector<pair<int, int>> possibleTargets;

    for (int coordY : {currentY - 1, currentY, currentY + 1}) {
      for (int coordX : {currentX - 1, currentX, currentX + 1}) {
        // if x and y are in range, if the tile is empty, and isn't the same as
        // the parent
        if (coordX >= 0 &&
            coordX < currentLayer->getParent()->getLayersWidth() &&
            coordY >= 0 &&
            coordY < currentLayer->getParent()->getLayersHeight() &&
            currentLayer->insideLayer[coordY][coordX] == nullptr &&
            coordX != currentX && coordY != currentY) {
          possibleTargets.push_back({coordX, coordY});
        }
      }
    }

    // randomize target coords until they aren't the same as current coords
    do {
      pair<int, int> targetPosition =
          possibleTargets[rand() % possibleTargets.size()];
      targetX = targetPosition.first;
      targetY = targetPosition.second;
    } while (targetX == currentX && targetY == currentY);
  }

  int xDiff = currentX - targetX;
  int yDiff = currentY - targetY;
  int nextX = currentX;
  int nextY = currentY;
  // walk in the opposite direction
  if (xDiff > 0) // currently to the right of the target
  {
    nextX = currentX - 1; // walk left
  }
  if (xDiff < 0) {
    nextX = currentX + 1;
  }
  if (yDiff > 0) // currently below the target
  {
    nextY = currentY + 1; // walk down
  }
  if (yDiff < 0) {
    nextY = currentY - 1;
  }

  // if target position is in bounds and is empty, move there
  if (nextX >= 0 && nextX < currentLayer->getParent()->getLayersWidth() &&
      nextY >= 0 && nextY < currentLayer->getParent()->getLayersHeight() &&
      currentLayer->insideLayer[nextY][nextX] != nullptr) {
    currentLayer->setLayer(parent->getParent(), nextX, nextY);
    currentLayer->setLayer(nullptr, currentX, currentY);
  }

  return;
}

//--------------------------- Mate ---------------------------

Mate::Mate(AbilitySystem *parent) : Ability(parent){
  parent->addAbility(this);
};

Mate::Mate(GameObject *target) : Ability(parent) {}

// no stats neede
void Mate::createStatParam() { return; }

// no passives
vector<GameObject *> Mate::findTargetForPassive() { return {}; }

// 1 random target of the same species within range
vector<GameObject *> Mate::findTargetForActive() {
  // get all animals in range
  vector<GameObject *> animals_in_range =
      parent->getParent()->findTargetInRange(1, false);
  vector<GameObject *> targets;
  for (GameObject *target : animals_in_range) {
    // if same species
    if (target->getName() == parent->getParent()->getName()) {
      targets.push_back(target);
    }
  }
  return targets;
};

// at least 1 tile around must be empty, contain the same species, and ability
// not on cooldown
bool Mate::canActive(vector<GameObject *> targets) {
  // same algorithm as canActive for walk, counting how many empty tiles there
  // are around
  int unspawnableTile = 0;
  Layer *selfLayer = parent->getParent()->getParent();
  pair<int, int> selfCoord = parent->getParent()->getVectorIndex();
  int currentX = selfCoord.first;
  int currentY = selfCoord.second;
  for (int coordY : {currentY - 1, currentY, currentY + 1}) {
    for (int coordX : {currentX - 1, currentX, currentX + 1}) {
      if (coordX >= 0 && coordX < selfLayer->getParent()->getLayersWidth() &&
          coordY >= 0 && coordY < selfLayer->getParent()->getLayersHeight()) {
        if (selfLayer->insideLayer[coordY][coordX] != nullptr &&
            (coordX != currentX || coordY != currentY)) {
          unspawnableTile++;
        }
      } else {
        unspawnableTile++;
      }
    }
  }

  return !(parent->getParent()->getStat()->isInAffliction("MateCooldown")) &&
         findTargetForActive().size() > 0 && unspawnableTile < 8;
};

// no passives
void Mate::passive(vector<GameObject *> targets) { return; }

void Mate::active(vector<GameObject *> targets) {
  // find all nullptr tiles in the 8 tiles around
  vector<pair<int, int>> possibleTargets;

  pair<int, int> selfCoord = parent->getParent()->getVectorIndex();
  int currentX = selfCoord.first;
  int currentY = selfCoord.second;
  Layer *currentLayer = parent->getParent()->getParent();

  for (int coordY : {currentY - 1, currentY, currentY + 1}) {
    for (int coordX : {currentX - 1, currentX, currentX + 1}) {
      // if x and y are in range, if the tile is empty, and isn't the same as
      // the parent
      if (coordX >= 0 && coordX < currentLayer->getParent()->getLayersWidth() &&
          coordY >= 0 &&
          coordY < currentLayer->getParent()->getLayersHeight() &&
          currentLayer->insideLayer[coordY][coordX] == nullptr &&
          (coordX != currentX || coordY != currentY)) {
        possibleTargets.push_back({coordX, coordY});
      }
    }
  }

  if (possibleTargets.size() > 0) {
    pair<int, int> targetCoord =
        possibleTargets[rand() % possibleTargets.size()];
    string animalType = parent->getParent()->getName();
    // apply cooldown affliction
    new MateCooldown(parent->getParent(), 100);
    new MateCooldown(targets[0], 100);

    // can't use switch, so I'm gonna use if-elses
    // if (animalType == "Lion"){

    // }

    return;
  }
}

// ===================================== CORPSES =====================================

//--------------------------- Rot ---------------------------

Rot::Rot(AbilitySystem *target) : Ability(target) {
  parent->addAbility(this);
}

Rot::Rot(GameObject *target) : Ability(target) {}

void Rot::createStatParam(){};

vector<GameObject *> Rot::findTargetForPassive() { return {}; }

vector<GameObject *> Rot::findTargetForActive() { return {}; }

bool Rot::canActive(vector<GameObject *> targets) { return false; }

void Rot::passive(vector<GameObject *> targets) {
  rot_timer++;
  Layer *animalLayer =
      parent->getParent()->getParent()->getParent()->getLayer("Animal");
  Layer *EnvironmentLayer =
      parent->getParent()->getParent()->getParent()->getLayer("Environment");

  vector<pair<int, int>> possibleFoodCoord;
  vector<pair<int, int>> possibleEnvCoord;
  // when rot timer is reached, spawn mushroom and earthworm
  if (rot_timer == 60) {
    pair<int, int> selfCoord = parent->getParent()->getVectorIndex();
    int currentX = selfCoord.first;
    int currentY = selfCoord.second;
    for (int coordY : {currentY - 1, currentY, currentY + 1}) {
      for (int coordX : {currentX - 1, currentX, currentX + 1}) {
        // if coord in range
        if (coordX >= 0 &&
            coordX < parent->getParent()
                         ->getParent()
                         ->getParent()
                         ->getLayersWidth() &&
            coordY >= 0 &&
            coordY < parent->getParent()
                         ->getParent()
                         ->getParent()
                         ->getLayersHeight()) {
          if (animalLayer->insideLayer[coordY][coordX] == nullptr) {
            possibleFoodCoord.push_back({coordX, coordY});
          }
          if (EnvironmentLayer->insideLayer[coordY][coordX] == nullptr) {
            possibleEnvCoord.push_back({coordX, coordY});
          }
        }
      }
    }
  }
  if (possibleFoodCoord.size() > 0) {
    pair<int, int> foodCoord =
        possibleFoodCoord[rand() % possibleFoodCoord.size()];
  }
  if (possibleEnvCoord.size() > 0) {
    pair<int, int> envCoord =
        possibleEnvCoord[rand() % possibleEnvCoord.size()];
    new Mushroom(EnvironmentLayer, envCoord.first, envCoord.second);
  }
}

void Rot::active(vector<GameObject *> targets) {}

// ===================================== PLANTS
// =====================================

// --------------------------- Fruition ---------------------------

Fruition::Fruition(AbilitySystem *target) : Ability(target) {
  parent->addAbility(this);
}

Fruition::Fruition(GameObject *target) : Ability(target) {}

void Fruition::createStatParam(){};

vector<GameObject *> Fruition::findTargetForPassive() { return {}; }

vector<GameObject *> Fruition::findTargetForActive() { return {}; };

// no active, always false
bool Fruition::canActive(vector<GameObject *> targets) { return false; }

// Ima store fruits as afflictions actually
void Fruition::passive(vector<GameObject *> targets) {
  if (!parent->getParent()->getStat()->isInAffliction(
          "FruitionCooldown")) { // if not on cooldown
    int fruitAmount =
        parent->getParent()->getStat()->amountOfAffliction("Fruit");
    if (fruitAmount < 3) {
      // generate fruits, set ability on cooldown
      new FruitionCooldown(parent->getParent(), 200);
      new EatTimes(parent->getParent());
    }
  }
  return;
}

void Fruition::active(vector<GameObject *> targets) { return; }

// --------------------------- SingleEat ---------------------------

SingleEat::SingleEat(AbilitySystem *target) : Ability(target) {
  parent->addAbility(this);
}

SingleEat::SingleEat(GameObject *target) : Ability(target) {}

void SingleEat::createStatParam() { return; }

vector<GameObject *> SingleEat::findTargetForPassive() { return {}; }

vector<GameObject *> SingleEat::findTargetForActive() { return {}; }

// no active, always false
bool SingleEat::canActive(vector<GameObject *> targets) { return false; }

// death when eaten
void SingleEat::passive(vector<GameObject *> targets) {
  StatusBlock *statBlock = parent->getParent()->getStat();
  if (statBlock->amountOfAffliction("EatTimes") <= 0) {
    Layer *selfLayer = parent->getParent()->getParent();
    pair<int, int> selfCoord = parent->getParent()->getVectorIndex();
    selfLayer->insideLayer[selfCoord.second][selfCoord.first] = nullptr;
    delete parent->getParent();
  }
}

// no active
void SingleEat::active(vector<GameObject *> targets) {}

Animal::Animal(Layer *parent, int x, int y) : GameObject(parent, x, y) {
  // abilities all aniimals can do
  cout << "Animal created\n";
  new WalkSeek(this);
  cout << "WalkSeek created\n";
  new WalkEscape(this);
  cout << "WalkEscape created\n";
  new Mate(this);
  cout << "Mate created\n";
  // mate cooldown on spawn
  new MateCooldown(this, 200);
  cout << "MateCooldown created\n";
}

Plant::Plant(Layer *parent, int x, int y) : GameObject(parent, x, y) {}

Corpse::Corpse(Layer *parent, int x, int y) : GameObject(parent, x, y) {
  // Make Corpses have 3 eat times and make it single eat
  new SingleEat(this);
  new EatTimes(this);
  new EatTimes(this);
  new EatTimes(this);
  // Corpses rot
  new Rot(this);
}

Deer::Deer(Layer *parent, int x, int y) : Animal(parent, x, y) {
  // All animals need these stats:
  // Health, Sight, Speed
  cout << "Deer created\n";
  new Health(this, nullptr, 100);
  cout << "Health created\n";
  new Sight(this, nullptr, 10);
  new Speed(this, nullptr, 3);

  // created by abilities: Hunger
  // Deers can eat
  new Eat(this, 120, true, false, false);
}

Bush::Bush(Layer *parent, int x, int y) : Plant(parent, x, y) {
  // Bushes get SingleEat
  new SingleEat(this);
  new EatTimes(this);
}

Mushroom::Mushroom(Layer *parent, int x, int y) : Plant(parent, x, y) {
  // Mushroom get SingleEat
  new SingleEat(this);
  new EatTimes(this);
}

Apple_tree::Apple_tree(Layer *parent, int x, int y) : Plant(parent, x, y) {
  // Can regen EatTimes with Fruition, get 3 EatTimes when created
  new Fruition(this);
  new EatTimes(this);
  new EatTimes(this);
  new EatTimes(this);
}

EarthWorm::EarthWorm(Layer *parent, int x, int y) : GameObject(parent, x, y) {
  // can only walk, randomly
  new WalkSeek(this);
}

Grid::Grid(Vector2f size, Vector2f position, GameObject *object)
    : object(object) {
  shape.setSize(size);
  shape.setPosition(position);
  shape.setOutlineThickness(2.f);
  shape.setOutlineColor(COLOR5);
  shape.setFillColor(COLOR4);
}

void Grid::draw(RenderWindow &window) { window.draw(shape); }

GameObject *Grid::getObject() { return object; }

//---------------------------------Map---------------------------------

void Map::update() {
  gridMap.clear();
  showText.clear();
  for (int i = 0; i < row; i++) {
    vector<Text> tempTextVec;
    vector<Grid> temp;
    for (int j = 0; j < column; j++) {
      Grid tempGrid(Vector2f((float)width / column, (float)height / row),
                    Vector2f(border.getPosition().x + j * (float)width / column,
                             border.getPosition().y + i * (float)height / row),
                    dataMap[center.x - floor(row / 2) + i]
                           [center.y - floor(column / 2) + j]);
      tempText.setFont(font);
      tempText.setCharacterSize((float)height / row);
      tempText.setFillColor(COLOR5);
      dataMap[center.x - floor(row / 2) + i][center.y - floor(column / 2) + j]
          ? tempText.setString(dataMap[center.x - floor(row / 2) + i]
                                      [center.y - floor(column / 2) + j]
                                          ->getRepresent())
          : tempText.setString("G");
      tempText.setPosition(
          Vector2f(border.getPosition().x + j * (float)width / column,
                   border.getPosition().y + i * (float)height / row));

      tempTextVec.push_back(tempText);
      temp.push_back(tempGrid);
    }
    gridMap.push_back(temp);
    showText.push_back(tempTextVec);
  }
}

Map::Map(Vector2f size, Vector2f position, vector<vector<GameObject *>> dataMap)
    : width(size.x), height(size.y), dataMap(dataMap), row(9), column(15),
      center(Vector2i(ceil(dataMap.size() / 2), ceil(dataMap[0].size() / 2))) {
  border.setSize(Vector2f(width, height));
  border.setFillColor(Color::Transparent);
  border.setOutlineColor(COLOR5);
  border.setOutlineThickness(2.0f);
  border.setPosition(position);
  font.loadFromFile(FONT_PATH);
  update();
}

void Map::draw(RenderWindow &window) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < column; j++) {
      gridMap[i][j].draw(window);
      window.draw(showText[i][j]);
    }
  }
  window.draw(border);
}

void Map::setDataMap(vector<vector<GameObject *>> dataMap) {
  this->dataMap = dataMap;
}

void Map::setRowColumn(Vector2i size) {
  this->row = size.x;
  this->column = size.y;
}

GameObject *Map::getObject(Vector2f position) {
  float gridHeight = (float)height / row;
  float gridWidth = (float)width / column;
  for (int i = 0; i < row; i++) {
    if (position.y >= i * gridHeight && position.y < (i + 1) * gridHeight) {
      for (int j = 0; j < column; j++) {
        if (position.x >= j * gridWidth && position.x < (j + 1) * gridWidth) {
          return gridMap[i][j].getObject();
        }
      }
    }
  }
  return nullptr;
}

void Map::changeCenter(Vector2i center) { this->center = center; }

Vector2i Map::getRowColumn() { return Vector2i(row, column); }
Vector2i Map::getSize() { return Vector2i(width, height); }
Vector2i Map::getCenter() { return center; }
vector<vector<GameObject *>> Map::getDataMap() { return dataMap; }

//---------------------------------ZoomIn---------------------------------

ZoomIn::ZoomIn(Vector2f position, int radius) {
  font.loadFromFile(FONT_PATH);
  shape.setOutlineColor(COLOR5);
  shape.setOutlineThickness(2.0f);
  text.setString("+");
  text.setFillColor(COLOR5);
  shape.setRadius(radius);
  shape.setPosition(position);
  text.setFont(font);
  Vector2f textPosition = Vector2f(position.x + 3, position.y - 12);
  text.setPosition(textPosition);
  shape.setFillColor(Color::Green);
}

void ZoomIn::draw(RenderWindow &window) {
  window.draw(shape);
  window.draw(text);
}

CircleShape ZoomIn::getShape() { return shape; }

void ZoomIn::zoomIn(Map &map) {
  Vector2i mapRowColumn = map.getRowColumn();
  if (mapRowColumn.y - 30 < 15 || mapRowColumn.x - 18 < 9) {
    return;
  }
  map.setRowColumn(Vector2i(mapRowColumn.x - 18, mapRowColumn.y - 30));
}

//---------------------------------ZoomOut---------------------------------

ZoomOut::ZoomOut(Vector2f position, int radius) : ZoomIn(position, radius) {
  shape.setFillColor(Color::Red);
  text.setString("-");
}

void ZoomOut::zoomOut(Map &map) {
  Vector2i mapRowColumn = map.getRowColumn();
  Vector2i mapCenter = map.getCenter();
  if (mapCenter.x - floor(mapRowColumn.x + 30 / 2) < 0 ||
      mapCenter.y - floor(mapRowColumn.y + 18 / 2) < 0 ||
      mapCenter.x + ceil(mapRowColumn.x + 30 / 2) > map.getDataMap().size() ||
      mapCenter.y + ceil(mapRowColumn.y + 18 / 2) >
          map.getDataMap()[0].size()) {
    return;
  }
  map.setRowColumn(Vector2i(mapRowColumn.x + 18, mapRowColumn.y + 30));
}

//---------------------------------LayerButton---------------------------------

LayerButton::LayerButton(Vector2f position, Vector2f size, string text,
                         vector<vector<GameObject *>> dataMap)
    : dataMap(dataMap) {
  font.loadFromFile(FONT_PATH);
  button.setPosition(position);
  button.setSize(size);
  button.setFillColor(COLOR3);
  button.setOutlineColor(COLOR5);
  button.setOutlineThickness(2.0f);
  /*คำนวน size ของ text ตาม size ของ button*/
  this->text.setString(text);
  // this->text.setFont(this->font); // i don't know why this line make error
  this->text.setCharacterSize(24); // เดะแก้ทีหลัง
  this->text.setFillColor(COLOR5);
  this->text.setPosition(position.x + 10, position.y + 10);
}

void LayerButton::draw(RenderWindow &window) {
  window.draw(button);
  window.draw(this->text);
}

void LayerButton::setDataMap(vector<vector<GameObject *>> dataMap) {
  this->dataMap = dataMap;
}
void LayerButton::pushToMap(Map &map) { map.setDataMap(dataMap); }

//---------------------------------LayerSheet---------------------------------

void LayerSheet::dumbToButton(vector<vector<GameObject *>> DataMap,
                              LayerButton &button) {
  button.setDataMap(DataMap);
}

LayerSheet::LayerSheet(LayerSystem *data, Vector2f position, Vector2f size)
    : data(data) {
  /*คำนวนขอบ*/
  font.loadFromFile(FONT_PATH);
  sheet.setPosition(position);
  sheet.setSize(size);
  sheet.setFillColor(Color::Transparent);
  sheet.setOutlineColor(COLOR5);
  sheet.setOutlineThickness(2.0f);
  float buttonWidth = size.x / ((data->layers.size() + 1) * (1 + 0.1));
  for (int i = 0; i < data->layers.size(); i++) {
    Text tempText;
    tempText.setFont(font);
    tempText.setCharacterSize(72);
    tempText.setFillColor(COLOR5);
    tempText.setString(data->layers[i]->getName());
    tempText.setPosition(Vector2f(
        position.x + (i * (buttonWidth + (0.1 * buttonWidth))), position.y));
    text.push_back(tempText);
    LayerButton tempButton(
        Vector2f(position.x + (i * (buttonWidth + (0.1 * buttonWidth))),
                 position.y),
        Vector2f(buttonWidth, size.y), data->layers[i]->getName(),
        data->layers[i]->insideLayer);
    buttons.push_back(tempButton);
  }
  Text tempTextz;
  tempTextz.setFont(font);
  tempTextz.setCharacterSize(72);
  tempTextz.setFillColor(COLOR5);
  tempTextz.setString("OverAll");
  tempTextz.setPosition(Vector2f(
      position.x + (3 * (buttonWidth + (0.1 * buttonWidth))), position.y));
  text.push_back(tempTextz);
  LayerButton tempButtonz(
      Vector2f(position.x + (3 * (buttonWidth + (0.1 * buttonWidth))),
               position.y),
      Vector2f(buttonWidth, size.y), "OverAll", data->getOverAllLayer());
  buttons.push_back(tempButtonz);
  selectedbutton = &(buttons[0]);
}

void LayerSheet::draw(RenderWindow &window) {
  window.draw(sheet);
  for (int i = 0; i < buttons.size(); i++) {
    buttons[i].draw(window);
    window.draw(text[i]);
  }
}

void LayerSheet::setSelectedButton(int i) { selectedbutton = &buttons[i]; }

void LayerSheet::update() {
  data->update();
  for (int i = 0; i < buttons.size(); i++) {
    if (i == buttons.size() - 1) {
      dumbToButton(data->getOverAllLayer(), buttons[0]);
    } else {
      dumbToButton(data->layers[i]->insideLayer, buttons[i]);
    }
  }
}

//---------------------------------MiniMap---------------------------------

//---------------------------------StopButton---------------------------------

StopButton::StopButton(Vector2f position, Vector2f size) {
  font.loadFromFile(FONT_PATH);
  text.setFont(font);
  text.setCharacterSize(24);
  text.setString("-");
  text.setFillColor(COLOR5);
  text.setPosition(position.x + 5, position.y - 5);
  shape.setPosition(position);
  shape.setSize(size);
  shape.setFillColor(Color::Red);
  shape.setOutlineColor(COLOR5);
  shape.setOutlineThickness(2.0f);
}

void StopButton::draw(RenderWindow &window) {
  window.draw(shape);
  window.draw(text);
}

RectangleShape StopButton::getShape() { return shape; }

//---------------------------------PlayButton---------------------------------

PlayButton::PlayButton(Vector2f position, Vector2f size)
    : StopButton(position, size) {
  text.setString("+");
  shape.setPosition(position);
  shape.setSize(size);
  shape.setFillColor(Color::Green);
  text.setPosition(position.x + 5, position.y - 6);
}

//---------------------------------RunButton---------------------------------

RunButton::RunButton(Vector2f position, Vector2f size)
    : StopButton(position, size) {
  text.setString(">");
  shape.setPosition(position);
  shape.setSize(size);
  shape.setFillColor(Color::Green);
}

//---------------------------------TextBox---------------------------------

TextBox::TextBox(Vector2f position, Vector2f size, vector<string> defaultVec,
                 int textCapacity, int showTextCapacity)
    : textCapacity(textCapacity), showTextCapacity(showTextCapacity),
      showTextIndex(0), allText(defaultVec) {
  font.loadFromFile(FONT_PATH);
  shape.setPosition(position);
  shape.setSize(size);
  shape.setFillColor(COLOR3);
  shape.setOutlineColor(COLOR5);
  shape.setOutlineThickness(2.0f);
  for (int i = 0; i < showTextCapacity; i++) {
    Text tempText;
    tempText.setFont(font);
    tempText.setCharacterSize(floor(size.y / showTextCapacity));
    tempText.setFillColor(COLOR5);
    tempText.setPosition(position.x,
                         position.y + ((size.y / showTextCapacity) * i));
    showText.push_back(tempText);
  }
  update();
  up.setSize(Vector2f(20, 20));
  up.setPosition(Vector2f(position.x + size.x - 20, position.y));
  up.setFillColor(Color::Green);
  up.setOutlineColor(COLOR5);
  up.setOutlineThickness(2.0f);
  textUp.setFont(font);
  textUp.setCharacterSize(32);
  textUp.setFillColor(COLOR5);
  textUp.setString("+");
  textUp.setPosition(Vector2f(position.x + size.x - 16, position.y - 12));
  down.setSize(Vector2f(20, 20));
  down.setPosition(
      Vector2f(position.x + size.x - 20, position.y + size.y - 20));
  down.setFillColor(Color::Green);
  down.setOutlineColor(COLOR5);
  down.setOutlineThickness(2.0f);
  textDown.setFont(font);
  textDown.setCharacterSize(32);
  textDown.setFillColor(COLOR5);
  textDown.setString("-");
  textDown.setPosition(
      Vector2f(position.x + size.x - 16, position.y + size.y - 30));
}

void TextBox::update() {
  for (int i = 0; i < showTextCapacity; i++) {
    showText[i].setString(allText[showTextIndex + i]);
  }
}

void TextBox::draw(RenderWindow &window) {
  window.draw(shape);
  for (int i = 0; i < showText.size(); i++) {
    window.draw(showText[i]);
  }
  window.draw(up);
  window.draw(down);
  window.draw(textUp);
  window.draw(textDown);
}

void TextBox::dumbText(string text) {
  allText.push_back(text);
  if (allText.size() > textCapacity) {
    allText.erase(allText.begin());
  }
  if (showTextIndex < allText.size() - textCapacity) {
    showTextIndex = allText.size() - textCapacity;
  }
  update();
}

void TextBox::dumbText(vector<string> text) {
  for (int i = 0; i < text.size(); i++) {
    allText.push_back(text[i]);
    if (allText.size() > textCapacity) {
      allText.erase(allText.begin());
    }
    if (showTextIndex < allText.size() - textCapacity) {
      showTextIndex = allText.size() - textCapacity;
    }
  }
  update();
}

RectangleShape TextBox::getShape() { return shape; }

void TextBox::shiftShowTextUp() {
  if (showTextIndex > 0) {
    showTextIndex--;
    update();
  }
}

void TextBox::shiftShowTextDown() {
  if (showTextIndex + showTextCapacity < allText.size()) {
    showTextIndex++;
    update();
  }
}

//---------------------------------Detail---------------------------------

void Detail::update() {
  if (object) {
    setAllText(object->getDetail());
    TextBox::update();
  }
}

Detail::Detail(Vector2f position, Vector2f size)
    : TextBox(position, size,
              {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11 "}, 100,
              10) {
  deleteButton.setRadius(10);
  deleteButton.setFillColor(Color::Red);
  deleteButton.setPosition(
      Vector2f(shape.getPosition().x + shape.getSize().x - 50,
               shape.getPosition().y + 10));
  deleteButton.setOutlineColor(COLOR5);
  deleteButton.setOutlineThickness(2.0f);
}

void Detail::setObject(GameObject *object) {
  this->object = object;
  update();
}

void Detail::draw(RenderWindow &window) {
  TextBox::draw(window);
  if (object) {
    window.draw(deleteButton);
  }
}

CircleShape Detail::getDeleteButton() { return deleteButton; }

//---------------------------------Log---------------------------------

void Log::update() {
  // ไปดึงข้อมูลการกระทำของสัตว์มาแสดง
}

Log::Log(Vector2f position, Vector2f size)
    : TextBox(position, size,
              {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"}, 500,
              10) {
  DefaultHeight = position.y;
  shape.setFillColor(COLOR1);
}

//---------------------------------SelectedList---------------------------------

void SelectedList::update() {
  for (int i = 0; i < showTextCapacity; i++) {
    if (showTextIndex + i < allObject.size()) {
      showText[i].setString(allObject[showTextIndex + i]->getRepresent());
    } else {
      showText[i].setString(" ");
    }
  }
}

SelectedList::SelectedList(Vector2f position, Vector2f size,
                           vector<GameObject *> allObject, int showTextCapacity)
    : showTextIndex(0), showTextCapacity(showTextCapacity),
      allObject(allObject) {
  font.loadFromFile(FONT_PATH);
  shape.setPosition(position);
  shape.setSize(size);
  shape.setFillColor(COLOR1);
  shape.setOutlineColor(COLOR5);
  shape.setOutlineThickness(2.0f);
  for (int i = 0; i < showTextCapacity; i++) {
    tempText.setFont(font);
    tempShape.setSize(Vector2f(size.x, size.y / showTextCapacity));
    tempShape.setPosition(position.x,
                          position.y + (size.y * i / showTextCapacity));
    tempShape.setFillColor(COLOR1);
    tempShape.setOutlineColor(COLOR5);
    tempShape.setOutlineThickness(3.0f);
    showShape.push_back(tempShape);
    tempText.setCharacterSize(size.y / showTextCapacity);
    tempText.setFillColor(COLOR5);
    tempText.setPosition(position.x + 10,
                         position.y + (size.y * i / showTextCapacity));
    showText.push_back(tempText);
  }
  update();
  up.setSize(Vector2f(20, 20));
  up.setPosition(Vector2f(position.x + size.x - 20, position.y));
  up.setFillColor(Color::Green);
  up.setOutlineColor(COLOR5);
  up.setOutlineThickness(2.0f);
  textUp.setFont(font);
  textUp.setCharacterSize(32);
  textUp.setFillColor(COLOR5);
  textUp.setString("+");
  textUp.setPosition(Vector2f(position.x + size.x - 16, position.y - 12));
  down.setSize(Vector2f(20, 20));
  down.setPosition(
      Vector2f(position.x + size.x - 20, position.y + size.y - 20));
  down.setFillColor(Color::Green);
  down.setOutlineColor(COLOR5);
  down.setOutlineThickness(2.0f);
  textDown.setFont(font);
  textDown.setCharacterSize(32);
  textDown.setFillColor(COLOR5);
  textDown.setString("-");
  textDown.setPosition(
      Vector2f(position.x + size.x - 16, position.y + size.y - 30));
}

void SelectedList::draw(RenderWindow &window) {
  window.draw(shape);
  for (int i = 0; i < showText.size(); i++) {
    window.draw(showShape[i]);
    window.draw(showText[i]);
  }
  window.draw(up);
  window.draw(down);
  window.draw(textUp);
  window.draw(textDown);
}

void SelectedList::dumbObject(GameObject *object) {
  allObject.push_back(object);
  showTextIndex = allObject.size() - 1;
  update();
}

RectangleShape SelectedList::getShape() { return shape; }

void SelectedList::shiftShowTextUp() {
  if (showTextIndex > 0) {
    showTextIndex--;
    update();
  }
}

void SelectedList::shiftShowTextDown() {
  if (showTextIndex < allObject.size() - showTextCapacity) {
    showTextIndex++;
    update();
  }
}

GameObject *SelectedList::click(Vector2f position) {
  for (int i = 0; i < showTextCapacity; i++) {
    if (showShape[i].getGlobalBounds().contains(position) &&
        showTextIndex + i < allObject.size()) {
      return allObject[showTextIndex + i];
    }
  }
  return nullptr;
}

void SelectedList::clear() {
  allObject.clear();
  showTextIndex = 0;
  update();
}

//---------------------------------CommandList---------------------------------

CommandList::CommandList(Vector2f position, Vector2f size,
                         vector<string> allObject, int textCapacity,
                         int showTextCapacity) {
  font.loadFromFile(FONT_PATH);
  shape.setPosition(position);
  shape.setSize(size);
  shape.setFillColor(COLOR1);
  shape.setOutlineColor(COLOR5);
  shape.setOutlineThickness(2.0f);
  for (int i = 0; i < textCapacity; i++) {
    Text tempText;
    RectangleShape tempShape;
    tempText.setFont(font);
    /*คำนวนขนาด text ตาม size ของ textBox*/
    tempShape.setSize(Vector2f(size.x, size.y / showTextCapacity));
    tempShape.setPosition(position.x,
                          position.y + (size.y * i / showTextCapacity));
    tempShape.setFillColor(COLOR3);
    tempShape.setOutlineColor(COLOR5);
    tempShape.setOutlineThickness(3.0f);
    showShape.push_back(tempShape);
    tempText.setCharacterSize(24);
    tempText.setFillColor(Color::White);
    tempText.setPosition(position.x + 10,
                         position.y + (size.y * i / showTextCapacity));
    showText.push_back(tempText);
  }
  for (int i = 0; i < textCapacity && i < allObject.size(); i++) {
    if (i < allObject.size()) {
      showText[i].setString(allObject[i]);
    } else {
      showText[i].setString(" ");
    }
  }
  up.setSize(Vector2f(20, 20));
  up.setPosition(Vector2f(position.x + size.x - 20, position.y));
  up.setFillColor(Color::Green);
  up.setOutlineColor(COLOR5);
  up.setOutlineThickness(2.0f);
  textUp.setFont(font);
  textUp.setCharacterSize(32);
  textUp.setFillColor(COLOR5);
  textUp.setString("+");
  textUp.setPosition(Vector2f(position.x + size.x - 16, position.y - 12));
  down.setSize(Vector2f(20, 20));
  down.setPosition(
      Vector2f(position.x + size.x - 20, position.y + size.y - 20));
  down.setFillColor(Color::Green);
  down.setOutlineColor(COLOR5);
  down.setOutlineThickness(2.0f);
  textDown.setFont(font);
  textDown.setCharacterSize(32);
  textDown.setFillColor(COLOR5);
  textDown.setString("-");
  textDown.setPosition(
      Vector2f(position.x + size.x - 16, position.y + size.y - 30));
}

void CommandList::draw(RenderWindow &window) {
  window.draw(shape);
  for (int i = 0; i < showText.size(); i++) {
    window.draw(showShape[i]);
    window.draw(showText[i]);
  }
  window.draw(up);
  window.draw(down);
  window.draw(textUp);
  window.draw(textDown);
}

void CommandList::shiftShowTextUp() {
  if (showTextIndex > 0) {
    showTextIndex--;
    for (int i = 0; i < textCapacity; i++) {
      if (showTextIndex + i < allObject.size()) {
        showText[i].setString(allObject[showTextIndex + i]);
      } else {
        showText[i].setString(" ");
      }
    }
  }
}

void CommandList::shiftShowTextDown() {
  if (showTextIndex < allObject.size() - textCapacity) {
    showTextIndex++;
    for (int i = 0; i < textCapacity; i++) {
      if (showTextIndex + i < allObject.size()) {
        showText[i].setString(allObject[showTextIndex + i]);
      } else {
        showText[i].setString(" ");
      }
    }
  }
}

string CommandList::click(Vector2f position) {
  for (int i = 0; i < showShape.size(); i++) {
    if (showShape[i].getGlobalBounds().contains(position)) {
      return allObject[showTextIndex + i];
    }
  }
  return "";
}

//---------------------------------Gui---------------------------------

void Gui::renderAny() {
  window->draw(turn);
  window->draw(sprite);
}

void Gui::pollEvent() {
  while (window->pollEvent(event)) {
    if (event.type == Event::Closed) {
      window->close();
    }
  }
}

void Gui::updateMousePosition() {
  mousePosition = Mouse::getPosition(*window);
  mousePositionView = window->mapPixelToCoords(mousePosition);
}

bool Gui::clickAbleCheck() {
  if (Mouse::isButtonPressed(Mouse::Left) && clickAble) {
    clickAble = false;
    return true;
  }
  return false;
}

void Gui::updateClickAble() {
  if (!Mouse::isButtonPressed(Mouse::Left)) {
    clickAble = true;
  }
}

void Gui::updateKeyboard() {
  if (Keyboard::isKeyPressed(Keyboard::Up)) {
    key = Keyboard::Up;
  } else if (Keyboard::isKeyPressed(Keyboard::Down)) {
    key = Keyboard::Down;
  } else if (Keyboard::isKeyPressed(Keyboard::Left)) {
    key = Keyboard::Left;
  } else if (Keyboard::isKeyPressed(Keyboard::Right)) {
    key = Keyboard::Right;
  } else if (Keyboard::isKeyPressed(Keyboard::LShift)) {
    key = Keyboard::LShift;
  } else {
    key = Keyboard::Unknown;
  }
}

bool Gui::pressAbleCheck() {
  if (pressAble) {
    pressAble = false;
    return true;
  }
  return false;
}

void Gui::updatePressAble() {
  if (!(Keyboard::isKeyPressed(Keyboard::Up) ||
        Keyboard::isKeyPressed(Keyboard::Down) ||
        Keyboard::isKeyPressed(Keyboard::Left) ||
        Keyboard::isKeyPressed(Keyboard::Right) ||
        Keyboard::isKeyPressed(Keyboard::LShift))) {
    pressAble = true;
  }
}

void Gui::updateUpdateButton() {
  if (stopButton->getShape().getGlobalBounds().contains(mousePositionView)) {
    if (clickAbleCheck()) {
      run = play = false;
    }
  }
  if (playButton->getShape().getGlobalBounds().contains(mousePositionView)) {
    if (clickAbleCheck()) {
      play = true;
      run = false;
    }
  }
  if (runButton->getShape().getGlobalBounds().contains(mousePositionView)) {
    if (clickAbleCheck()) {
      run = true;
    }
  }
}

void Gui::updateLayerSystem() {
  if (play || run) {
    layerSheet->update();
    turnCount++;
    turn.setString("Turn: " + to_string(turnCount));
    log->dumbText(layerSheet->getData()->logActionData);
    play ? play = false : void();
  }
}

void Gui::renderUpdateButton() {
  stopButton->draw(*window);
  playButton->draw(*window);
  runButton->draw(*window);
}

void Gui::updateLayerSheet() {
  for (int i = 0; i < layerSheet->buttons.size(); i++) {
    if (layerSheet->buttons[i].getButton().getGlobalBounds().contains(
            mousePositionView)) {
      if (clickAbleCheck()) {
        layerSheet->setSelectedButton(i);
        break;
      }
    }
  }
  layerSheet->selectedbutton->pushToMap(*map);
}

void Gui::renderLayerSheet() { layerSheet->draw(*window); }

void Gui::updateMapCenter() {
  if (key == Keyboard::Up) {
    pressAbleCheck() ? map->changeCenter(map->getCenter() + Vector2(0, 1))
                     : void();
  } else if (key == Keyboard::Down) {
    pressAbleCheck() ? map->changeCenter(map->getCenter() + Vector2(0, -1))
                     : void();
  } else if (key == Keyboard::Left) {
    pressAbleCheck() ? map->changeCenter(map->getCenter() + Vector2(1, 0))
                     : void();
  } else if (key == Keyboard::Right) {
    pressAbleCheck() ? map->changeCenter(map->getCenter() + Vector2(-1, 0))
                     : void();
  }
}

void Gui::renderMap() {
  map->update();
  map->draw(*window);
}

void Gui::updateZoomButton() {
  if (zoomIn->getShape().getGlobalBounds().contains(mousePositionView)) {
    clickAbleCheck() ? zoomIn->zoomIn(*map) : void();
  } else if (zoomOut->getShape().getGlobalBounds().contains(
                 mousePositionView)) {
    clickAbleCheck() ? zoomOut->zoomOut(*map) : void();
  }
}

void Gui::renderZoomButton() {
  zoomIn->draw(*window);
  zoomOut->draw(*window);
}

void Gui::updateDetail() {
  if (map->getBorder().getGlobalBounds().contains(mousePositionView)) {
    if (clickAbleCheck()) {
      GameObject *object = map->getObject(mousePositionView);
      if (object) {
        detail->setObject(object);
        detailShow = true;
      }
    }
  } else if (selectedList->getShape().getGlobalBounds().contains(
                 mousePositionView)) {
    if (clickAbleCheck()) {
      GameObject *object = selectedList->click(mousePositionView);
      if (object) {
        detail->setObject(object);
        detailShow = true;
      }
    }

  } else if (detail->getDeleteButton().getGlobalBounds().contains(
                 mousePositionView) &&
             detailShow) {
    if (clickAbleCheck()) {
      detail->setObject(nullptr);
      detailShow = false;
    }
  } else if (detail->getUp().getGlobalBounds().contains(mousePositionView) &&
             detailShow) {
    clickAbleCheck() ? detail->shiftShowTextUp() : void();
  } else if (detail->getDown().getGlobalBounds().contains(mousePositionView) &&
             detailShow) {
    clickAbleCheck() ? detail->shiftShowTextDown() : void();
  }
}

void Gui::renderDetail() {
  if (detailShow) {
    detail->draw(*window);
  }
}

void Gui::updateLog() {
  if (log->getUp().getGlobalBounds().contains(mousePositionView)) {
    clickAbleCheck() ? log->shiftShowTextUp() : void();
  } else if (log->getDown().getGlobalBounds().contains(mousePositionView)) {
    clickAbleCheck() ? log->shiftShowTextDown() : void();
  }
}

void Gui::renderLog() { log->draw(*window); }

void Gui::updateSelectedList() {
  if (map->getBorder().getGlobalBounds().contains(mousePositionView) &&
      key == Keyboard::LControl) {
    if (clickAbleCheck()) {
      GameObject *object = map->getObject(mousePositionView);
      object ? selectedList->dumbObject(object) : void();
    }
  } else if (map->getBorder().getGlobalBounds().contains(mousePositionView)) {
    if (clickAbleCheck()) {
      GameObject *object = map->getObject(mousePositionView);
      selectedList->clear();
      object ? selectedList->dumbObject(object) : void();
    }
  } else if (selectedList->getUp().getGlobalBounds().contains(
                 mousePositionView)) {
    clickAbleCheck() ? selectedList->shiftShowTextUp() : void();
  } else if (selectedList->getDown().getGlobalBounds().contains(
                 mousePositionView)) {
    clickAbleCheck() ? selectedList->shiftShowTextDown() : void();
  }
}

void Gui::renderSelectedList() { selectedList->draw(*window); }

void Gui::updateCommandList() {
  if (commandList->getUp().getGlobalBounds().contains(mousePositionView)) {
    clickAbleCheck() ? commandList->shiftShowTextUp() : void();
  } else if (commandList->getDown().getGlobalBounds().contains(
                 mousePositionView)) {
    clickAbleCheck() ? commandList->shiftShowTextDown() : void();
  }
}

void Gui::renderCommandList() { commandList->draw(*window); }

Gui::Gui() {
  this->window =
      new RenderWindow(VideoMode(1920, 1080), "Ecosystem Simulation");
  font.loadFromFile(FONT_PATH);
  window->setFramerateLimit(120);
  cout << "start create layerSystem\n";
  LayerSystem *layerSystem = new LayerSystem(255, 153);
  cout << "start create layerSheet\n";
  layerSheet = new LayerSheet(layerSystem, Vector2f(0, 0), Vector2f(1920, 100));
  map = new Map(Vector2f(1291, 775), Vector2f(629, 150),
                layerSheet->getData()->getOverAllLayer());
  zoomIn = new ZoomIn(Vector2f(1840, 115), 10);
  zoomOut = new ZoomOut(Vector2f(1875, 115), 10);
  font.loadFromFile(FONT_PATH);
  turn.setFont(font);
  turn.setCharacterSize(24);
  turn.setFillColor(COLOR5);
  turn.setString("Turn: " + to_string(turnCount));
  turn.setPosition(Vector2f(629, 115));
  stopButton = new StopButton(Vector2f(100, 115), Vector2f(20, 20));
  playButton = new PlayButton(Vector2f(230, 115), Vector2f(20, 20));
  runButton = new RunButton(Vector2f(360, 115), Vector2f(20, 20));

  log = new Log(Vector2f(629, 925), Vector2f(1291, 155));
  detail = new Detail(Vector2f(0, 150), Vector2f(625, 555.6));

  selectedList = new SelectedList(Vector2f(0, 427.8), Vector2f(625, 277.8), {});
  commandList = new CommandList(Vector2f(0, 150), Vector2f(625, 277.8));
  spriteTexture.loadFromFile(
      "C:\\Users\\ADMIN\\Documents\\GitHub\\Cminusminus\\22676.jpg");
  sprite.setTexture(spriteTexture);
  sprite.setPosition(Vector2f(0, 705.6));
  sprite.setScale(Vector2f(0.617, 0.45));
}

bool Gui::isOpen() { return window->isOpen(); }

void Gui::update() {
  pollEvent();
  updateMousePosition();
  updateClickAble();
  updateKeyboard();
  updatePressAble();
  updateMapCenter();
  updateZoomButton();
  updateLayerSheet();
  updateUpdateButton();
  updateLayerSystem();
  updateSelectedList();
  updateDetail();
  updateCommandList();
  updateLog();
}

void Gui::render() {
  window->clear(COLOR2);

  renderMap();
  renderZoomButton();
  renderLayerSheet();
  renderSelectedList();
  renderCommandList();
  renderLog();
  renderUpdateButton();
  renderAny();
  renderDetail();

  window->display();
}

int main() {
  vector<Ability *> ability;
  cout << ability.size() << endl;
  srand(time(NULL));
  Gui game;
  while (game.isOpen()) {
    game.update();

    game.render();
  }
}