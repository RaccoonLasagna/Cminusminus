#include <array>
#include <string>
using namespace std;

#ifndef CLASS_H
#define CLASS_H

class GameObject {
private:
  string repersentation;
  array<int, 2> coordinate;

public:
  virtual string repersent();
  virtual void decition();
  array<int, 2> get_coordinate();
};

class LivingThing : private GameObject {
private:
  int health_point, lifetime;
  bool alive;

public:
  virtual bool isAlive();
  virtual void die();
};

class Feild : private GameObject {};

class Land : private Feild {
private:
public:
  explicit Land(int x, int y);
};

class Plant : private LivingThing {
private:
  bool hasfruit;
  int fruitamount, fruithungervalue;

public:
  explicit Plant(int x, int y);
  int eaten();
  virtual void reproduce();
};

class Animal : private LivingThing {
private:
  string type;
  int health_point, hungerpoint, stamina, stamina_regen, damage, vision_range,
      lifetime;
  virtual void move();
  virtual void rest();
  virtual void eat();
  virtual int eaten();
  virtual void attack();
  virtual void reproduce();

public:
  explicit Animal(int x, int y, string type, int health_point, int hunger_point,
                  int stamina, int stamina_regen, int damage, int vision_range,
                  int lifetime);
  void decition() override;
};

class Region : private Feild {
private:
int width, height;
  array<array<GameObject *, width>, height> map;
  string lastrepersentation;

public:
  explicit Region(int width, int height);
  void update();
  void printmap();
  void takerepresent();
  void place(GameObject *object);
};

class Herbivore : private Animal {
  virtual void eat();
};

class Carnivore : private Animal {
  virtual void eat();
};

class Omnivore : private Animal {
  virtual void eat();
};

#endif