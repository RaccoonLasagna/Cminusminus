// #include "Gui.h"
// #include "SFML/Graphics/Color.hpp"
// #include "SFML/Graphics/RenderTarget.hpp"
// #include "SFML/Graphics/RenderTexture.hpp"
// #include "SFML/Graphics/RenderWindow.hpp"
// #include "SFML/System/Vector2.hpp"
// #include "SFML/Window/Event.hpp"
// #include "SFML/Window/Keyboard.hpp"
// #include "SFML/Window/Mouse.hpp"
// #include "SFML/Window/VideoMode.hpp"
// #include "SFML/Window/Window.hpp"
// #include "SFML/Window/WindowStyle.hpp"
// #include <iostream>
// #include <random>
// #include <sstream>

// using namespace sf;
// using namespace std;

// void Game::initVar() {
//   this->window = nullptr;

//   // game logic
//   this->points = 0;
//   this->enemySpawnTimerMax = 10.f;
//   this->enemySpawnTimer = enemySpawnTimerMax;
//   this->maxEnemies = 10;
//   this->mouseClick = false;
//   this->health = 10;
//   this->endGAme = false;
// }

// void Game::initFont() {
//   //   if (!fonts.loadFromFile("Font/f.ttf")) {
//   //     cout << "Failed to download font" << endl;
//   //   }
// }

// void Game::initText() {
//   //   this->uiText.setFont(this->fonts);
// //   this->uiText.setCharacterSize(12);
// //   this->uiText.setFillColor(Color(255, 255, 255, 255));
// //   this->uiText.setString("NONE");
// }

// void Game::initWindow() {
//   this->videoMode.height = 600;
//   this->videoMode.width = 800;
//   this->window = new RenderWindow(VideoMode(800, 600), "My frist game",
//                                   Style::Titlebar | Style::Close);
//   this->window->setFramerateLimit(60);
// }

// void Game::initEnemies() {
//   this->enemy.setPosition(10.f, 10.f);
//   this->enemy.setSize(Vector2f(100.f, 100.f));
//   this->enemy.setFillColor(Color::Cyan);
//   //   this->enemy.setOutlineColor(Color::Green);
//   //   this->enemy.setOutlineThickness(1.f);
// }

// Game::Game() {
//   this->initVar();
//   this->initWindow();
//   this->initEnemies();
//   this->initFont();
//   this->initText();
// }

// Game::~Game() {
//   delete this->window;
//   cout << "Point: " << points;
// }

// bool Game::getEndGAme() { return endGAme; }
// bool Game::runing() { return this->window->isOpen(); }

// void Game::spawnEnemy() {
//   this->enemy.setPosition(
//       static_cast<float>(
//           rand() % (int)(this->window->getSize().x -
//           this->enemy.getSize().x)),
//       (float)/*(rand() % (int)(this->window->getSize().y -
//                              this->enemy.getSize().y)));*/
//       0.f);

//   int types = rand() % 5;
//   switch (types) {
//   case 0:
//     this->enemy.setFillColor(Color::Magenta);
//     this->enemy.setSize(Vector2f(10.f, 10.f));
//     break;
//   case 1:
//     this->enemy.setFillColor(Color::Blue);
//     this->enemy.setSize(Vector2f(30.f, 30.f));
//     break;
//   case 2:
//     this->enemy.setFillColor(Color::Cyan);
//     this->enemy.setSize(Vector2f(50.f, 50.f));
//     break;
//   case 3:
//     this->enemy.setFillColor(Color::Red);
//     this->enemy.setSize(Vector2f(70.f, 70.f));
//     break;
//   case 4:
//     this->enemy.setFillColor(Color::Green);
//     this->enemy.setSize(Vector2f(100.f, 100.f));
//     break;
//   default:
//     this->enemy.setFillColor(Color::Yellow);
//     this->enemy.setSize(Vector2f(100.f, 100.f));
//     break;
//   }

//   enimies.push_back(enemy);
// }

// void Game::pollEvent() {
//   while (this->window->pollEvent(this->env)) {
//     switch (this->env.type) {
//     case Event::Closed:
//       this->window->close();
//       break;
//     case Event::KeyPressed:
//       if (this->env.key.code == Keyboard::Escape) {
//         this->window->close();
//       }
//       break;
//     default:
//       break;
//     }
//   }
// }

// void Game::updateText() {
//   stringstream ss;
//   ss << "Points: " << this->points << endl << "Health: " << this->health;
// //   this->uiText.setString(ss.str());
// }

// void Game::updateMousePosition() {
//   this->mousePosition = Mouse::getPosition(*this->window);
//   this->mousePOsView = this->window->mapPixelToCoords(mousePosition);
// }

// void Game::updateEnemies() {
//   if (enimies.size() < maxEnemies) {
//     if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
//       this->spawnEnemy();
//       this->enemySpawnTimer = 0.f;
//     } else {
//       this->enemySpawnTimer += 1.f;
//     }
//   }
//   for (int i = 0; i < enimies.size(); i++) {
//     enimies[i].move(0.f, 1.f);
//     if (enimies[i].getPosition().y > this->window->getSize().y) {
//       enimies.erase(enimies.begin() + i);
//       health -= 1;
//       cout << "Health: " << health << endl;
//     }
//   }

//   if (Mouse::isButtonPressed(Mouse::Left)) {
//     if (this->mouseClick == false) {
//       mouseClick = true;
//       bool deleted = false;
//       for (int i = 0; i < enimies.size() && !deleted; i++) {
//         if (this->enimies[i].getGlobalBounds().contains(this->mousePOsView))
//         {
//           deleted = true;
//           if (enimies[i].getFillColor() == Color::Magenta) {
//             points += 10.f;
//           } else if (enimies[i].getFillColor() == Color::Blue) {
//             points += 7.f;
//           } else if (enimies[i].getFillColor() == Color::Cyan) {
//             points += 5.f;
//           } else if (enimies[i].getFillColor() == Color::Red) {
//             points += 3.f;
//           } else if (enimies[i].getFillColor() == Color::Green) {
//             points += 1.f;
//           }
//           enimies.erase(enimies.begin() + i);
//           cout << "Points: " << points << endl;
//         }
//       }
//     }
//   } else {
//     mouseClick = false;
//   }
// }

// void Game::update() {
//   this->pollEvent();
//   if (!endGAme) {
//     this->updateMousePosition();
//     this->updateEnemies();
//     this->updateText();
//   }
//   if (health <= 0) {
//     endGAme = true;
//   }
// }

// void Game::renderEnemies() {
//   for (auto &e : enimies) {
//     this->window->draw(e);
//   }
// }

// // void Game::renderText(RenderTarget &target) { target.draw(uiText); }

// void Game::render() {
//   this->window->clear(Color(0, 0, 0, 255));

//   this->renderEnemies();

// //   this->renderText(*this->window);

//   this->window->display();
// }

#include "Gui.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/WindowStyle.hpp"

using namespace sf;

void Gui::initWindow() {
  videoMode = VideoMode(800, 600);
  window = new RenderWindow(videoMode, "Game2", Style::Close | Style::Titlebar);
}

void Gui::initVarible() { 
    endGame = false;
    player = new Player;
 }

void Gui::pollEvent() {
  while (window->pollEvent(sfmlEvent)) {
    switch (sfmlEvent.type) {
    case Event::Closed:
      window->close();
      break;
    case Event::KeyPressed:
      if (sfmlEvent.key.code == Keyboard::Escape) {
        window->close();
      }
      break;
    default:
      break;
    }
  }
}

Gui::Gui() {
  initVarible();
  initWindow();
}

Gui::~Gui() { delete window; }

bool Gui::runing() { return window->isOpen(); }

void Gui::update() {
    pollEvent();
}

void Gui::render() {
    window->clear();
    player->render(window);
    window->display();
}

void Player::initVar() {

}

void Player::initShape() {
    shape.setFillColor(Color::Green);
    shape.setSize(Vector2f(100.f, 100.f));
}

Player::Player() {
    initVar();
    initShape();
}

Player::~Player() {

}

void Player::update() {

}

void Player::render(RenderTarget *target) {
    target->draw(shape);
}