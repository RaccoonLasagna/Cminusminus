#include "FundamentalSystem.h"

using namespace std;

#ifndef ABILITY_H
#define ABILITY_H

class Eat : public Ability {
protected:
  string name = "Eat";
  Var3DGraph *data = new Var3DGraph({{0, 0, 0, 0},
                                     {0, 100, 0, 100},
                                     {100, 100, 0, 40},
                                     {0, 0, 100, 10},
                                     {30, 80, 5, 70},
                                     {100, 100, 100, 25}});
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

#endif