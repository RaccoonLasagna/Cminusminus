#include "FundamentalSystem.h"

using namespace std;

#ifndef ABILITY_CPP
#define ABILITY_CPP

class TurnEater : public Affliction {

};

class Full : public Affliction {
    protected:
    string name = "Food";
    string targetValue = "Hunger";
    int duration = 5;
    int valueIncrease = 0;
    int value = 20;
    bool permanent = false;
    public:
    Full(StatusBlock *parent);
    Full(GameObject *target);
};

class Hungry : public Affliction {
    protected:
    string name = "Hungry";
    string targetValue = "Hunger";
    int duration = 21;
    int valueIncrease = 0;
    int value = -5;
    bool permanent = false;
    public:
    Hungry(StatusBlock *parent);
    Hungry(GameObject *target);
};




#endif