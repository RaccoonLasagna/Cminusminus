#include "FundamentalSystem.h"

using namespace std;

#ifndef ABILITY_H
#define ABILITY_H

class PlayTurn : public Ability {
protected:
  string name = "PlayTurn";
  Var3DGraph *data =
      new Var3DGraph({{0, 0, 0, 0}, {100, 100, 100, 0}}); // เดี๋ยวค่อยคิดใหม่
  void produceAffliction() override;
  void createStatParam() override;

public:
  PlayTurn(AbilitySystem *parent);
  PlayTurn(GameObject *target);
  vector<GameObject *> virtual findTargetForPassive() override;
  vector<GameObject *> virtual findTargetForActive() override;
  bool virtual canActive(vector<GameObject *>) override;
  void virtual passive(vector<GameObject *>) override;
  void virtual active(vector<GameObject *>) override;
};

class Eat : public Ability {
protected:
  string name = "Eat";
  Var3DGraph *data = new Var3DGraph({{0, 0, 0, 0},
                                     {0, 100, 0, 100},
                                     {100, 100, 0, 40},
                                     {0, 0, 100, 10},
                                     {30, 80, 5, 70},
                                     {100, 100, 100, 25},
                                     {50, 10, 100, 10}}); // เดี๋ยวค่อยคิดใหม่
  void produceAffliction() override;
  void createStatParam() override;

public:
  Eat(AbilitySystem *parent);
  Eat(GameObject *target);
  vector<GameObject *> virtual findTargetForPassive() override;
  vector<GameObject *> virtual findTargetForActive() override;
  bool virtual canActive(vector<GameObject *>) override;
  void virtual passive(vector<GameObject *>) override;
  void virtual active(vector<GameObject *>) override;
};

class Rest : public Ability {
protected:
  string name = "Rest";
  Var3DGraph *data = new Var3DGraph({{0, 0, 0, 0},
                                     {0, 100, 0, 100},
                                     {100, 100, 0, 40},
                                     {0, 0, 100, 10},
                                     {30, 80, 5, 70},
                                     {100, 100, 100, 25}}); // เดี๋ยวค่อยคิดใหม่
  void produceAffliction() override;
  void createStatParam() override;

public:
  Rest(AbilitySystem *parent);
  Rest(GameObject *target);
  vector<GameObject *> virtual findTargetForPassive() override;
  vector<GameObject *> virtual findTargetForActive() override;
  bool virtual canActive(vector<GameObject *>) override;
  void virtual passive(vector<GameObject *>) override;
  void virtual active(vector<GameObject *>) override;
};

class Attack: public Ability{
protected:
string name = "Attack";
Var3DGraph *data = new Var3DGraph({{0, 0, 0, 0},
                                   {0, 100, 0, 100},
                                   {100, 100, 0, 40},
                                   {0, 0, 100, 10},
                                   {30, 80, 5, 70},
                                   {100, 100, 100, 25}}); // remember to change pls
void produceAffliction() override;
void createStatParam() override;

public:
Attack(AbilitySystem *parent);
Attack(GameObject *target);
vector<GameObject *> virtual findTargetForPassive() override;
vector<GameObject *> virtual findTargetForActive() override;
bool virtual canActive(vector<GameObject *>) override;
void virtual passive(vector<GameObject *>) override;
void virtual active(vector<GameObject *>) override;
};

#endif