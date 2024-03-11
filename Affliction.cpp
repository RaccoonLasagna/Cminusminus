#include "Affliction.h"

using namespace std;

Health_Aff::Health_Aff(StatusBlock *parent, int duration, int valueIncrease, int value) 
: Affliction(parent), duration(duration),valueIncrease(valueIncrease),value(value)
{}
Health_Aff::Health_Aff(GameObject *target, int duration, int valueIncrease, int value) 
: Affliction(target), duration(duration),valueIncrease(valueIncrease),value(value)
{}

Hunger_Aff::Hunger_Aff(StatusBlock *parent, int duration, int valueIncrease, int value) 
: Affliction(parent), duration(duration),valueIncrease(valueIncrease),value(value)
{}
Hunger_Aff::Hunger_Aff(GameObject *target, int duration, int valueIncrease, int value) 
: Affliction(target), duration(duration),valueIncrease(valueIncrease),value(value)
{}

Attack_Aff::Attack_Aff(StatusBlock *parent, int duration, int valueIncrease, int value) 
: Affliction(parent), duration(duration),valueIncrease(valueIncrease),value(value)
{}
Attack_Aff::Attack_Aff(GameObject *target, int duration, int valueIncrease, int value) 
: Affliction(target), duration(duration),valueIncrease(valueIncrease),value(value)
{}

Sight_Aff::Sight_Aff(StatusBlock *parent, int duration, int value) 
: Affliction(parent), duration(duration),value(value)
{}
Sight_Aff::Sight_Aff(GameObject *target, int duration, int value) 
: Affliction(target), duration(duration),value(value)
{}

Speed_Aff::Speed_Aff(StatusBlock *parent, int duration, int valueIncrease, int value) 
: Affliction(parent), duration(duration),valueIncrease(valueIncrease),value(value)
{}
Speed_Aff::Speed_Aff(GameObject *target, int duration, int valueIncrease, int value) 
: Affliction(target), duration(duration),valueIncrease(valueIncrease),value(value)
{}

MateCooldown::MateCooldown(StatusBlock *parent, int duration) 
: Affliction(parent), duration(duration)
{}
MateCooldown::MateCooldown(GameObject *target, int duration) 
: Affliction(target), duration(duration)
{}

EatTimes::EatTimes(StatusBlock *parent) 
: Affliction(parent)
{}
EatTimes::EatTimes(GameObject *target) 
: Affliction(target)
{}