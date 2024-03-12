// #include "Ability.h"
// #include "Ability.cpp"
// #include "Affliction.h"
// #include "Affliction.cpp"
// #include "FundamentalSystem.cpp"
// #include "FundamentalSystem.h"
// #include "StatParam.h"
// #include "StatParam.cpp"
// #include "LivingThing.h"

// using namespace std;


// Animal::Animal(Layer *parent, int x, int y) : GameObject(parent, x, y) {
//     // abilities all aniimals can do
//     new WalkSeek(this);
//     new WalkEscape(this);
//     new Mate(this);
//     // mate cooldown on spawn
//     new MateCooldown(this, 200);
// }

// Plant::Plant(Layer *parent, int x, int y) : GameObject(parent, x, y) {}

// Corpse::Corpse(Layer *parent, int x, int y) : GameObject(parent, x, y) {
//     // Make Corpses have 3 eat times and make it single eat
//     new SingleEat(this);
//     new EatTimes(this);
//     new EatTimes(this);
//     new EatTimes(this);
//     // Corpses rot
//     new Rot(this);
// }

// Deer::Deer(Layer *parent, int x, int y) : Animal(parent, x, y) {
//     // All animals need these stats: 
//     // Health, Sight, Speed
//     new Health(this, nullptr, 100);
//     new Sight(this, nullptr, 10);
//     new Speed(this, nullptr, 3);

//     // created by abilities: Hunger 
//     // Deers can eat
//     new Eat(this, 120, true, false, false);
// }

// Bush::Bush(Layer *parent, int x, int y) : Plant(parent, x, y) {
//     // Bushes get SingleEat
//     new SingleEat(this);
//     new EatTimes(this);
// }

// Mushroom::Mushroom(Layer *parent, int x, int y) : Plant(parent, x, y) {
//     // Mushroom get SingleEat
//     new SingleEat(this);
//     new EatTimes(this);
// }

// Apple_tree::Apple_tree(Layer *parent, int x, int y) : Plant(parent, x, y) {
//     // Can regen EatTimes with Fruition, get 3 EatTimes when created
//     new Fruition(this);
//     new EatTimes(this);
//     new EatTimes(this);
//     new EatTimes(this);
// }

// EarthWorm::EarthWorm(Layer *parent, int x, int y) : GameObject(parent, x, y) {
//     //can only walk, randomly
//     new WalkSeek(this);
// }