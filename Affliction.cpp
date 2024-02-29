#include "Affliction.h"

using namespace std;

Full::Full(StatusBlock *parent) : Affliction(parent) {}
Full::Full(GameObject *target) : Affliction(target) {}

Hungry::Hungry(StatusBlock *parent) : Affliction(parent) {}
Hungry::Hungry(GameObject *target) : Affliction(target) {}