#include "class.h"
#include <array>
#include <iostream>
#include <string>

using namespace std;

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
void Animal::decision() {
  if (Animal::lifetime == 0) {
    Animal::die();
  } else {
    --Animal::lifetime;
    // and so on
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