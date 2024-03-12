// #include "FundamentalSystem.h"

// using namespace std;

// #ifndef ABILITY_H
// #define ABILITY_H

// // ===================================== LIVING THINGS =====================================

// // ===================================== ANIMALS =====================================

// //--------------------------- Eat ---------------------------

// class Eat : public Ability
// {
// protected:
//   string name = "Eat";
//   int max_hunger;
//   bool eatPlants, eatEarthworms, eatAnimals;
//   Var3DGraph data = Var3DGraph({{0, 0, 0, 0},
//                                 {0, 100, 0, 100},
//                                 {100, 100, 0, 40},
//                                 {0, 0, 100, 10},
//                                 {30, 80, 5, 70},
//                                 {100, 100, 100, 25},
//                                 {50, 10, 100, 10}}); // เดี๋ยวค่อยคิดใหม่
//   void createStatParam() override;

// public:
//   Eat(AbilitySystem *parent, int max_hunger, bool eatPlants, bool eatEarthworms, bool eatAnimals);
//   Eat(GameObject *target, int max_hunger, bool eatPlants, bool eatEarthworms, bool eatAnimals);
//   vector<GameObject *> virtual findTargetForPassive() override;
//   vector<GameObject *> virtual findTargetForActive() override;
//   bool virtual canActive(vector<GameObject *> targets) override;
//   void virtual passive(vector<GameObject *> targets) override;
//   void virtual active(vector<GameObject *> targets) override;
// };

// //--------------------------- Attack ---------------------------

// class Attack : public Ability
// {
// protected:
//   string name = "Attack";
//   int atk_amount;
//   Var3DGraph data = Var3DGraph({{0, 0, 0, 0},
//                                 {0, 100, 0, 100},
//                                 {100, 100, 0, 40},
//                                 {0, 0, 100, 10},
//                                 {30, 80, 5, 70},
//                                 {100, 100, 100, 25}}); // remember to change pls
//   void createStatParam() override;

// public:
//   Attack(AbilitySystem *parent, int atk_amount);
//   Attack(GameObject *target, int atk_amount);
//   vector<GameObject *> virtual findTargetForPassive() override;
//   vector<GameObject *> virtual findTargetForActive() override;
//   bool virtual canActive(vector<GameObject *> targets) override;
//   void virtual passive(vector<GameObject *> targets) override;
//   void virtual active(vector<GameObject *> targets) override;
// };

// //--------------------------- Walk ---------------------------

// class Walk : public Ability
// {
// protected:
//   string name = "Walk";
//   void createStatParam() override;

// public:
//   Walk(AbilitySystem *parent);
//   Walk(GameObject *target);
//   vector<GameObject *> virtual findTargetForPassive() override;
//   vector<GameObject *> virtual findTargetForActive() override;
//   bool virtual canActive(vector<GameObject *> targets) override;
//   void virtual passive(vector<GameObject *> targets) override;
//   void virtual active(vector<GameObject *> targets) = 0;
// };

// class WalkSeek : public Walk
// {
// protected:
//   Var3DGraph data = Var3DGraph({{0, 0, 0, 0},
//                                 {0, 100, 0, 100},
//                                 {100, 100, 0, 40},
//                                 {0, 0, 100, 10},
//                                 {30, 80, 5, 70},
//                                 {100, 100, 100, 25}});

// public:
//   WalkSeek(AbilitySystem *parent);
//   WalkSeek(GameObject *target);
//   void virtual active(vector<GameObject *> targets) override;
// };

// class WalkEscape : public Walk
// {
// protected:
//   Var3DGraph data = Var3DGraph({{0, 0, 0, 0},
//                                 {0, 100, 0, 100},
//                                 {100, 100, 0, 40},
//                                 {0, 0, 100, 10},
//                                 {30, 80, 5, 70},
//                                 {100, 100, 100, 25}});

// public:
//   WalkEscape(AbilitySystem *parent);
//   WalkEscape(GameObject *target);
//   void virtual active(vector<GameObject *> targets) override;
// };

// //--------------------------- Mate ---------------------------

// class Mate : public Ability
// {
// protected:
//   string name = "Mate";
//   void createStatParam() override;

// public:
//   Mate(AbilitySystem *parent);
//   Mate(GameObject *target);
//   vector<GameObject *> virtual findTargetForPassive() override;
//   vector<GameObject *> virtual findTargetForActive() override;
//   bool virtual canActive(vector<GameObject *> targets) override;
//   void virtual passive(vector<GameObject *> targets) override;
//   void virtual active(vector<GameObject *> targets) override;
// };

// // ===================================== CORPSES =====================================

// //--------------------------- Rot ---------------------------

// class Rot : public Ability
// {
// protected:
//   string name = "Mate";
//   int rot_timer = 0;
//   void createStatParam() override;

// public:
//   Rot(AbilitySystem *parent);
//   Rot(GameObject *target);
//   vector<GameObject *> virtual findTargetForPassive() override;
//   vector<GameObject *> virtual findTargetForActive() override;
//   bool virtual canActive(vector<GameObject *> targets) override;
//   void virtual passive(vector<GameObject *> targets) override;
//   void virtual active(vector<GameObject *> targets) override;
// };

// // ===================================== PLANTS =====================================

// // --------------------------- Fruition ---------------------------

// class Fruition : public Ability
// {
// protected:
//   string name = "Fruition";
//   void createStatParam() override;

// public:
//   Fruition(AbilitySystem *parent);
//   Fruition(GameObject *target);
//   vector<GameObject *> virtual findTargetForPassive() override;
//   vector<GameObject *> virtual findTargetForActive() override;
//   bool virtual canActive(vector<GameObject *> targets) override;
//   void virtual passive(vector<GameObject *> targets) override;
//   void virtual active(vector<GameObject *> targets) override;
// };

// class SingleEat : public Ability
// {
// protected:
//   string name = "SingleEat";
//   void createStatParam() override;

// public:
//   SingleEat(AbilitySystem *parent);
//   SingleEat(GameObject *target);
//   vector<GameObject *> virtual findTargetForPassive() override;
//   vector<GameObject *> virtual findTargetForActive() override;
//   bool virtual canActive(vector<GameObject *> targets) override;
//   void virtual passive(vector<GameObject *> targets) override;
//   void virtual active(vector<GameObject *> targets) override;
// };

// #endif

// /*
// new abilities
// Living Things:
//     , die()

// for Corpses:
//     ,rot(when animal dies,corpse rot, enable for fungis and earthworms to spawn)
// */