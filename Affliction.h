#include "FundamentalSystem.h"
#include "FundamentalSystem.cpp"
using namespace std;

#ifndef AFFLICTION_H
#define AFFLICTION_H

class NCDs : public Affliction { 
    private:
        bool tick() override;
    public:
        NCDs(AfflictionSystem *parent, int duration, string name);
        ~NCDs();
};

class InfectiousDisease : public Affliction {
    private:
        bool virtual canSpread(GameObject *target) = 0;
        // void virtual action(GameObject *target) = 0;
    public:
        InfectiousDisease(AfflictionSystem *parent, int duration, string name);
        ~InfectiousDisease();
        void virtual spread(GameObject *target) = 0;
};

class Blind : public NCDs {
    private:
        void action() override;
    public:
        Blind(AfflictionSystem *parent, int duration, string name);
        ~Blind();
};

class Posion : public InfectiousDisease {
    private:
        bool canSpread(GameObject *target) override;
        void action() override;
    public:
        Posion(AfflictionSystem *parent, int duration, string name);
        ~Posion();
        void spread(GameObject *target) override;
};

#endif