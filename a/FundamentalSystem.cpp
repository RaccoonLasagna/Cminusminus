#include "FundamentalSystem.h"
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

//-------------------StatParameter-------------------// Clean!
StatParameter::StatParameter(StatusSystem *parentInput, string nameInput,
                             int value = 0)
    : parent(*parentInput), name(nameInput), value(value) {}

void inline StatParameter::change(const int i) { value += i; }
void inline StatParameter::setValue(const int i) { value = i; }
void inline StatParameter::setName(const string nameInput) { name = nameInput; }
int inline StatParameter::getValue() { return value; }
string inline StatParameter::getName() { return name; }
StatusSystem inline &StatParameter::getParent() { return parent; }

// This shit please

//-------------------StatParameterMax-------------------//
StatParameterMax::StatParameterMax(StatusSystem *parentInput, string nameInput,
                                   int value = 0, int maxValueInput = 0)
    : StatParameter(parentInput, nameInput, value), maxValue(maxValueInput) {}

void StatParameterMax::change(const int i)
{
  int value = getValue();
  if (value + i <= maxValue)
  {
    value += i;
  }
  else
  {
    value = maxValue;
  }
}

int inline StatParameterMax::getMax() { return maxValue; }
void inline StatParameterMax::setMax(const int i) { maxValue = i; }

//-------------------StatParameterCoord-------------------// Clean!
StatParameterCoord::StatParameterCoord(StatusSystem *parentInput,
                                       string nameInput = "position",
                                       int xValueInput = 0, int yValueInput = 0,
                                       int xMAX = 0, int yMAX = 0, int xMIN = 0,
                                       int yMIN = 0)
    : StatParameter(parentInput, nameInput), xValue(xValueInput),
      yValue(yValueInput), xMAX(xMAX), yMAX(yMAX), xMIN(xMIN), yMIN(yMIN) {}

bool StatParameterCoord::changeCoord(const int x, const int y)
{
  int newX = xValue + x;
  int newY = yValue + y;
  if (newX >= xMIN && xMAX >= newX && newY >= yMIN && yMAX >= newY)
  {
    xValue = newX;
    yValue = newY;
    return true;
  }
  else
  {
    return false;
  }
}

bool StatParameterCoord::setCoord(const int x, const int y)
{
  if (x >= xMIN && xMAX >= x && y >= yMIN && yMAX >= y)
  {
    xValue = x;
    yValue = y;
    return true;
  }
  else
  {
    return false;
  }
}

void StatParameterCoord::setMax(const int x, const int y)
{
  xMAX = x;
  yMAX = y;
}
void StatParameterCoord::setMin(const int x, const int y)
{
  xMIN = x;
  yMIN = y;
}
int inline StatParameterCoord::getX() { return xValue; }
int inline StatParameterCoord::getY() { return yValue; }
int inline StatParameterCoord::getXMax() { return xMAX; }
int inline StatParameterCoord::getYMax() { return yMAX; }
int inline StatParameterCoord::getXMin() { return xMIN; }
int inline StatParameterCoord::getYMin() { return yMIN; }

//-------------------Ability-------------------// Clean!
Ability::Ability(AbilitySystem *parentInput, string nameInput)
    : parent(*parentInput), name(nameInput) {}

void inline Ability::setName(const string nameInput) { name = nameInput; }
string inline Ability::getName() { return name; }
AbilitySystem inline &Ability::getParent() { return parent; }
// ฟังก์ชั่น desition class ลูกคลาสจะต้องเขียนเอง

//-------------------Affliction-------------------// Clean!
void Affliction::update()
{
  tick();
  action();
}

bool Affliction::tick()
{
  if (duration - passedTime > 0)
  {
    passedTime++;
    return true;
  }
  return false;
}

bool Affliction::refresh()
{
  if (passedTime == 0)
  {
    return false;
  }
  passedTime = 0;
  return true;
}

AfflictionSystem &Affliction::getParent() { return parent; }
void inline Affliction::setName(const string nameInput) { name = nameInput; }
void inline Affliction::setDuration(const int i) { duration = i; }
string inline Affliction::getName() { return name; }
int inline Affliction::getDuration() { return duration; }

//-------------------AbilitySystem-------------------// Clean!
AbilitySystem::AbilitySystem(GameObject *parentInput,
                             vector<Ability *> abilitiesInput = {})
    : parent(parentInput), abilities(abilitiesInput) {}

bool AbilitySystem::addAbility(Ability *ability)
{
  for (Ability *a : abilities)
  {
    if (a->getName() == ability->getName())
    {
      return false;
    }
  }
  abilities.push_back(ability);
  return true;
}

bool AbilitySystem::removeAbility(const string name)
{
  for (int i = 0; i < abilities.size(); i++)
  {
    if (abilities.at(i)->getName() == name)
    {
      abilities.erase(abilities.begin() + i);
      return true;
    }
  }
  return false;
}

bool AbilitySystem::isInAbility(const string name)
{
  for (Ability *a : abilities)
  {
    if (a->getName() == name)
    {
      return true;
    }
  }
  return false;
}

void AbilitySystem::decisionMakeing(const int needSur, const int needEnv, const int needRepr) const
{
  int maxValue = 0;
  int maxIndex = 0;
  for (int i = 0; i < abilities.size(); i++)
  {
    int value = abilities.at(i)->decision(needSur, needEnv, needRepr);
    if (value > maxValue)
    {
      maxValue = value;
      maxIndex = i;
    }
  }
  abilities.at(maxIndex)->action();
}

GameObject inline *AbilitySystem::getParent() { return parent; }
int inline AbilitySystem::getAbilitySize() { return abilities.size(); }

//-------------------AfflictionSystem-------------------// clean!
bool AfflictionSystem::checkAlive()
{
  if (parent->getStat().isInStat("health"))
  {
    if (parent->getStat().getValue("health")->getValue() <= 0)
    {
      return false;
    }
  }
  return true;
}

AfflictionSystem::AfflictionSystem(GameObject *parentInput,
                                   vector<Affliction *> afflictionsInput = {})
    : parent(parentInput), afflictions(afflictionsInput) {}

bool AfflictionSystem::addAffliction(Affliction *affliction)
{
  string name = affliction->getName();
  for (Affliction *a : afflictions)
  {
    string tempName = a->getName();
    if (tempName == name)
    {
      afflictions.push_back(affliction);
      return false;
    }
  }
  afflictions.push_back(affliction);
  return true;
}

bool AfflictionSystem::removeAffliction(const string name)
{
  for (int i = 0; i < afflictions.size(); i++)
  {
    if (afflictions.at(i)->getName() == name)
    {
      afflictions.erase(afflictions.begin() + i);
      return true;
    }
  }
  return false;
}

bool AfflictionSystem::isInAffliction(const string name)
{
  for (Affliction *a : afflictions)
  {
    if (a->getName() == name)
    {
      return true;
    }
  }
  return false;
}

bool AfflictionSystem::updateAffliction()
{
  for (Affliction *a : afflictions)
  {
    a->update();
  }
  return checkAlive();
}

vector<Affliction *> inline &AfflictionSystem::getAfflictions()
{
  return afflictions;
}
GameObject inline *AfflictionSystem::getParent() { return parent; }
int inline AfflictionSystem::getAfflictionSize() { return afflictions.size(); }

//-------------------StatusSystem-------------------//
StatusSystem::StatusSystem(GameObject *parentInput, int xValueInput = 0,
                           int yValueInput = 0, int xMAX = 0, int yMAX = 0,
                           int xMIN = 0, int yMIN = 0,
                           vector<StatParameterMax *> *value = {},
                           vector<Ability *> *abilities = {},
                           vector<Affliction *> *afflictions = {})
    : parentParamiter(parentInput), value(*value),
      AfflictionSystem(parentInput, *afflictions),
      AbilitySystem(parentInput, *abilities),
      positionParamiter(this, "position", xValueInput, yValueInput, xMAX, yMAX,
                        xMIN, yMIN) {}

bool StatusSystem::isInStat(string name)
{
  for (StatParameterMax *s : value)
  {
    if (s->getName() == name)
    {
      return true;
    }
  }
  return false;
}

bool StatusSystem::addStat(string name)
{
  if (isInStat(name))
  {
    return false;
  }
  StatParameterMax *stat = new StatParameterMax(this, name);
  value.push_back(stat);
  return true;
}

bool StatusSystem::removeStat(string name)
{
  for (int i = 0; i < value.size(); i++)
  {
    if (value.at(i)->getName() == name)
    {
      value.erase(value.begin() + i);
      return true;
    }
  }
  return false;
}

void StatusSystem::setParent(GameObject *parentInput)
{
  parentParamiter = parentInput;
}

StatParameterMax *StatusSystem::getValue(string name)
{
  for (StatParameterMax *s : value)
  {
    if (s->getName() == name)
    {
      return s;
    }
  }
  return nullptr;
}
StatParameterCoord inline &StatusSystem::getPosition()
{
  return positionParamiter;
}
int StatusSystem::getPositonX() { return positionParamiter.getX(); }
int StatusSystem::getPositonY() { return positionParamiter.getY(); }
GameObject *StatusSystem::getParent() { return parentParamiter; }

//-------------------GameObject-------------------// clean!
GameObject::GameObject(Layer *parentInput, string representInput,
                       string nameInput = "", int xValueInput = 0,
                       int yValueInput = 0, int xMaxInput = 0,
                       int yMaxInput = 0, int xMinInput = 0, int yMinInput = 0,
                       vector<StatParameterMax *> *value = {},
                       vector<Ability *> *abilities = {},
                       vector<Affliction *> *afflictions = {})
    : parent(parentInput), represent(representInput), name(nameInput)
{

  StatusSystem *statCreate =
      new StatusSystem(this, xValueInput, yValueInput, xMaxInput, yMaxInput,
                       xMinInput, yMinInput, value, abilities, afflictions);
  statCreate->setParent(this);
}

void inline GameObject::setName(const string nameInput) { name = nameInput; }
void inline GameObject::setRepresent(const string representInput)
{
  represent = representInput;
}

void GameObject::setStat(StatusSystem *statInput)
{
  if (statInput->getParent() != nullptr)
  {
    statInput->getParent()->stat = nullptr;
  }
  statInput->setParent(this);
  stat = statInput;
}
void inline GameObject::setParent(Layer *parentInput) { parent = parentInput; }
string inline GameObject::getRepresent() { return represent; }
string inline GameObject::getName() { return name; }
StatusSystem inline &GameObject::getStat() { return *stat; }
Layer inline *GameObject::getParent() { return parent; }
pair<int, int> GameObject::getVectorIndex()
{
  if (parent)
  {
    const vector<vector<GameObject *>> &layerVectors = parent->getLayer();
    for (size_t i = 0; i < layerVectors.size(); ++i)
    {
      const vector<GameObject *> &currentVector = layerVectors[i];
      auto it = find(currentVector.begin(), currentVector.end(), this);
      if (it != currentVector.end())
      {
        size_t j = distance(currentVector.begin(), it);
        return {static_cast<int>(j), static_cast<int>(i)};
      }
    }
  }

  // not found
  return {-2147483648, -2147483648};
}
pair<int, int> GameObject::getCoord()
{
  if (parent)
  {
    pair<int, int> vector_index = getVectorIndex();
    LayerSystem &ls = parent->getParent();
    Ground ground = ls.getGround();
    Land floor = ground.getLayer()[vector_index.second][vector_index.first];
    return {floor.getX(), floor.getY()};
  }

  return {-2147483648, -2147483648};
}

// -------------------Land--------------------//
Land::Land(Ground *parentInput, string representInput, int x, int y) : parent(parentInput), represent(representInput), x(x), y(y) {}

// placeholder
void Land::update() { return; }
int Land::getX() { return x; }
int Land::getY() { return y; }

//-------------------Layer-------------------// clean!
Layer::Layer(LayerSystem *parent, int width, int height, string name) : parent(*parent)
{
  for (int y = 0; y < height; y++)
  {
    vector<GameObject *> row;

    for (int x = 0; x < width; x++)
    {
      row.push_back(nullptr);
    }
    Layer::layer.push_back(row);
  }
}

// update everything in the layer
void Layer::action()
{
  for (int i = 0; i < layer.size(); i++)
  {
    for (int j = 0; j < layer.at(i).size(); j++)
    {
      if (layer.at(i).at(j) != nullptr)
      {
        layer.at(i).at(j)->update();
      }
    }
  }
}

// void inline Layer::setName(const string nameInput) { name = nameInput; }

string inline Layer::getName() { return name; }

LayerSystem inline &Layer::getParent() { return parent; }

vector<vector<GameObject *>> Layer::getLayer() { return layer; }

// ------------------Ground--------------------//

Ground::Ground(LayerSystem *p = nullptr, int width = 0, int height = 0) : parent(*p)
{
  int width_min = static_cast<int>(-floor(width / 2));
  int height_min = static_cast<int>(-floor(height / 2));

  for (int y = height_min; y < height_min + height; y++)
  {
    vector<Land> row;
    for (int x = width_min; x < width_min + width; x++)
    {
      // casts ground pointer to layer pointer so that Land can be constructed
      row.push_back(Land(this, "#", x, y));
    }
    Ground::layer.push_back(row);
  }
}

vector<vector<Land>> Ground::getLayer() { return layer; }

//-------------------LayerSystem-------------------//
LayerSystem::LayerSystem(int width, int height, int amount) : width(width), height(height), ground(Ground(this, width, height))
{
  for (size_t lCount = 0; lCount < amount; lCount++)
  {
    createNewLayer("layer" + to_string(lCount));
  }
}

bool LayerSystem::createNewLayer(string name)
{ // ต้องสร้าง layer ที่หนาดเท่ากับ layer ที่มีอยูแล้วด้วย
  for (Layer *l : layers)
  {
    if (l->getName() == name)
    {
      return false;
    }
  }
  Layer *layer = new Layer(this, width, height, name);
  layers.push_back(layer);
  return true;
}

Ground LayerSystem::getGround() { return ground; }

bool LayerSystem::removeLayer(string name)
{
  for (int i = 0; i < layers.size(); i++)
  {
    if (layers.at(i)->getName() == name)
    {
      layers.erase(layers.begin() + i);
      return true;
    }
  }
  return false;
}

Layer inline *LayerSystem::getLayer(string name)
{
  for (Layer *l : layers)
  {
    if (l->getName() == name)
    {
      return l;
    }
  }
  return nullptr;
}

Layer inline *LayerSystem::getLayer(int i) { return layers.at(i); }

string inline LayerSystem::getLayerName(int i)
{
  return layers.at(i)->getName();
}

vector<Layer *> *LayerSystem::getLayers() { return &layers; }
int inline LayerSystem::getLayerSize() { return layers.size(); }

// //-------------------Livingthing-------------------//
// LivingThing::LivingThing(
//     Layer *parentInput, string representInput, string nameInput = "",
//     int healthInput = 10, int maxHealthInput = 10, int staminaInput = 10,
//     int maxStaminaInput = 10, int hungerInput = 10, int maxHungerInput = 10,
//     int visionInput = 1, int maxVisionInput = 1, int lifetimeInput = 10,
//     int maxLifetimeInput = 10, int xValueInput = 0, int yValueInput = 0,
//     int xMaxInput = 0, int yMaxInput = 0, int xMinInput = 0, int yMinInput = 0)
//     : GameObject(parentInput, representInput, nameInput, healthInput,
//                  maxHealthInput, staminaInput, maxStaminaInput, hungerInput,
//                  maxHungerInput, visionInput, maxVisionInput, lifetimeInput,
//                  maxLifetimeInput, xValueInput, yValueInput, xMaxInput,
//                  yMaxInput, xMinInput, yMinInput) {
//   needToEnvironment = needToReproduce = needToSurvive = 0;
// }

// //-------------------Animal-------------------//
// Animal::Animal(Layer *parentInput, string representInput, string nameInput = "",
//                int healthInput = 10, int maxHealthInput = 10,
//                int staminaInput = 10, int maxStaminaInput = 10,
//                int hungerInput = 10, int maxHungerInput = 10,
//                int visionInput = 1, int maxVisionInput = 1,
//                int lifetimeInput = 10, int maxLifetimeInput = 10,
//                int xValueInput = 0, int yValueInput = 0, int xMaxInput = 0,
//                int yMaxInput = 0, int xMinInput = 0, int yMinInput = 0)
//     : LivingThing(parentInput, representInput, nameInput, healthInput,
//                   maxHealthInput, staminaInput, maxStaminaInput, hungerInput,
//                   maxHungerInput, visionInput, maxVisionInput, lifetimeInput,
//                   maxLifetimeInput, xValueInput, yValueInput, xMaxInput,
//                   yMaxInput, xMinInput, yMinInput) {}

// //-------------------Plant-------------------//
// Plant::Plant(Layer *parentInput, string representInput, string nameInput = "",
//              int healthInput = 10, int maxHealthInput = 10,
//              int staminaInput = 10, int maxStaminaInput = 10,
//              int hungerInput = 10, int maxHungerInput = 10, int visionInput = 1,
//              int maxVisionInput = 1, int lifetimeInput = 10,
//              int maxLifetimeInput = 10, int xValueInput = 0,
//              int yValueInput = 0, int xMaxInput = 0, int yMaxInput = 0,
//              int xMinInput = 0, int yMinInput = 0)
//     : LivingThing(parentInput, representInput, nameInput, healthInput,
//                   maxHealthInput, staminaInput, maxStaminaInput, hungerInput,
//                   maxHungerInput, visionInput, maxVisionInput, lifetimeInput,
//                   maxLifetimeInput, xValueInput, yValueInput, xMaxInput,
//                   yMaxInput, xMinInput, yMinInput) {}

// //-------------------NonLivingThing-------------------//
// NonLivingThing::NonLivingThing(
//     Layer *parentInput, string representInput, string nameInput = "",
//     int healthInput = 10, int maxHealthInput = 10, int staminaInput = 10,
//     int maxStaminaInput = 10, int hungerInput = 10, int maxHungerInput = 10,
//     int visionInput = 1, int maxVisionInput = 1, int lifetimeInput = 10,
//     int maxLifetimeInput = 10, int xValueInput = 0, int yValueInput = 0,
//     int xMaxInput = 0, int yMaxInput = 0, int xMinInput = 0, int yMinInput = 0)
//     : GameObject(parentInput, representInput, nameInput, healthInput,
//                  maxHealthInput, staminaInput, maxStaminaInput, hungerInput,
//                  maxHungerInput, visionInput, maxVisionInput, lifetimeInput,
//                  maxLifetimeInput, xValueInput, yValueInput, xMaxInput,
//                  yMaxInput, xMinInput, yMinInput) {}

// //-------------------Field-------------------//
// Field::Field(Layer *parentInput, string representInput, string nameInput = "",
//              int healthInput = 10, int maxHealthInput = 10,
//              int staminaInput = 10, int maxStaminaInput = 10,
//              int hungerInput = 10, int maxHungerInput = 10, int visionInput = 1,
//              int maxVisionInput = 1, int lifetimeInput = 10,
//              int maxLifetimeInput = 10, int xValueInput = 0,
//              int yValueInput = 0, int xMaxInput = 0, int yMaxInput = 0,
//              int xMinInput = 0, int yMinInput = 0)
//     : NonLivingThing(parentInput, representInput, nameInput, healthInput,
//                      maxHealthInput, staminaInput, maxStaminaInput, hungerInput,
//                      maxHungerInput, visionInput, maxVisionInput, lifetimeInput,
//                      maxLifetimeInput, xValueInput, yValueInput, xMaxInput,
//                      yMaxInput, xMinInput, yMinInput) {}