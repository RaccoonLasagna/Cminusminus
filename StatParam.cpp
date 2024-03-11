#include "StatParam.h"
#include <iostream>

using namespace std;

Hunger::Hunger(StatusBlock *parent, Ability *createBy, int rawValue) : StatParam(parent, createBy, rawValue) {}

Hunger::Hunger(GameObject *target, Ability *createBy, int rawValue) : StatParam(target, createBy, rawValue) {}

void Hunger::action()
{
    // Animal dies here
    Layer *currentLayer = parent->getParent()->getParent();
    Layer *foodLayer = currentLayer->getParent()->getLayer("Food");
    pair<int, int> currentIndex = parent->getParent()->getVectorIndex();
    currentLayer->insideLayer[currentIndex.second][currentIndex.first] = nullptr;
    if (foodLayer->insideLayer[currentIndex.second][currentIndex.first] == nullptr)
    {
        // create a corpse
    }
    // kill this animal :(
    delete parent->getParent();
    return;
};

Health::Health(StatusBlock *parent, Ability *createBy, int rawValue) : StatParam(parent, createBy, rawValue) {}

Health::Health(GameObject *target, Ability *createBy, int rawValue) : StatParam(target, createBy, rawValue) {}

void Health::action()
{
    // Animal dies here
    Layer *currentLayer = parent->getParent()->getParent();
    Layer *foodLayer = currentLayer->getParent()->getLayer("Food");
    pair<int, int> currentIndex = parent->getParent()->getVectorIndex();
    currentLayer->insideLayer[currentIndex.second][currentIndex.first] = nullptr;
    if (foodLayer->insideLayer[currentIndex.second][currentIndex.first] == nullptr)
    {
        // create a corpse
    }
    // kill this animal :(
    delete parent->getParent();
    return;
    return;
}

Stamina::Stamina(StatusBlock *parent, Ability *createBy, int rawValue) : StatParam(parent, createBy, rawValue) {}

Stamina::Stamina(GameObject *target, Ability *createBy, int rawValue) : StatParam(target, createBy, rawValue) {}

void Stamina::action()
{
    return;
}

Atk::Atk(StatusBlock *parent, Ability *createBy, int rawValue) : StatParam(parent, createBy, rawValue) {}

Atk::Atk(GameObject *target, Ability *createBy, int rawValue) : StatParam(target, createBy, rawValue) {}

void Atk::action()
{
    return;
}

Sight::Sight(StatusBlock *parent, Ability *createBy, int rawValue) : StatParam(parent, createBy, rawValue) {}

Sight::Sight(GameObject *target, Ability *createBy, int rawValue) : StatParam(target, createBy, rawValue) {}

void Sight::action()
{
    return;
}

Speed::Speed(StatusBlock *parent, Ability *createBy, int rawValue) : StatParam(parent, createBy, rawValue) {}

Speed::Speed(GameObject *target, Ability *createBy, int rawValue) : StatParam(target, createBy, rawValue) {}

void Speed::action()
{
    return;
}