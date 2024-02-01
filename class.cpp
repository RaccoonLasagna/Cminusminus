#include "class.h"
#include <array>
#include <iostream>
#include <string>

using namespace std;

/*----------------------------------------------------------*/
// GameObject
GameObject::virtual string repersent() { return GameObject::repersentation; }
GameObject::virtual void decition() { return; }
GameObject::array<int, 2> get_coordinate() { return GameObject::coordinate; }

/*----------------------------------------------------------*/
// LivingThing
LivingThing::virtual bool isAlive() { return LivingThing::alive; }
LivingThing::virtual void die() { LivingThing::alive = false; }
LivingThing::void dicition() override {
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
Land::explicit Land(int x, int y) {
  Land::repersentation = " ";
  Land::coordinate[0] = x;
  Land::coordinate[1] = y;
}

/*----------------------------------------------------------*/
// Plant
Plant::explicit Plant(int x, int y) {
  Plant::repersentation = "P";
  Plant::coordinate[0] = x;
  Plant::coordinate[1] = y;
}
Plant::int eatend() {
  if (Plant::hasfruit) {
    --Plant::fruitamount;
    if (Plant::fruitamount == 0) {
      Plant::hasfruit = false;
    }
    return Plant::fruithungervalue;
  } else {
    return 0;
  }
}

/*----------------------------------------------------------*/
// Animal
Animal::explicit Animal(int x, int y, string type, int health_point,
                        int hunger_point, int stamina, int stamina_regen,
                        int damage, int vision_range, int lifetime) {
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
Animal::void move() { return; }
Animal::void rest() { return; }
Animal::void eat() { return; }
Animal::int eaten() { return 0; }
Animal::void attack() { return; }
Animal::void reproduce() { return; }
Animal::void dicition() override {
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
Region::explicit Region(int x, int y) {
  Region::width = x;
  Region::height = y;
}
Region::void update() {
  for (auto &i : Region::field) {
    for (auto &j : i) {
      j.update();
    }
  }
  return;
}
Region::void update() { return; }
Region::printmap() { cout << lastrepersentation; }
Region::void takerepresent() { return; }
Region::void place(GameObject *object) { return; }

/*----------------------------------------------------------*/