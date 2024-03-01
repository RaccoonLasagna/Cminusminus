#include "Ability.h"
#include "Affliction.h"
#include "FundamentalSystem.cpp"
#include "FundamentalSystem.h"
#include "StatParam.h"
#include <cstddef>
#include <cstdlib>
#include <ctime>

using namespace std;

//--------------------------PLAYTURN----------------------------//

void PlayTurn::produceAffliction() { return; }

void PlayTurn::createStatParam() {
  StatParam *aff = new Turn(getParent()->getParent(), this, 1);
}

vector<GameObject *> PlayTurn::findTargetForPassive() {
  vector<GameObject *> target = {getParent()->getParent()};
  return target;
}

vector<GameObject *> PlayTurn::findTargetForActive() {
  vector<GameObject *> target = {getParent()->getParent()};
  return target;
}

bool PlayTurn::canActive(vector<GameObject *> target) { return false; }

void PlayTurn::passive(vector<GameObject *> target) {
  return;
}

//--------------------------EAT----------------------------//

void Eat::produceAffliction() { return; }

void Eat::createStatParam() {
  StatParam *aff = new Hunger(getParent()->getParent(), this, 100);
}

vector<GameObject *> Eat::findTargetForPassive() { return {}; }

vector<GameObject *> Eat::findTargetForActive() {
  vector<GameObject *> target = {};
  Layer *foodLayer =
      getParent()->getParent()->getParent()->getParent()->getLayer("Food");
  GameObject *self = getParent()->getParent();
  for (vector<GameObject *> row : foodLayer->insideLayer) {
    for (GameObject *food : row) {
      if (foodLayer->getParent()->getDistance(food, getParent()->getParent()) <=
              1 &&
          food != self) {
        target.push_back(food);
      }
    }
  }
  return target;
}

bool Eat::canActive(vector<GameObject *> target) {
  Layer *foodLayer =
      getParent()->getParent()->getParent()->getParent()->getLayer("Food");
  GameObject *self = getParent()->getParent();
  for (vector<GameObject *> row : foodLayer->insideLayer) {
    for (GameObject *food : row) {
      if (foodLayer->getParent()->getDistance(food, getParent()->getParent()) <=
              1 &&
          food != self) {
        return true;
      }
    }
  }
  return false;
}

void Eat::passive(vector<GameObject *> target) {
  Hungry *hungry = new Hungry(getParent()->getParent());
  return;
}

void Eat::active(vector<GameObject *> target) {
  srand(time(NULL));
  GameObject *realTarget = target[rand() % target.size()];
  pair<int, int> coord = realTarget->getCoord();
  getParent()
      ->getParent()
      ->getParent()
      ->getParent()
      ->getLayer("Food")
      ->removeFromLayer(coord.first, coord.second);
  getParent()->getParent()->getStat()->removeAffliction("Hungry");
  Full *full = new Full(getParent()->getParent());
  return;
}

//--------------------------REST----------------------------//

void Rest::produceAffliction() { return; }

void Rest::createStatParam() {
  StatParam *aff = new Stemina(getParent()->getParent(), this, 100);
}

//---------------------------ATTACK----------------------------//

// Attack::Attack(AbilitySystem *parent) : Ability(parent) {}

void Attack::produceAffliction() {

  return;
  }