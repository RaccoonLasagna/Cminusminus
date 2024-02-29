#include "FundamentalSystem.h"

int main() {
    LayerSystem Game = LayerSystem(10,10);
    Layer Map = Layer();
    GameObject player = GameObject(&Map);
    Ability health = Ability(&player);
    return 0;
}