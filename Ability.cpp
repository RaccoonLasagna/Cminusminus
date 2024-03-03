#include "Ability.h"
#include "Affliction.h"
#include "FundamentalSystem.cpp"
#include "FundamentalSystem.h"
#include "StatParam.h"
#include <cstddef>
#include <cstdlib>
#include <ctime>

using namespace std;

//-------------------------- Playturn ----------------------------//

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

bool PlayTurn::canActive(vector<GameObject *> targets) { return false; }

void PlayTurn::passive(vector<GameObject *> targets) {
  return;
}

void PlayTurn::active(vector<GameObject *> targets) {
  return;
}

//-------------------------- Eat ----------------------------//

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

//-------------------------- Rest ----------------------------//

// sleeping replenishes stamina, so give animals stamina
void Rest::createStatParam() {
  StatParam *aff = new Stamina(getParent()->getParent(), this, 100);
}

// An animal can always rest
bool Rest::canActive(vector<GameObject *> targets){
  return targets.size() > 0;
}

// sleeping adds stamina through adding "stamina surge" affliction
void Rest::active(vector<GameObject *> targets){
  GameObject* animal = parent->getParent();
  StatusBlock *stats = animal->getStat();
  int current_stamina = stats->getParamValueRaw("Stamina");
  current_stamina++;
  // animal->getStat()->addAffliction(StaminaSurge);
}

void passive(vector<GameObject *> targets);
void active(vector<GameObject *> targets);

//--------------------------- Attack ----------------------------//

// you need atk to attack
void Attack::createStatParam(){
  StatParam *aff = new Atk(getParent()->getParent(), this, 100);
}

// no passives
vector<GameObject *> Attack::findTargetForPassive(){
  return {};
}

vector<GameObject *> Attack::findTargetForActive(){
  vector<GameObject *> possible_targets = parent->getParent()->findTargetInRange(1);
}

bool Attack::canActive(vector<GameObject *> targets){
  vector<GameObject *> possible_targets = parent->getParent()->findTargetInRange(1);
  return (possible_targets.size() > 0);
}

void Attack::passive(vector<GameObject *> targets){
  return;
}

// deal damage by inflicting bleed
void Attack::active(vector<GameObject *> targets){
  for (GameObject* target: targets)
  {
    // target->getStat()->addAffliction(Bleed);
  }
}

// --------------------------- Walk ---------------------------


void createStatParam(){

}

vector<GameObject *> findTargetForPassive(){

}

vector<GameObject *> findTargetForActive(){

}

bool canActive(vector<GameObject *> targets){

}

void passive(vector<GameObject *> targets){

}

void active(vector<GameObject *> targets){

}