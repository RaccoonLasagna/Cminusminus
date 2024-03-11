#include "Ability.h"
#include "Affliction.h"
#include "Affliction.cpp"
#include "FundamentalSystem.cpp"
#include "FundamentalSystem.h"
#include "StatParam.h"
#include "StatParam.cpp"
#include <cstddef>
#include <cstdlib>
#include <ctime>

using namespace std;

// ===================================== ANIMALS =====================================

//-------------------------- Eat ----------------------------//

Eat::Eat(AbilitySystem *parent) : Ability(parent) {}

Eat::Eat(GameObject *target) : Ability(target) {}

void Eat::createStatParam()
{
  new Hunger(getParent()->getParent(), this, 100);
}

// Eating earthworms is passive
vector<GameObject *> Eat::findTargetForPassive() { 
  vector<GameObject *> targetInRange = parent->getParent()->findTargetInRange(1, false);
  while (targetInRange.size() > 0)
  {
    if (targetInRange[0]->getName() != "Earthworm")
    {
      targetInRange.erase(targetInRange.begin());
    } else {
      break;
    }
  }
  
  if (targetInRange.size() > 0){
    GameObject* target = targetInRange[0];
    pair<int, int> targetPosition = target->getVectorIndex();
    Layer* currentLayer = parent->getParent()->getParent();
    // kill the earthworm
    currentLayer->insideLayer[targetPosition.second][targetPosition.first] = nullptr;
    delete target;
    // reduced hunger
    getParent()->getParent()->getStat()->removeAffliction("Hungry");
  }
  return {}; 
}

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
  Hunger_Aff *hungry = new Hunger_Aff(getParent()->getParent(), 1, 1, 0);
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
  // Full *full = new Full(getParent()->getParent());
  return;
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
  return {possible_targets[rand() % possible_targets.size()]};
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

Walk::Walk(AbilitySystem *parent) : Ability(parent) {}

Walk::Walk(GameObject *target) : Ability(target) {}

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
  int unwalkableTileCount = 0;
  Layer *selfLayer = parent->getParent()->getParent();
  pair<int, int> selfCoord = parent->getParent()->getVectorIndex();
  int currentX = selfCoord.first;
  int currentY = selfCoord.second;
  // check 8 tiles around the animal
  for (int coordY : {currentY - 1, currentY, currentY + 1})
  {
    for (int coordX : {currentX - 1, currentX, currentX + 1})
    {
      // if the coordinate is within bounds,
      if (coordX >= 0 && coordX < selfLayer->getParent()->getLayersWidth() && coordY >= 0 && coordY < selfLayer->getParent()->getLayersHeight())
      {
        // if the tile isn't nullptr and the tile isnt the same as the animal
        if (selfLayer->insideLayer[coordY][coordX] != nullptr && (coordX != currentX || coordY != currentY))
        {
          unwalkableTileCount++;
        }
      }
      else // if the coordinate is out of bounds, it is unwalkable
      {
        unwalkableTileCount++;
      }
    }
  }

  return (unwalkableTileCount < 8);
}

// no passives
void Walk::passive(vector<GameObject *> targets)
{
  return;
}

// --------------------------- WalkSeek ---------------------------

WalkSeek::WalkSeek(AbilitySystem *parent) : Walk(parent) {}

WalkSeek::WalkSeek(GameObject *target) : Walk(parent) {}

// walk by setting another unoccupied tile to be this object's pointer and set the previous tile to nullptr
void WalkSeek::active(vector<GameObject *> targets)
{
  Layer *currentLayer = parent->getParent()->getParent();
  int currentX = parent->getParent()->getVectorIndex().first;
  int currentY = parent->getParent()->getVectorIndex().second;
  // code to choose a target
  int targetX, targetY;
  if (targets.size() > 0) // if there's a target, set target coords to the variables
  {
    GameObject *target = targets[0];
    targetX = target->getVectorIndex().first;
    targetY = target->getVectorIndex().second;
  }
  else // else, set target coords a random 8 tile around the parent
  {
    // all coordinates around the parent that isn't out of bounds
    vector<pair<int, int>> possibleTargets;

    for (int coordY : {currentY - 1, currentY, currentY + 1})
    {
      for (int coordX : {currentX - 1, currentX, currentX + 1})
      {
        // if x and y are in range, if the tile is empty, and isn't the same as the parent
        if (coordX >= 0 && coordX < currentLayer->getParent()->getLayersWidth() &&
            coordY >= 0 && coordY < currentLayer->getParent()->getLayersHeight() &&
            currentLayer->insideLayer[coordY][coordX] == nullptr &&
            (coordX != currentX || coordY != currentY))
        {
          possibleTargets.push_back({coordX, coordY});
        }
      }
    }

    // randomize target coords
    if (possibleTargets.size() > 0)
    {
      pair<int, int> targetPosition = possibleTargets[rand() % possibleTargets.size()];
      targetX = targetPosition.first;
      targetY = targetPosition.second;
    }
    else
    {
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
  if (xDiff < 0)
  {
    nextX = currentX - 1;
  }
  if (yDiff > 0) // target above
  {
    nextY = currentY + 1; // walk up
  }
  if (yDiff < 0)
  {
    nextY = currentY - 1;
  }
  // if the next position is empty, set the next position to be the parent's pointer
  if (currentLayer->insideLayer[nextY][nextX] == nullptr)
  {
    currentLayer->setLayer(parent->getParent(), nextX, nextY);
    currentLayer->setLayer(nullptr, currentX, currentY);
  }

  return;
}

// --------------------------- WalkEscape ---------------------------

void WalkEscape::active(vector<GameObject *> targets)
{
  Layer *currentLayer = parent->getParent()->getParent();
  int currentX = parent->getParent()->getVectorIndex().first;
  int currentY = parent->getParent()->getVectorIndex().second;
  // code to choose a target
  int targetX, targetY;
  if (targets.size() > 0) // hopefully there's always going to be a target to escape from?
  {
    GameObject *target = targets[0];
    targetX = target->getVectorIndex().first;
    targetY = target->getVectorIndex().second;
  }
  else // else, set target coords a random 8 tile around the parent
  {
    // all coordinates around the parent that isn't out of bounds
    vector<pair<int, int>> possibleTargets;

    for (int coordY : {currentY - 1, currentY, currentY + 1})
    {
      for (int coordX : {currentX - 1, currentX, currentX + 1})
      {
        // if x and y are in range, if the tile is empty, and isn't the same as the parent
        if (coordX >= 0 && coordX < currentLayer->getParent()->getLayersWidth() &&
            coordY >= 0 && coordY < currentLayer->getParent()->getLayersHeight() &&
            currentLayer->insideLayer[coordY][coordX] == nullptr &&
            coordX != currentX && coordY != currentY)
        {
          possibleTargets.push_back({coordX, coordY});
        }
      }
    }

    // randomize target coords until they aren't the same as current coords
    do
    {
      pair<int, int> targetPosition = possibleTargets[rand() % possibleTargets.size()];
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
  if (xDiff < 0)
  {
    nextX = currentX + 1;
  }
  if (yDiff > 0) // currently above the target
  {
    nextY = currentY - 1; // walk down
  }
  if (yDiff < 0)
  {
    nextY = currentY + 1;
  }

  // if target position is in bounds and is empty, move there
  if (nextX >= 0 && nextX < currentLayer->getParent()->getLayersWidth() &&
      nextY >= 0 && nextY < currentLayer->getParent()->getLayersHeight() &&
      currentLayer->insideLayer[nextY][nextX] != nullptr)
  {
    currentLayer->setLayer(parent->getParent(), nextX, nextY);
    currentLayer->setLayer(nullptr, currentX, currentY);
  }

  return;
}

//--------------------------- Mate ---------------------------

// no stats neede
void Mate::createStatParam()
{
  return;
}

// no passives
vector<GameObject *> Mate::findTargetForPassive()
{
  return {};
}

// 1 random target of the same species within range
vector<GameObject *> Mate::findTargetForActive()
{
  // get all animals in range
  vector<GameObject *> animals_in_range = parent->getParent()->findTargetInRange(1, false);
  vector<GameObject *> targets;
  for (GameObject *target : animals_in_range)
  {
    if (target->getName() == parent->getParent()->getName())
    {
      targets.push_back(target);
    }
  }
  return targets;
};

// at least 1 tile around must be empty, contain the same species, and ability not on cooldown
bool Mate::canActive(vector<GameObject *> targets)
{
  // same algorithm as canActive for walk, counting how many empty tiles there are around
  int unspawnableTile = 0;
  Layer *selfLayer = parent->getParent()->getParent();
  pair<int, int> selfCoord = parent->getParent()->getVectorIndex();
  int currentX = selfCoord.first;
  int currentY = selfCoord.second;
  for (int coordY : {currentY - 1, currentY, currentY + 1})
  {
    for (int coordX : {currentX - 1, currentX, currentX + 1})
    {
      if (coordX >= 0 && coordX < selfLayer->getParent()->getLayersWidth() && coordY >= 0 && coordY < selfLayer->getParent()->getLayersHeight())
      {
        if (selfLayer->insideLayer[coordY][coordX] != nullptr && (coordX != currentX || coordY != currentY))
        {
          unspawnableTile++;
        }
      }
      else
      {
        unspawnableTile++;
      }
    }
  }

  return !(parent->getParent()->getStat()->isInAffliction("MateCooldown")) && findTargetForActive().size() > 0 && unspawnableTile < 8;
};

// no passives
void Mate::passive(vector<GameObject *> targets)
{
  return;
}

void Mate::active(vector<GameObject *> targets)
{
  
  // find all nullptr tiles in the 8 tiles around
  vector<pair<int, int>> possibleTargets;

  pair<int, int> selfCoord = parent->getParent()->getVectorIndex();
  int currentX = selfCoord.first;
  int currentY = selfCoord.second;
  Layer *currentLayer = parent->getParent()->getParent();

  for (int coordY : {currentY - 1, currentY, currentY + 1})
  {
    for (int coordX : {currentX - 1, currentX, currentX + 1})
    {
      // if x and y are in range, if the tile is empty, and isn't the same as the parent
      if (coordX >= 0 && coordX < currentLayer->getParent()->getLayersWidth() &&
          coordY >= 0 && coordY < currentLayer->getParent()->getLayersHeight() &&
          currentLayer->insideLayer[coordY][coordX] == nullptr &&
          (coordX != currentX || coordY != currentY))
      {
        possibleTargets.push_back({coordX, coordY});
      }
    }
  }

  if (possibleTargets.size() > 0)
  {
    pair<int, int> targetCoord = possibleTargets[rand() % possibleTargets.size()];
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

// ===================================== PLANTS =====================================

// --------------------------- Fruition ---------------------------

Fruition::Fruition(AbilitySystem *target) : Ability(target) {}

Fruition::Fruition(GameObject *target) : Ability(target) {}

vector<GameObject *> Fruition::findTargetForPassive()
{
  return {};
}

vector<GameObject *> Fruition::findTargetForActive()
{
  return {};
};

bool Fruition::canActive(vector<GameObject *> targets){
  return true;
}

// Ima store fruits as afflictions actually
void Fruition::passive(vector<GameObject *> targets)
{
  if (!parent->getParent()->getStat()->isInAffliction("FruitionCooldown")){ // if not on cooldown
    int fruitAmount = parent->getParent()->getStat()->amountOfAffliction("Fruit");
    if (fruitAmount < 20){
      // generate fruits, set ability on cooldown
      new FruitionCooldown(parent->getParent(), 200);
      for (size_t i = 0; i < 5; i++)
      {
        new Fruit(parent->getParent(), 1);
      }
    }
  }
  return;
}

void Fruition::active(vector<GameObject *> targets)
{
  return;
}

