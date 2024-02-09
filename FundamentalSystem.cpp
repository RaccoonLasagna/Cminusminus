#include "FundamentalSystem.h"
#include <vector>
// #include <iterator>
using namespace std;

//-------------------StatParameterMax-------------------//
void StatParameterMax::increse(const int i) {
  int value = getValue();
  if (value + i <= maxValue) {
    value += i;
  } else {
    value = maxValue;
  }
}

void StatParameterMax::decrese(const int i) {
  int value = getValue();
  if (value - i >= 0) {
    value -= i;
  } else {
    value = 0;
  }
}

//-------------------StatParameterCoord-------------------//
bool StatParameterCoord::changeCoord(const int x, const int y) {
  int newX = xValue + x;
  int newY = yValue + y;
  if (newX >= xMIN && xMAX >= newX && newY >= yMIN && yMAX >= newY) {
    xValue = newX;
    yValue = newY;
    return true;
  } else {
    return false;
  }
}

bool StatParameterCoord::setCoord(const int x, const int y) {
  if (x >= xMIN && xMAX >= x && y >= yMIN && yMAX >= y) {
    xValue = x;
    yValue = y;
    return true;
  } else {
    return false;
  }
}

//-------------------Ability-------------------//

//-------------------AbilitySystem-------------------//
bool AbilitySystem::addAbility(Ability *ability) {
  for (Ability *a : abilities) {
    if (a->getName() == ability->getName()) {
      return false;
    }
  }
  abilities.push_back(ability);
  return true;
}

bool AbilitySystem::removeAbility(const string name) {
  for (int i = 0; i < abilities.size(); i++) {
    if (abilities.at(i)->getName() == name) {
      abilities.erase(abilities.begin() + i);
      return true;
    }
  }
  return false;
}

void AbilitySystem::decisionMakeing(const int needSur, const int needEnv,
                                    const int needRepr) const {
  int maxValue = 0;
  int maxIndex = 0;
  for (int i = 0; i < abilities.size(); i++) {
    int value = abilities.at(i)->decision(needSur, needEnv, needRepr);
    if (value > maxValue) {
      maxValue = value;
      maxIndex = i;
    }
  }
  abilities.at(maxIndex)->action();
}

//-------------------Affliction-------------------//
void Affliction::update() {
  tick();
  action();
}

bool Affliction::tick() {
  if (duration > 0) {
    duration--;
    return true;
  }
  return false;
}

//-------------------AfflictionSystem-------------------//
bool AfflictionSystem::addAffliction(Affliction *affliction) {
  string name = affliction->getName();
  for (Affliction *a : afflictions) {
    string tempName = a->getName();
    if (tempName == name) {
      removeAffliction(tempName);
      afflictions.push_back(affliction);
      return false;
    }
  }
  afflictions.push_back(affliction);
  return true;
}

bool AfflictionSystem::removeAffliction(const string name) {
  for (int i = 0; i < afflictions.size(); i++) {
    if (afflictions.at(i)->getName() == name) {
      afflictions.erase(afflictions.begin() + i);
      return true;
    }
  }
  return false;
}

bool AfflictionSystem::checkAlive() {
  if (parent->getStat()->getHealth()) {
    return false;
  }
  return true;
}

bool AfflictionSystem::updateAffliction() {
  for (Affliction *a : afflictions) {
    a->update();
  }
  return checkAlive();
}

//-------------------StatusSystem-------------------//

//-------------------GameObject-------------------//
void GameObject::setStat(StatusSystem *statInput) {
  if (statInput->getParent() !=nullptr){
    statInput->getParent()->stat = nullptr;
  }
  statInput->setParent(this);
  stat = statInput;
}

//-------------------Layers-------------------//

//-------------------Environment-------------------//
// void Environment::update() {
//   for (Layer layer : layers) {
//   }
// }