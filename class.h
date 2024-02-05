#include <array>
#include <string>
#include <vector>
using namespace std;

#ifndef CLASS_H
#define CLASS_H

template <typename T, typename R> inline bool checkSubclass(const R *obj);
// checkSubclass<Herbivore>(animalPtr) <= use like this.

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
  double need_to_survive, need_to_eat, need_to_reproduce;
  virtual void calculate_need_to_survive();
  virtual void calculate_need_to_eat();
  virtual void calculate_need_to_reproduce();

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
  virtual double calculate_point_for_reproduce();

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

  virtual void calculate_need_to_survive() override;
  virtual void calculate_need_to_eat() override;
  virtual void calculate_need_to_reproduce() override;

  virtual double calculate_point_for_move();
  virtual double calculate_point_for_rest();
  virtual double calculate_point_for_eat();
  virtual double calculate_point_for_attack();
  virtual double calculate_point_for_reproduce();
  
  virtual void move();
  virtual void rest();
  virtual void eat();
  virtual void attack();
  virtual void reproduce();

  virtual int eaten();


public:
  explicit Animal(int x, int y, string type, int health_point, int hunger_point,
                  int stamina, int stamina_regen, int damage, int vision_range,
                  int lifetime);
  void decision() override;
};

class Region : public Feild {
private:
  int width, height;
  vector<vector<GameObject *>> map;
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

class Scarvenger : public Omnivore {
  virtual void eat();
};

/*--------------------------------------------------------------------------*/

#endif