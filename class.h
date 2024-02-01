#include <array>
#include <string>
#include <vector>
using namespace std;

#ifndef CLASS_H
#define CLASS_H

class GameObject {
private:
public:
  string repersentation;
  array<int, 2> coordinate;
  virtual string repersent();
  virtual void decision();
  array<int, 2> get_coordinate();
};

class LivingThing : public GameObject {
private:
  int health_point, lifetime;
  bool alive;
public:
  virtual bool isAlive();
  virtual void die();
  virtual void decision() override;
};

class Feild : public GameObject {};

class Land : public Feild {
private:
public:
  explicit Land(int x, int y);
};

class Plant : public LivingThing {
private:
  bool has_fruit;
  int fruit_amount, fruit_hunger_value;

public:
  explicit Plant(int x, int y);
  int eaten();
  virtual void reproduce();
};

class Animal : public LivingThing {
private:
  string type;
  int health_point, hunger_point, stamina, stamina_regen, damage, vision_range,
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
  void decision() override;
};

class Region : public Feild {
private:
int width, height;
  vector<vector<GameObject*>> map;
  string last_repersentation;

public:
  explicit Region(int width, int height);
  void update();
  void printmap();
  void takerepresent();
  void place(GameObject *object);
};

class Herbivore : public Animal {
  virtual void eat();
};

class Carnivore : public Animal {
  virtual void eat();
};

class Omnivore : public Animal {
  virtual void eat();
};

#endif