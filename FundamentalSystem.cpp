#include "FundamentalSystem.h"
#include <_mingw_stat64.h>
#include <cmath>
#include <string>
#include <utility>
#include <vector>

using namespace std;

//----------------------------//

bool StatParam::isAction(int fristScope, int secondScope)
{
  if (fristScope > targetValue && targetValue >= secondScope)
  {
    action();
    return true;
  }
  return false;
}

void StatParam::updateAddBy()
{
  int highest = 0;
  for (pair<Ability *, int> *pair : stackInfo)
  {
    int value = pair->second;
    if (value > highest)
    {
      highest = value;
    }
  }
  setRawTo(highest);
}

StatParam::StatParam(StatusBlock *parent, Ability *createBy, int rawValue)
    : parent(parent), createBy(createBy), rawValue(rawValue), value(rawValue)
{
  pair<Ability *, int> pair = make_pair(createBy, rawValue);
  stackInfo.push_back(&pair);
  parent->addStat(this);
}

StatParam::StatParam(GameObject *target, Ability *createBy, int rawValue)
    : StatParam(target->getStat(), createBy, rawValue) {}

void StatParam::setRawTo(int i)
{
  if (i < 0)
  {
    rawValue = 0;
  }
  else
  {
    rawValue = i;
  }
  value = rawValue;
}

bool StatParam::changeValueBy(int i)
{
  int valueBeforeChange, valueAfterChange;
  valueBeforeChange = value;
  value + i < 0 ? value = 0 : value += i;
  valueAfterChange = value;
  return isAction(valueBeforeChange, valueAfterChange);
}

void StatParam::pushStackInfo(pair<Ability *, int> *target)
{
  stackInfo.push_back(target);
  updateAddBy();
}

//----------------------------//

Ability::Ability(AbilitySystem *parent) : parent(parent)
{
  parent->addAbility(this);
  createStatParam(parent->getParent()->getStat(), this);
}

Ability::Ability(GameObject *target) : Ability(target->getStat()) {}

int Ability::decision(int sur, int env, int repo)
{
  passiveAction(findTargetForPassive());
  if (canActiveAction(findTargetForActive()))
  {
    return data.interpolate(sur, env, repo);
  }
  return 0;
}

//----------------------------//

bool Affliction::tick()
{
  passedTime += 1;
  if (duration <= passedTime && !getPermanant())
  {
    return false;
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

//----------------------------//
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

void AbilitySystem::decisionMakeing(int sur, int env, int repo)
{
  int maxValue, indexMax, index = 0;
  for (Ability *ability : abilityGroup)
  {
    index++;
    int value = ability->decision(sur, env, repo);
    if (maxValue < value)
    {
      maxValue = value;
      indexMax = index - 1;
    }
  }
  Ability *choosenAction = abilityGroup[indexMax];
  choosenAction->activeAction(choosenAction->findTargetForActive());
}
//----------------------------//

void AfflictionSystem::reClaCulateValue()
{
  StatusBlock *statBlock = getParent()->getStat();
  string affName;
  StatParam *paramTarget;
  statBlock->resetValue();
  for (Affliction *aff : afflictionGroup)
  {
    affName = aff->getName();
    paramTarget = statBlock->getParam(affName);
    if (paramTarget != nullptr)
    {
      paramTarget->changeValueBy(aff->getValue());
    }
  }
}

AfflictionSystem::AfflictionSystem(GameObject *parent)
    : parent(parent), afflictionGroup({}) {}

bool AfflictionSystem::addAffliction(Affliction *target)
{
  for (Affliction *aff : afflictionGroup)
  {
    if (aff->getName() == target->getName() && target->getPermanant())
    {
      return false;
    }
  }
  afflictionGroup.push_back(target);
  reClaCulateValue();
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
    {
      afflictionGroup.erase(afflictionGroup.begin() + index - 1);
      delete aff;
    }
  }
  reClaCulateValue();
}

//----------------------------//

StatusBlock::StatusBlock(GameObject *parent)
    : parent(parent), AbilitySystem(parent), AfflictionSystem(parent) {}

StatusBlock::StatusBlock(StatusBlock *copy)
    : parent(copy->getParent()), AbilitySystem(copy->getParent()),
      AfflictionSystem(copy->getParent()) {}

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
    oldStat->pushStackInfo(target->getStackInfo()[0]);
    reClaCulateValue();
    return false;
  }
  else
  {
    statParamGroup.push_back(target);
    reClaCulateValue();
    return true;
  }
}

int StatusBlock::getParamValueRaw(string name)
{
  for (StatParam *stat : statParamGroup)
  {
    if (name == stat->getName())
    {
      return stat->getRawValue();
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

//----------------------------//

GameObject::GameObject(string name, string represent, Layer *parent, int x,
                       int y)
    : name(name), represent(represent), parent(parent), x(x), y(y)
{
  parent->addToLayer(this, x, y);
}

GameObject::GameObject(string name, Layer *parent, int x, int y)
    : GameObject(name, string(1, name[0]), parent, x, y) {}

GameObject::GameObject(string name, Layer *parent)
    : name(name), represent(string(1, name[0])), parent(parent), x(0), y(0) {}

GameObject::GameObject(GameObject *copy)
    : GameObject(copy->getName(), copy->getRepresent(), copy->getParent(),
                 getPosition().first, getPosition().second) {}

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

void GameObject::update()
{
  stat->updateAffliction();
  stat->decisionMakeing(getSur(), getEnv(), getRepo());
  stat->reClaCulateValue();
}

//----------------------------//

//----------------------------//

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

//----------------------------//

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

// // get coordinate code - tba
// // pair<int, int> GameObject::getVectorIndex() const
// // {
// //     if (parent)
// //     {
// //         const vector<vector<GameObject *>> &layerVectors =
// //         parent->getLayerVectors();

// //         for (size_t i = 0; i < layerVectors.size(); ++i)
// //         {
// //             const vector<GameObject *> &currentVector = layerVectors[i];

// //             auto it = find(currentVector.begin(), currentVector.end(),
// this);

// //             if (it != currentVector.end())
// //             {
// //                 size_t j = distance(currentVector.begin(), it);
// //                 return {static_cast<int>(j), static_cast<int>(i)};
// //             }
// //         }
// //     }

// //     // Return a pair indicating not found
// //     return {-1, -1};
// // }

// // -------------------Feild--------------------//
// Feild::Feild(Layer *parentInput, string representInput, string nameInput,
//              vector<StatParameter *> *value = {},
//              vector<Ability *> *abilities = {},
//              vector<Affliction *> *afflictions = {})
//     : GameObject(parentInput, representInput, nameInput, value, abilities,
//                  afflictions) {}

// // -------------------Land--------------------//
// Land::Land(Layer *parentInput, string representInput, string nameInput, int
// x,
//            int y, vector<StatParameter *> *value = {},
//            vector<Ability *> *abilities = {},
//            vector<Affliction *> *afflictions = {})
//     : Feild(parentInput, representInput, nameInput, value, abilities,
//             afflictions),
//       x(x), y(y) {}

// // placeholder
// void Land::update() { return; }

// //-------------------Layer-------------------// clean!
// Layer::Layer(LayerSystem *parent, int width, int height, string name,
//              bool ground)
//     : parent(parent), name(name) {
//   int width_min = static_cast<int>(-floor(width / 2));
//   int height_min = static_cast<int>(-floor(height / 2));

//   for (int y = height_min; y < height_min + height; y++) {
//     vector<GameObject *> row;
//     for (int x = width_min; x < width_min + width; x++) {
//       if (ground) {
//         row.push_back(new Land(this, "#", "land", x, y));
//       } else {
//         row.push_back(nullptr);
//       }
//     }
//     Layer::layer.push_back(row);
//   }
// }

// // update everything in the layer
// void Layer::action() {
//   for (int i = 0; i < layer.size(); i++) {
//     for (int j = 0; j < layer.at(i).size(); j++) {
//       if (layer.at(i).at(j) != nullptr) {
//         layer.at(i).at(j)->update();
//       }
//     }
//   }
// }

// void inline Layer::setName(const string nameInput) { name = nameInput; }

// string inline Layer::getName() { return name; }

// LayerSystem inline *Layer::getParent() { return parent; }

// //-------------------LayerSystem-------------------//
// LayerSystem::LayerSystem(int width, int height, int amount)
//     : width(width), height(height) {
//   for (size_t lCount = 0; lCount < amount; lCount++) {
//     if (lCount == 0) {
//       createNewLayer("ground", true);
//     } else {
//       createNewLayer("layer" + to_string(lCount), false);
//     }
//   }
// }

// bool LayerSystem::createNewLayer(
//     string name, bool ground) { // ต้องสร้าง layer ที่หนาดเท่ากับ layer ที่มีอยูแล้วด้วย
//   for (Layer *l : layers) {
//     if (l->getName() == name) {
//       return false;
//     }
//   }
//   Layer *layer = new Layer(this, width, height, name, ground);
//   layers.push_back(layer);
//   return true;
// }

// bool LayerSystem::removeLayer(string name) {
//   for (int i = 0; i < layers.size(); i++) {
//     if (layers.at(i)->getName() == name) {
//       layers.erase(layers.begin() + i);
//       return true;
//     }
//   }
//   return false;
// }

// Layer inline *LayerSystem::getLayer(string name) {
//   for (Layer *l : layers) {
//     if (l->getName() == name) {
//       return l;
//     }
//   }
//   return nullptr;
// }

// Layer inline *LayerSystem::getLayer(int i) { return layers.at(i); }

// string inline LayerSystem::getLayerName(int i) {
//   return layers.at(i)->getName();
// }
// vector<Layer *> *LayerSystem::getLayers() { return &layers; }
// int inline LayerSystem::getLayerSize() { return layers.size(); }