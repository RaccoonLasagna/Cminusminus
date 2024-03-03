// #include "SFML/Graphics/Color.hpp"
// #include <SFML/Graphics/Text.hpp>
// #include "SFML/Graphics/Text.hpp"
// #include "SFML/Graphics/Font.hpp"
// #include "SFML/Graphics/RectangleShape.hpp"
// #include "SFML/Graphics/RenderTarget.hpp"
// #include "SFML/Graphics/RenderWindow.hpp"
// #include "SFML/System/Vector2.hpp"
// #include "SFML/Window/Event.hpp"
// #include "SFML/Window/Keyboard.hpp"
// #include "SFML/Window/VideoMode.hpp"
// #include "SFML/Window/Window.hpp"
// #include "SFML/Window/WindowStyle.hpp"
// #include <vector>

// using namespace sf;
// using namespace std;

// #ifndef GUI_H
// #define GUI_H

// class Game {
// private:
//   // private varible
//   RenderWindow *window;
//   VideoMode videoMode;
//   Event env;

//   // resource
// //   Font fonts;

//   // text
// //   Text uiText;

//   // game logic
//   bool endGAme;
//   unsigned points;
//   int health;
//   float enemySpawnTimer;
//   float enemySpawnTimerMax;
//   int maxEnemies;
//   bool mouseClick;

//   // mouse position
//   Vector2i mousePosition;
//   Vector2f mousePOsView;

//   // Gameobject
//   vector<RectangleShape> enimies;
//   RectangleShape enemy;

//   // private function
//   void initVar();
//   void initFont();
//   void initText();
//   void initWindow();
//   void initEnemies();

// public:
//   // constructor/destructor
//   Game();
//   virtual ~Game();

//   // accessor
//   bool runing();
//   bool getEndGAme();

//   // function
//   void spawnEnemy();

//   void pollEvent();
//   void updateText();
//   void updateMousePosition();
//   void updateEnemies();
//   void update();

//   void renderText(RenderTarget &target);
//   void renderEnemies();
//   void render();
// };

// #endif

#ifndef GUI_H
#define GUI_H

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"

using namespace sf;

class Player {
private:
    RectangleShape shape;
    void initVar();
    void initShape();
public:
  Player();
  ~Player();
  void update();
  void render(RenderTarget *target);
};

class Gui {
private:
  RenderWindow *window;
  VideoMode videoMode;
  Event sfmlEvent;
  Player *player;
  bool endGame;
  void initVarible();
  void initWindow();
  void pollEvent();

public:
  Gui();
  ~Gui();
  bool runing();
  void update();
  void render();
};

#endif