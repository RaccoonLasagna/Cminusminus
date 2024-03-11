#include "FundamentalSystem.h"

using namespace std;

#ifndef LIVING_THING_H
#define LIVINGTHING_H

// All animals can WalkSeek, WalkEscape, Attack, Eat,
class Animal : public GameObject {
protected:
public:
    Animal(Layer *parent, int x, int y);
};


class Plant : public GameObject {

protected:
public:
    Plant(Layer *parent, int x, int y);
};


class Corpse : public GameObject {

protected:
    int hunger_yield;
    string name = "Corpse";

public:
    Corpse(Layer *parent, int x, int y, int hunger_yield);
};


class Lion : public Animal {
protected:
    string name = "Lion";
    string diet_type = "Carnivore";

public:
    Lion(Layer *parent, int x, int y);
};


class Bear : public Animal {
protected:
    string name = "Bear";
    string diet_type = "Carnivore";

public:
    Bear(Layer *parent, int x, int y);
};


class Cheetah : public Animal {
protected:
    string name = "Cheetah";
    string diet_type = "Carnivore";

public:
    Cheetah(Layer *parent, int x, int y);
};


class Giant_Snake : public Animal {
protected:
    string name = "Giant Snake";
    string diet_type = "Carnivore";

public:
    Giant_Snake(Layer *parent, int x, int y);
};


class Rabbit : public Animal {
protected:
    string name = "Rabbit";
    string diet_type = "Herbivore";

public:
    Rabbit(Layer *parent, int x, int y);
};


class Goat : public Animal {
protected:
    string name = "Goat";
    string diet_type = "Herbivore";

public:
    Goat(Layer *parent, int x, int y);
};


class Deer : public Animal {
protected:
    string name = "Deer";
    string diet_type = "Herbivore";

public:
    Deer(Layer *parent, int x, int y);
};


class Chicken : public Animal {
protected:
    string name = "Chicken";
    string diet_type = "Omnivore";

public:
    Chicken(Layer *parent, int x, int y);
};


class Poison_Frog : public Animal {
protected:
    string name = "Poison Frog";
    string diet_type = "Omnivore";

public:
    Poison_Frog(Layer *parent, int x, int y);
};


class EarthWorm : public Animal {
protected:
    string name = "EarthWorm";

public:
    EarthWorm(Layer *parent, int x, int y);
};


class Bush : public Plant {
protected:
    string name = "Bush";
    
public:
    Bush(Layer *parent, int x, int y);
};


class Apple_tree : public Plant {
protected:
    string name = "Apple tree";
    
public:
    Apple_tree(Layer *parent, int x, int y);
};


class Mushroom : public Plant {
protected:
    int Health,hunger_yield;
    string name = "Mushroom";
    
public:
    Mushroom(Layer *parent, int x, int y);
};

#endif
//