#include <iostream>
#include <iomanip>
#include <array>
#include <string>
#include <random>
#include <time.h>
using namespace std;

const int LANDWIDTH = 50;
const int LANDHEIGHT = 30;

class GameObject
{
private:
    int x;
    int y;
    bool alive;

public:
    GameObject(int x, int y) : x(x), y(y), alive(true) {}
    virtual string represent();
    virtual void update();
    int get_x()
    {
        return x;
    }
    int get_y()
    {
        return y;
    }
    bool is_alive(){
        return alive;
    }
    void die(){
        alive = false;
    }
};

class Land : public GameObject
{
private:
    int time_to_tree;

public:
    Land(int x, int y) : GameObject(x, y){};
    string represent()
    {
        return "  ";
    }
};

class Animal : public GameObject
{
private:
    string type;
    int health_point;
    int hunger_point;
    int stamina;
    int stamina_regen;
    int damage;
    int vision_range;

public:
    Animal(int x, int y, string type) : GameObject(x, y), type(type) {}
    string represent()
    {
        return "\U0001F42F";
    }
};

class Plant : public GameObject
{
private:
    bool has_fruit;
    int fruit_amount;
    int fruit_hunger_value;
    int lifetime;

public:
    Plant(int x, int y) : GameObject(x, y), lifetime(15) {}
    string represent()
    {
        return "🌳";
    }
    void update()
    {
        if (lifetime <= 0)
        {
            die();
        }
        else
        {
            lifetime -= 0;
        }
    }
};

class Region
{
private:
    array<array<GameObject *, LANDWIDTH>, LANDHEIGHT> landmass;

public:
    Region()
    {
        for (size_t y = 0; y < LANDHEIGHT; y++)
        {
            for (size_t x = 0; x < LANDWIDTH; x++)
            {
                place_gameobj(new Land(x, y));
            }
        }
    }
    void print_land()
    {
        string output{""};
        for (array<GameObject *, LANDWIDTH> row : landmass)
        {
            for (GameObject *obj : row)
            {
                output += obj->represent();
            }
            output += "|\n";
        }
        cout << output << string(LANDWIDTH * 2 + 1, '_') << endl;
    }
    void place_gameobj(GameObject *obj)
    {
        landmass[obj->get_y()][obj->get_x()] = obj;
    }
    void step(){
        for (size_t y = 0; y < LANDHEIGHT; y++)
        {
            for (size_t x = 0; x < LANDWIDTH; x++)
            {
                if (~landmass[y][x]->is_alive()){
                    place_gameobj(new Land(x, y));
                }
            }
        }
    }
    void forest()
    {
        for (size_t y = 0; y < LANDHEIGHT; y++)
        {
            for (size_t x = 0; x < LANDWIDTH; x++)
            {
                int gacha = (rand() % 100) + 1;
                if (gacha <= 5)
                {
                    place_gameobj(new Plant(x, y));
                }
            }
        }
    }
    void extinction()
    {
        for (size_t y = 0; y < LANDHEIGHT; y++)
        {
            for (size_t x = 0; x < LANDWIDTH; x++)
            {
                place_gameobj(new Land(x, y));
            }
        }
    }
};

void wait()
{
    string x;
    getline(cin, x);
}

int main()
{
    srand(time(NULL));
    Region x;
    x.print_land();

    wait();

    x.place_gameobj(new Plant(25, 15));
    x.print_land();

    wait();

    x.forest();
    x.print_land();

    wait();

    x.extinction();
    x.print_land();

    wait();
    return 0;
}