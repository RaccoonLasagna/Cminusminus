// #include "SFML/Audio.hpp"
// #include "SFML/Graphics.hpp"
// #include "SFML/Network.hpp"
// #include "SFML/System.hpp"
// #include "SFML/Window.hpp"
// #include "SFML/Graphics/Color.hpp"
// #include "SFML/Graphics/RenderStates.hpp"
// #include "SFML/Graphics/RenderWindow.hpp"
// #include "SFML/Window/Event.hpp"
// #include "SFML/Window/Keyboard.hpp"
// #include "SFML/Window/VideoMode.hpp"
// #include "SFML/Window/Window.hpp"
// #include "SFML/Window/WindowStyle.hpp"
// #include "Gui.h"

// using namespace sf;

// int main() {

//   srand(time(NULL));

//   // window
//   Game game;

//   // Game loop
//   while (game.runing() && !game.getEndGAme()) {
//     game.update();
//     game.render();
//   }
  
//   return 0;
// }

#include "Gui.h"
#include <ctime>

using namespace sf;

int main() {
  srand(static_cast<unsigned>(time(0)));
  Gui gui;
  while (gui.runing()) {
    gui.update();
    gui.render();
  }
  return 0;
}