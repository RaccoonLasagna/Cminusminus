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

void PlayTurn::createStatParam()
{
  new Turn(getParent()->getParent(), this, 1);
}

vector<GameObject *> PlayTurn::findTargetForPassive()
{
  vector<GameObject *> target = {getParent()->getParent()};
  return target;
}

vector<GameObject *> PlayTurn::findTargetForActive()
{
  vector<GameObject *> target = {getParent()->getParent()};
  return target;
}

bool PlayTurn::canActive(vector<GameObject *> targets) { return false; }

void PlayTurn::passive(vector<GameObject *> targets)
{
  return;
}

void PlayTurn::active(vector<GameObject *> targets)
{
  return;
}

//-------------------------- Eat ----------------------------//

void Eat::createStatParam()
{
  StatParam *aff = new Hunger(getParent()->getParent(), this, 100);
}

vector<GameObject *> Eat::findTargetForPassive() { return {}; }

vector<GameObject *> Eat::findTargetForActive()
{
  vector<GameObject *> target = {};
  Layer *foodLayer =
      getParent()->getParent()->getParent()->getParent()->getLayer("Food");
  GameObject *self = getParent()->getParent();
  for (vector<GameObject *> row : foodLayer->insideLayer)
  {
    for (GameObject *food : row)
    {
      if (foodLayer->getParent()->getDistance(food, getParent()->getParent()) <=
              1 &&
          food != self)
      {
        target.push_back(food);
      }
    }
  }
  return target;
}

bool Eat::canActive(vector<GameObject *> target)
{
  Layer *foodLayer =
      getParent()->getParent()->getParent()->getParent()->getLayer("Food");
  GameObject *self = getParent()->getParent();
  for (vector<GameObject *> row : foodLayer->insideLayer)
  {
    for (GameObject *food : row)
    {
      if (foodLayer->getParent()->getDistance(food, getParent()->getParent()) <=
              1 &&
          food != self)
      {
        return true;
      }
    }
  }
  return false;
}

void Eat::passive(vector<GameObject *> target)
{
  Hungry *hungry = new Hungry(getParent()->getParent());
  return;
}

void Eat::active(vector<GameObject *> target)
{
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
void Rest::createStatParam()
{
  StatParam *aff = new Stamina(getParent()->getParent(), this, 100);
}

// no passives
vector<GameObject *> Rest::findTargetForPassive()
{
  return {};
}

// no targets
vector<GameObject *> Rest::findTargetForActive()
{
  return {};
}

// An animal can always rest
bool Rest::canActive(vector<GameObject *> targets)
{
  return true;
}

// no passives :(
void Rest::passive(vector<GameObject *> targets)
{
  return;
}

// resting removes a stack of fatigue
void Rest::active(vector<GameObject *> targets)
{
  GameObject *animal = parent->getParent();
  StatusBlock *stats = animal->getStat();
  stats->removeAffliction("Fatigue");
}

//--------------------------- Attack ----------------------------//

// you need atk to attack
void Attack::createStatParam()
{
  new Atk(getParent()->getParent(), this, 100);
}

// no passives
vector<GameObject *> Attack::findTargetForPassive()
{
  return {};
}

vector<GameObject *> Attack::findTargetForActive()
{
  vector<GameObject *> possible_targets = parent->getParent()->findTargetInRange(1, false);
}

bool Attack::canActive(vector<GameObject *> targets)
{
  vector<GameObject *> possible_targets = parent->getParent()->findTargetInRange(1, false);
  return (possible_targets.size() > 0);
}

// no passives yet
void Attack::passive(vector<GameObject *> targets)
{
  return;
}

// deal damage by inflicting bleed to all targets
void Attack::active(vector<GameObject *> targets)
{
  for (GameObject *target : targets)
  {
    int atkValue = parent->getParent()->getStat()->getParamValue("Atk");
    // new Bleed(target, atkValue, atkValue/10, 5)
  }
}

// --------------------------- Walk ---------------------------

// walk doesn't scale with any stats, no stat creation needed
void Walk::createStatParam()
{
  return;
}

// Walking has no passives
vector<GameObject *> Walk::findTargetForPassive()
{
  return {};
}

// Walk direction target is the closest target in sight
vector<GameObject *> Walk::findTargetForActive()
{
  int visionRange = parent->getParent()->getStat()->getParamValue("Vision");
  vector<GameObject *> targets = parent->getParent()->findTargetInRange(visionRange, false);
  GameObject *closestObj = targets[0];
  for (GameObject *obj : targets)
  {
    int xDiff = parent->getParent()->getCoord().first - obj->getCoord().first;
    int yDiff = parent->getParent()->getCoord().second - obj->getCoord().second;
    int distance = sqrt(xDiff * xDiff + yDiff * yDiff);
    if (distance < sqrt(closestObj->getCoord().first * closestObj->getCoord().first + closestObj->getCoord().second * closestObj->getCoord().second))
    {
      closestObj = obj;
    }
  }
  return {closestObj};
}

// if you have 8 or more in range, you can't walk
bool Walk::canActive(vector<GameObject *> targets)
{
  return !(parent->getParent()->findTargetInRange(1, false).size() >= 8);
}

// no passives
void Walk::passive(vector<GameObject *> targets)
{
  return;
}

// walk by setting another unoccupied tile to be this object's pointer and set the previous tile to nullptr
void WalkSeek::active(vector<GameObject *> targets)
{
  Layer *currentLayer = parent->getParent()->getParent();
  int currentX = parent->getParent()->getCoord().first;
  int currentY = parent->getParent()->getCoord().second;
  // code to choose a target
  int targetX, targetY;
  if (targets.size() > 0) // if there's a target, set target coords to the variables
  {
    GameObject *target = targets[0];
    targetX = target->getCoord().first;
    targetY = target->getCoord().second;
  }
  else // else, set target coords a random 8 tile around the parent
  {
    // all coordinates around the parent that isn't out of bounds
    vector<int> possibleTargetX;
    vector<int> possibleTargetY;
    for (int coordX : {currentX - 1, currentX, currentX + 1})
    {
      if (coordX >= 0 && coordX < currentLayer->getParent()->getLayersWidth())
      {
        possibleTargetX.push_back(coordX);
      }
    }
    for (int coordY : {currentY - 1, currentY, currentY + 1})
    {
      if (coordY >= 0 && coordY < currentLayer->getParent()->getLayersHeight())
      {
        possibleTargetY.push_back(coordY);
      }
    }
    
    // randomize target coords until they aren't the same as current coords
    do {
      targetX = possibleTargetX[rand() % possibleTargetX.size()];
      targetY = possibleTargetY[rand() % possibleTargetY.size()];
    } while (targetX == currentX && targetY == currentY);
  }

  // code to walk towards the selected target
  int xDiff = targetX - currentX;
  int yDiff = targetY - currentY;
  int nextX = currentX;
  int nextY = currentY;
  // walk in the target's direction
  if (xDiff > 0)
  {
    nextX = currentX + 1;
  }
  if (xDiff < 0)
  {
    nextX = currentX - 1;
  }
  if (yDiff > 0)
  {
    nextY = currentY + 1;
  }
  if (yDiff < 0)
  {
    nextY = currentY - 1;
  }
  // no checks for out of bounds needed, since the target won't be out of bounds
  currentLayer->setLayer(parent->getParent(), nextX, nextY);
  currentLayer->setLayer(nullptr, currentX, currentY);

  return;
}

void WalkEscape::active(vector<GameObject *> targets)
{
  return;
}