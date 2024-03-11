#include "FundamentalSystem.h"
#include "LivingThing.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// ------------------StatParam--------------------//

bool StatParam::isAction(int before, int after)
{
  if (before > targetValue && targetValue >= after)
  {
    action();
    return true;
  }
  return false;
}

void StatParam::updateStackInfo()
{
  int highest = 0;
  for (pair<string, int> pair : stackInfo)
  {
    int value = pair.second;
    if (value > highest)
    {
      highest = value;
    }
  }
  setDefaultTo(highest);
}

StatParam::StatParam(StatusBlock *parent, Ability *createBy, int defaultValue)
    : parent(parent)
{
  pair<string, int> pair = make_pair(createBy->getName(), defaultValue);
  stackInfo.push_back(pair);
  updateStackInfo();
  parent->addStat(this);
}

StatParam::StatParam(GameObject *target, Ability *createBy, int defaultValue)
    : StatParam(target->getStat(), createBy, defaultValue) {}

void StatParam::setDefaultTo(int i)
{
  if (i < 0)
  {
    defaultValue = 0;
  }
  else
  {
    defaultValue = i;
  }
  value = defaultValue;
}

bool StatParam::changeValueBy(int i)
{
  int before = value;
  value + i < 0 ? value = 0 : value += i;
  return isAction(before, value);
}

void StatParam::pushStackInfo(pair<string, int> *target)
{
  for (pair<string, int> pair : stackInfo)
  {
    if (pair.first == target->first)
    {
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

// ------------------Ability--------------------//

Ability::Ability(AbilitySystem *parent) : parent(parent)
{
  parent->addAbility(this);
  createStatParam();
}

Ability::Ability(GameObject *target) : Ability(target->getStat()) {}

int Ability::decision(int sur, int env, int repo)
{
  return data->interpolate(sur, env, repo);
}

string Ability::getName() { return name; }
AbilitySystem *Ability::getParent() { return parent; }

// ------------------Affliction--------------------//

bool Affliction::tick()
{
  if (duration <= passedTime)
  {
    if (!permanant)
    {
      return false;
    }
  }
  else
  {
    passedTime += 1;
  }
  value += valueIncrese;
  return true;
}

Affliction::Affliction(AfflictionSystem *parent) : parent(parent)
{
  parent->addAffliction(this);
}

Affliction::Affliction(GameObject *target) : Affliction(target->getStat()) {}

bool Affliction::update()
{
  if (tick())
  {
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

bool AbilitySystem::isInAbility(string name)
{
  for (Ability *ability : abilityGroup)
  {
    if (ability->getName() == name)
    {
      return true;
    }
  }
  return false;
}

AbilitySystem::AbilitySystem(GameObject *parent)
    : parent(parent), abilityGroup({}) {}

bool AbilitySystem::addAbility(Ability *target)
{
  if (!isInAbility(target->getName()))
  {
    abilityGroup.push_back(target);
    return true;
  }
  return false;
}

void AbilitySystem::decisionMaking(int sur, int env, int repo)
{
  int maxValue, indexMax = -1, index, value = 0;
  for (Ability *ability : abilityGroup)
  {
    index++;
    ability->passive(ability->findTargetForPassive());
    if (ability->canActive(ability->findTargetForActive()))
    {
      value = ability->decision(sur, env, repo);
      if (maxValue < value)
      {
        maxValue = value;
        indexMax = index - 1;
      }
    }
  }
  if (indexMax > -1)
  {
    Ability *choosenAction = abilityGroup[indexMax];
    choosenAction->active(choosenAction->findTargetForActive());
  }
}

GameObject *AbilitySystem::getParent() { return parent; }

// ------------------Affliction--------------------//

AfflictionSystem::AfflictionSystem(GameObject *parent)
    : parent(parent), afflictionGroup({}) {}

void AfflictionSystem::recalculateValue()
{
  StatusBlock *statBlock = getParent()->getStat();
  string affName;
  StatParam *paramTarget;
  statBlock->resetValue();
  for (Affliction *aff : afflictionGroup)
  {
    affName = aff->getTargetValue();
    paramTarget = statBlock->getParam(affName);
    if (paramTarget != nullptr)
    {
      paramTarget->changeValueBy(aff->getValue());
    }
  }
}

bool AfflictionSystem::addAffliction(Affliction *target)
{
  afflictionGroup.push_back(target);
  recalculateValue();
  return true;
}

bool AfflictionSystem::removeAffliction(string name)
{
  int index = 0;
  for (Affliction *aff : afflictionGroup)
  {
    index++;
    if (aff->getName() == name)
    {
      afflictionGroup.erase(afflictionGroup.begin() + index - 1);
      return true;
    }
  }
  return false;
}

bool AfflictionSystem::removeAllAffliction(string name)
{
  int index = 0;
  bool have = false;
  for (Affliction *aff : afflictionGroup)
  {
    index++;
    if (aff->getName() == name)
    {
      afflictionGroup.erase(afflictionGroup.begin() + index - 1);
      have = true;
    }
  }
  return have;
}

int AfflictionSystem::amountOfAffliction(string name)
{
  int amount = 0;
  for (Affliction *aff : afflictionGroup)
  {
    if (aff->getName() == name)
    {
      amount++;
    }
  }
  return amount;
}

void AfflictionSystem::updateAffliction()
{
  int index = 0;
  for (Affliction *aff : afflictionGroup)
  {
    if (!(aff->update()))
    { // เหตุผลที่ไม่ใช้ remove ของตัว class เองก็เพราะว่าการ
      // remove ของตัว class เองจะ remove ตัวแรกที่มันเจอ
      // ไม่ใช่ตัวที่เราต้องการ
      afflictionGroup.erase(afflictionGroup.begin() + index - 1);
      delete aff;
    }
  }
  recalculateValue();
}

GameObject *AfflictionSystem::getParent() { return parent; }

bool AfflictionSystem::isInAffliction(string name)
{
  for (Affliction *aff : afflictionGroup)
  {
    if (aff->getName() == name)
    {
      return true;
    }
  }
  return false;
}

// ------------------StatusBlock--------------------//

StatusBlock::StatusBlock(GameObject *parent)
    : parent(parent), AbilitySystem(parent), AfflictionSystem(parent) {}

bool StatusBlock::isInParam(string name)
{
  for (StatParam *param : statParamGroup)
  {
    if (param->getName() == name)
    {
      return true;
    }
  }
  return false;
}

void StatusBlock::resetValue()
{
  for (StatParam *stat : statParamGroup)
  {
    stat->resetValue();
  }
}

bool StatusBlock::addStat(StatParam *target)
{
  string targetName = target->getName();
  StatParam *oldStat = getParam(targetName);
  if (oldStat != nullptr)
  {
    oldStat->pushStackInfo(&(target->getStackInfo()[0]));
    recalculateValue();
    return false;
  }
  else
  {
    statParamGroup.push_back(target);
    recalculateValue();
    return true;
  }
}

int StatusBlock::getParamValueDefault(string name)
{
  for (StatParam *stat : statParamGroup)
  {
    if (name == stat->getName())
    {
      return stat->getDefaultValue();
    }
  }
  return 0;
}

int StatusBlock::getParamValue(string name)
{
  for (StatParam *stat : statParamGroup)
  {
    if (name == stat->getName())
    {
      return stat->getValue();
    }
  }
  return 0;
}

StatParam *StatusBlock::getParam(string name)
{
  for (StatParam *param : statParamGroup)
  {
    if (param->getName() == name)
    {
      return param;
    }
  }
  return nullptr;
}

GameObject *StatusBlock::getParent() { return parent; }

// ------------------GameObject--------------------//

GameObject::GameObject(Layer *parent, int x, int y) : parent(parent)
{
  parent->setLayer(this, x, y);
}

pair<int, int> GameObject::getVectorIndex()
{
  if (parent != nullptr)
  {
    const vector<vector<GameObject *>> layerVectors = parent->insideLayer;
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
    LayerSystem *ls = parent->getParent();
    Ground ground = ls->getGround();
    Land floor = ground.insideLayer[vector_index.second][vector_index.first];
    return {floor.getX(), floor.getY()};
  }
  // not found
  return {-2147483648, -2147483648};
}

void GameObject::setName(string stringInput) { name = stringInput; }
void GameObject::setRepresent(string stringInput) { represent = stringInput; }
string GameObject::getName() { return name; }
string GameObject::getRepresent() { return represent; }
StatusBlock *GameObject::getStat() { return stat; }
Layer *GameObject::getParent() { return parent; }

// returns only GameObjects
vector<GameObject *> GameObject::findTargetInRange(int range, bool allLayers)
{
  vector<GameObject *> target;
  pair<int, int> selfIndex = getVectorIndex();
  for (double i{0}; i <= range; i++)
  {
    for (double j{0}; j <= range; j++)
    {
      // if the selected tile is within range and isn't the same tile as the GameObject:
      if (ceil(sqrt(pow(i, 2.) + pow(j, 2.))) <= range && (i != 0 || j != 0))
      {
        // if the selected tile is more than or equal to 0 (not out of range)
        if (selfIndex.first - i >= 0 && selfIndex.second - j >= 0)
        {
          if (allLayers) // all layers in the same layer system
          {
            LayerSystem *layersystem = parent->getParent();
            for (size_t k = 0; k < layersystem->getLayersAmount(); k++)
            {
              Layer *layer = layersystem->getLayer(0);
              GameObject *targetObject = layer->getFromLayerIndex(selfIndex.first - i, selfIndex.second - j);
              if (targetObject != nullptr)
              {
                target.push_back(targetObject);
              }
            }
          }
          else // GameObject's own layer only
          {
            GameObject *targetObject = parent->getFromLayerIndex(selfIndex.first - i, selfIndex.second - j);
            if (targetObject != nullptr)
            {
              target.push_back(targetObject);
            }
          }
        }
        // if the selected tile is less than the size (not out of range)
        if (selfIndex.first + i < parent->insideLayer.size() && selfIndex.second + j < parent->insideLayer.size())
        {
          if (allLayers) // all layers in the same layer system
          {
            LayerSystem *layersystem = parent->getParent();
            for (size_t k = 0; k < layersystem->getLayersAmount(); k++)
            {
              Layer *layer = layersystem->getLayer(0);
              GameObject *targetObject = layer->getFromLayerIndex(selfIndex.first + i, selfIndex.second + j);
              if (targetObject != nullptr)
              {
                target.push_back(targetObject);
              }
            }
          }
          else // GameObject's own layer only
          {
            GameObject *targetObject = parent->getFromLayerIndex(selfIndex.first + i, selfIndex.second + j);
            if (targetObject != nullptr)
            {
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
    : parent(parent), name(name)
{
  for (int y = 0; y < height; y++)
  {
    vector<GameObject *> row;
    for (int x = 0; x < width; x++)
    {
      row.push_back(nullptr);
    }
    insideLayer.push_back(row);
  }
}

// update everything in the layer
void Layer::action()
{
  for (int i = 0; i < insideLayer.size(); i++)
  {
    for (int j = 0; j < insideLayer.at(i).size(); j++)
    {
      if (insideLayer.at(i).at(j) != nullptr)
      {
        insideLayer.at(i).at(j)->update();
      }
    }
  }
}

void Layer::setLayer(GameObject *target, int x, int y)
{
  insideLayer.at(y).at(x) = target;
}

void Layer::removeFromLayer(int x, int y) { insideLayer.at(y).at(x) = nullptr; }

inline string Layer::getName() { return name; }
void Layer::setName(string nameInput) { name = nameInput; }
GameObject *Layer::getFromLayerIndex(int x, int y)
{
  return insideLayer.at(y).at(x);
}

GameObject *Layer::getFromLayerCoord(int x, int y)
{
  pair<int, int> targetIndex = parent->getGround().getVectorIndex(x, y);
  return getFromLayerIndex(targetIndex.first, targetIndex.second);
}

LayerSystem *Layer::getParent() { return parent; }

// ------------------Land--------------------//

Land::Land(Ground *parent, string represent, int x, int y) : parent(parent), represent(represent), x(x), y(y) {}

inline int Land::getX() { return x; }
inline int Land::getY() { return y; }

// ------------------Ground--------------------//

Ground::Ground(LayerSystem *p, int width, int height) : parent(p)
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
    Ground::insideLayer.push_back(row);
  }
}

pair<int, int> Ground::getVectorIndex(int x, int y)
{
  for (int i = 0; i < insideLayer.size(); ++i)
  {
    for (int j = 0; j < insideLayer[i].size(); ++j)
    {
      if (insideLayer[i][j].getX() == x && insideLayer[i][j].getY() == y)
      {
        return make_pair(j, i);
      }
    }
  }

  // -1 = not found
  return make_pair(-1, -1);
}

//-------------------LayerSystem-------------------//

bool LayerSystem::isInLayer(string name)
{
  for (Layer *l : layers)
  {
    if (l->getName() == name)
    {
      return true;
    }
  }
  return false;
}

LayerSystem::LayerSystem(int width, int height)
    : width(width), height(height), ground(Ground(this, width, height))
{
  createNewLayer("Food");
  createNewLayer("Animal");
  createNewLayer("Environment");
}

bool LayerSystem::createNewLayer(string name)
{
  if (isInLayer(name))
  {
    return false;
  }
  Layer *layer = new Layer(name, this, width, height);
  layers.push_back(layer);
  return true;
}

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

int LayerSystem::getDistance(GameObject *a, GameObject *b)
{
  pair<int, int> aCoord = a->getCoord();
  pair<int, int> bCoord = b->getCoord();
  return getDistance(aCoord.first, aCoord.second, bCoord.first, bCoord.second);
}

int LayerSystem::getDistance(int x1, int y1, int x2, int y2)
{
  return floor(abs(x1 - x2) + abs(y1 - y2));
}

inline Layer *LayerSystem::getLayer(string name)
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

inline Layer *LayerSystem::getLayer(int i) { return layers.at(i); }

inline Layer *LayerSystem::getRandomLayer() { return layers.at(rand() % layers.size()); }

Ground LayerSystem::getGround() { return ground; }

string inline LayerSystem::getLayerName(int i) { return layers.at(i)->getName(); }

int inline LayerSystem::getLayersAmount() { return layers.size(); }

int inline LayerSystem::getLayersWidth() { return width; }

int inline LayerSystem::getLayersHeight() { return height; }

void LayerSystem::printLayer()
{
  Layer *env_layer = getLayer("Environment");
  Layer *animal_layer = getLayer("Animal");
  Layer *food_layer = getLayer("Food");

  for (int column = 0; column < height; column++)
  {
    for (int row = 0; row < width; row++)
    {
      if (env_layer->getFromLayerIndex(column, row) != nullptr)
      {
        cout << env_layer->getFromLayerIndex(column, row) << ' ';
      }
      else if (animal_layer->getFromLayerIndex(column, row) != nullptr)
      {
        cout << animal_layer->getFromLayerIndex(column, row) << ' ';
      }
      else if (food_layer->getFromLayerIndex(column, row) != nullptr)
      {
        cout << food_layer->getFromLayerIndex(column, row) << ' ';
      }
      else
      {
        cout << "x ";
      }
    }
    cout << endl;
  }
}

void LayerSystem::randomGenerateLayers()
{
  Layer *env_layer = getLayer("Environment");
  Layer *animal_layer = getLayer("Animal");
  Layer *food_layer = getLayer("Food");

  for (int column = 0; column < height; column++)
  {
    for (int row = 0; row < width; row++)
    {
      int rand_animal = rand() % 100;
      int rand_food = rand() % 100;
      int rand_env = rand() % 100;
      if (rand_animal < 10){
        new Deer(animal_layer, row, column);
      }
      if (rand_food < 1){
        new Corpse(food_layer, row, column);
      }
      if (rand_env < 10){
        int rand_plant = rand() % 2;
        if (rand_plant == 0){
          new Bush(env_layer, row, column);
        } else {
          new Apple_tree(env_layer, row, column);
        }
      }
    }
  }
}