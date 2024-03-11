#include "FundamentalSystem.h"

using namespace std;

#ifndef STATPARAM_H
#define STATPARAM_H

class Hunger : public StatParam {
    protected:
    string name = "Hunger";
    int targetValue = 0;
    void virtual action() override;
    public:
    Hunger(StatusBlock *parent, Ability *createBy, int rawValue);
    Hunger(GameObject *target, Ability *createBy, int rawValue);
};

// No abilities create this, include for all living things
class Health : public StatParam {
    protected:
    string name = "Health";
    int targetValue = 0;
    void virtual action() override;
    public:
    Health(StatusBlock *parent, Ability *createBy, int rawValue);
    Health(GameObject *target, Ability *createBy, int rawValue);
};

class Stamina : public StatParam {
    protected:
    string name = "Stamina";
    int targetValue = 0;
    void virtual action() override;
    public:
    Stamina(StatusBlock *parent, Ability *createBy, int rawValue);
    Stamina(GameObject *target, Ability *createBy, int rawValue);
};

class Atk : public StatParam {
    protected:
    string name = "Atk";
    int targetValue = 0;
    void virtual action() override;
    public:
    Atk(StatusBlock *parent, Ability *createBy, int rawValue);
    Atk(GameObject *target, Ability *createBy, int rawValue);
};

class Sight : public StatParam {
    protected:
    string name = "Sight";
    int targetValue = 0;
    void virtual action() override;
    public:
    Sight(StatusBlock *parent, Ability *createBy, int rawValue);
    Sight(GameObject *target, Ability *createBy, int rawValue);
};

class Speed : public StatParam {
    protected:
    string name = "Speed";
    int targetValue = 0;
    void virtual action() override;
    public:
    Speed(StatusBlock *parent, Ability *createBy, int rawValue);
    Speed(GameObject *target, Ability *createBy, int rawValue);
};

#endif