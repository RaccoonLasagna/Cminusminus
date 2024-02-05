#include "class.h"
#include <array>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename T, typename R> inline bool checkSubclass(const R *obj) {
  return dynamic_cast<const T *>(obj) != nullptr;
}

/*----------------------------------------------------------*/
// GameObject
string GameObject::repersent() { return GameObject::repersentation; }
void GameObject::decision() { return; }
array<int, 2> GameObject::get_coordinate() { return GameObject::coordinate; }

/*----------------------------------------------------------*/
// LivingThing
bool LivingThing::isAlive() { return LivingThing::alive; }
void LivingThing::die() { LivingThing::alive = false; }
void LivingThing::decision() {
  if (LivingThing::lifetime == 0) {
    LivingThing::die();
  } else {
    --LivingThing::lifetime;
  }
  return;
}

// use same as GameObject::repersent()

/*----------------------------------------------------------*/
// Feild
// everything is same as GameObject

/*----------------------------------------------------------*/
// Land
Land::Land(int x, int y) {
  Land::repersentation = " ";
  Land::coordinate[0] = x;
  Land::coordinate[1] = y;
}

/*----------------------------------------------------------*/
// Plant
Plant::Plant(int x, int y) {
  Plant::repersentation = "P";
  Plant::coordinate[0] = x;
  Plant::coordinate[1] = y;
}
int Plant::eaten() {
  if (Plant::has_fruit) {
    --Plant::fruit_amount;
    if (Plant::fruit_amount == 0) {
      Plant::has_fruit = false;
    }
    return Plant::fruit_hunger_value;
  } else {
    return 0;
  }
}

/*----------------------------------------------------------*/
// Animal
Animal::Animal(int x, int y, string type, int health_point, int hunger_point,
               int stamina, int stamina_regen, int damage, int vision_range,
               int lifetime) {
  Animal::repersentation = type.at(0);
  Animal::coordinate[0] = x;
  Animal::coordinate[1] = y;
  Animal::type = type;
  Animal::health_point = health_point;
  Animal::hunger_point = hunger_point;
  Animal::stamina = stamina;
  Animal::stamina_regen = stamina_regen;
  Animal::damage = damage;
  Animal::vision_range = vision_range;
  Animal::lifetime = lifetime;
}
void Animal::move() { return; }
void Animal::rest() { return; }
void Animal::eat() { return; }
int Animal::eaten() { return 0; }
void Animal::attack() { return; }
void Animal::reproduce() { return; }

// need_to_function will use input from current status of the animal
// point_for_move will use input from current need_to_function of the animal

// these equation function will change depend on the type of animal
void Animal::calculate_need_to_survive() { return; }
void Animal::calculate_need_to_eat() { return; }
void Animal::calculate_need_to_reproduce() { return; }

/* these equation fuction can change if action is wried */
double Animal::calculate_point_for_move() { return 0; }
double Animal::calculate_point_for_rest() { return 0; }
double Animal::calculate_point_for_eat() { return 0; }
double Animal::calculate_point_for_attack() { return 0; }
double Animal::calculate_point_for_reproduce() { return 0; }

void Animal::decision() {
  if (Animal::lifetime == 0) {
    Animal::die();
  } else {
    --Animal::lifetime;
  }

  Animal::calculate_need_to_eat();
  Animal::calculate_need_to_survive();
  Animal::calculate_need_to_reproduce();

  vector<double> desition_value = {
      Animal::calculate_point_for_move(), Animal::calculate_point_for_rest(),
      Animal::calculate_point_for_eat(), Animal::calculate_point_for_attack(),
      Animal::calculate_point_for_reproduce()};
  int max_index = 0;
  for (int i = 1; i < desition_value.size(); ++i) {
    if (desition_value.at(i) > desition_value.at(max_index)) {
      max_index = i;
    }
  }
  switch (max_index) {
  case 0:
    Animal::move();
    break;
  case 1:
    Animal::rest();
    break;
  case 2:
    Animal::eat();
    break;
  case 3:
    Animal::attack();
    break;
  case 4:
    Animal::reproduce();
    break;
  }
  return;
}

/*----------------------------------------------------------*/
// Region
Region::Region(int x, int y) {
  Region::width = x;
  Region::height = y;
}
void Region::update() { return; }
void Region::printmap() { cout << last_repersentation; }
void Region::takerepresent() { return; }
void Region::place(GameObject *object) { return; }

/*----------------------------------------------------------*/