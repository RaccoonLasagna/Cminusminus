#include "FundamentalSystem.h"

using namespace std;

#ifndef ABILITY_H
#define ABILITY_H

//--------------------------- PlayTurn ---------------------------

class PlayTurn : public Ability {
protected:
  string name = "PlayTurn";
  Var3DGraph *data =
      new Var3DGraph({{0, 0, 0, 0}, {100, 100, 100, 0}}); // เดี๋ยวค่อยคิดใหม่
  void createStatParam() override;

public:
  PlayTurn(AbilitySystem *parent);
  PlayTurn(GameObject *target);
  vector<GameObject *> virtual findTargetForPassive() override;
  vector<GameObject *> virtual findTargetForActive() override;
  bool virtual canActive(vector<GameObject *> targets) override;
  void virtual passive(vector<GameObject *> targets) override;
  void virtual active(vector<GameObject *> targets) override;
};

//--------------------------- Eat ---------------------------

class Eat : public Ability {
protected:
  string name = "Eat";
  bool eatPlants;
  bool eatAnimals;
  Var3DGraph *data = new Var3DGraph({{0, 0, 0, 0},
                                     {0, 100, 0, 100},
                                     {100, 100, 0, 40},
                                     {0, 0, 100, 10},
                                     {30, 80, 5, 70},
                                     {100, 100, 100, 25},
                                     {50, 10, 100, 10}}); // เดี๋ยวค่อยคิดใหม่
  void createStatParam() override;

public:
  Eat(AbilitySystem *parent);
  Eat(GameObject *target);
  vector<GameObject *> virtual findTargetForPassive() override;
  vector<GameObject *> virtual findTargetForActive() override;
  bool virtual canActive(vector<GameObject *> targets) override;
  void virtual passive(vector<GameObject *> targets) override;
  void virtual active(vector<GameObject *> targets) override;
};



//--------------------------- Rest ---------------------------

class Rest : public Ability {
protected:
  string name = "Rest";
  Var3DGraph *data = new Var3DGraph({{0, 0, 0, 0},
                                     {0, 100, 0, 100},
                                     {100, 100, 0, 40},
                                     {0, 0, 100, 10},
                                     {30, 80, 5, 70},
                                     {100, 100, 100, 25}}); // เดี๋ยวค่อยคิดใหม่
  void createStatParam() override;

public:
  Rest(AbilitySystem *parent);
  Rest(GameObject *target);
  vector<GameObject *> virtual findTargetForPassive() override;
  vector<GameObject *> virtual findTargetForActive() override;
  bool virtual canActive(vector<GameObject *> targets) override;
  void virtual passive(vector<GameObject *> targets) override;
  void virtual active(vector<GameObject *> targets) override;
};

//--------------------------- Attack ---------------------------

class Attack: public Ability{
protected:
string name = "Attack";
Var3DGraph *data = new Var3DGraph({{0, 0, 0, 0},
                                   {0, 100, 0, 100},
                                   {100, 100, 0, 40},
                                   {0, 0, 100, 10},
                                   {30, 80, 5, 70},
                                   {100, 100, 100, 25}}); // remember to change pls
void createStatParam() override;

public:
Attack(AbilitySystem *parent);
Attack(GameObject *target);
vector<GameObject *> virtual findTargetForPassive() override;
vector<GameObject *> virtual findTargetForActive() override;
bool virtual canActive(vector<GameObject *> targets) override;
void virtual passive(vector<GameObject *> targets) override;
void virtual active(vector<GameObject *> targets) override;
};

//--------------------------- Walk ---------------------------

class Walk: public Ability{
protected:
string name = "Walk";
void createStatParam() override;

public:
Walk(AbilitySystem *parent);
Walk(GameObject *target);
vector<GameObject *> virtual findTargetForPassive() override;
vector<GameObject *> virtual findTargetForActive() override;
bool virtual canActive(vector<GameObject *> targets) override;
void virtual passive(vector<GameObject *> targets) override;
void virtual active(vector<GameObject *> targets) override;
};

class WalkSeek: public Walk{
protected:
Var3DGraph *data = new Var3DGraph({{0, 0, 0, 0},
                                   {0, 100, 0, 100},
                                   {100, 100, 0, 40},
                                   {0, 0, 100, 10},
                                   {30, 80, 5, 70},
                                   {100, 100, 100, 25}});
public:
WalkSeek(AbilitySystem *parent);
WalkSeek(GameObject *target);
void virtual active(vector<GameObject *> targets) override;
};

class WalkEscape: public Walk{
protected:
Var3DGraph *data = new Var3DGraph({{0, 0, 0, 0},
                                   {0, 100, 0, 100},
                                   {100, 100, 0, 40},
                                   {0, 0, 100, 10},
                                   {30, 80, 5, 70},
                                   {100, 100, 100, 25}});
public:
WalkEscape(AbilitySystem *parent);
WalkEscape(GameObject *target);
void virtual active(vector<GameObject *> targets) override;
};

//--------------------------- Procreate ---------------------------


/*
--------------------------- IDEAS ---------------------------

sleep:

inflicts sleep(target, duration) on self
every tick regens stamina for the animal
in each animals' action:

if (animal has sleep){
  sleep.tick();
} else {
  ...everything else
}

*/

#endif