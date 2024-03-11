#include <string>
#include <vector>
#include "Gui.h"

// using namespace sf;

int main() {
    Gui game;
    while (game.isOpen()) {
        game.update();
        
        game.render();
    }
}