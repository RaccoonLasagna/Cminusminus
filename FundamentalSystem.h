#include <string>
#include <vector>
using namespace std;

class StatParam;
class StatParamMax;
class StatParamCoord;
class Ability;
class AbilitySlot;
class Afflication;
class AfflicationSlot;
class Stat;
class GameObject;
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
public:
  inline int getValue() { return value; }

private:
  int value;
};

class StatParamMax : public StatParam {
public:
  void increse(int);
  void decrese(int);
  void setValue(int);
  void setMax(int);
  inline int getMax() { return maxValue; }

private:
  int maxValue;
};

class StatParamCoord : public StatParam {
public:
  void increse(int, int);
  void decrese(int, int);
  void setCoord(int, int);
  void setMax(int, int);
  void setMin(int, int);
  inline int getX() { return xValue; }
  inline int getY() { return yValue; }

private:
  int xValue, yValue, xMAX, yMAX, xMIN, yMIN;
};

class Ability {
public:
  int virtual desition(Environment *) = 0;
  void virtual action(Environment *) = 0;
  inline string getName() { return name; }

private:
  string name;
};

class AbilitySlot {
public:
  void addAbility(Ability *);
  void removeAbility(Ability *);
  void desition(int, int, int);

private:
  vector<Ability *> abilities;
};

class Afflication {
public:
  void virtual action(Environment *) = 0;
  inline string getName() { return name; }

private:
  string name;
  int duration;
  void update();
};

class AfflicationSlot {
public:
  void addAfflication(Afflication *);
  void removeAfflication(Afflication *);
  void update();

private:
  vector<Afflication *> afflications;
};

class Stat {
public:
  inline int getHealth() { return health.getValue(); }
  inline int getStamina() { return stamina.getValue(); }
  inline int getHunger() { return hunger.getValue(); }
  inline int getVision() { return vision.getValue(); }
  inline int getLifetime() { return lifetime.getValue(); }
  inline int getX() { return position.getX(); }
  inline int getY() { return position.getY(); }
  inline AbilitySlot *getAbilities() { return &abilities; }
  inline AfflicationSlot *getAfflications() { return &afflications; }

private:
  StatParamMax health, stamina, hunger, vision, lifetime;
  StatParamCoord position;
  AbilitySlot abilities;
  AfflicationSlot afflications;
};

class GameObject {
public:
  Stat *getStat() { return &stat; }
  string getRepresent() { return represent; }

private:
  Stat stat;
  string represent;
};

class Layers {
public:
  vector<vector<GameObject *>> getLayer(int i) { return layers.at(i); };
  string getLayerName(int i) { return layersName.at(i); };
  void addLayer(string name, vector<vector<GameObject *>> layer);
  void removeLayer(int i) {
    layers.erase(layers.begin() + i);
    layersName.erase(layersName.begin() + i);
  };

private:
  vector<string> layersName;
  vector<vector<vector<GameObject *>>> layers;
};

class Environment {
public:
  void update();
  Layers *getLayers() { return &layers; }

private:
  Layers layers;
};

class LivingThing : public GameObject {
public:
  void virtual update();

private:
  int needToEnvironment, needToSurvive, needToReproduce;
  void virtual calculateNeeds() = 0;
  void virtual evolve() = 0;
  void desition();
};

class Animal : public LivingThing {
private:
  void virtual calculateNeeds() = 0;
};

class Plant : public LivingThing {
private:
  void virtual calculateNeeds() = 0;
};

class NonLivingThing : public GameObject {
public:
  void virtual update();

private:
  void virtual evolve() = 0;
};

class Field {
public:
  bool stayAble(GameObject *);
  string getname() { return name; }

private:
  string name;
};

#endif