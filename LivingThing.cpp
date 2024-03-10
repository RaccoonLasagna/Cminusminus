#include "LivingThing.h"

TestLivingThing::TestLivingThing(Layer *parent, int x, int y): GameObject(parent, x, y) {
    setRepresent("L");
}

int TestLivingThing::getSur() {
    return 0;
}

int TestLivingThing::getEnv() {
    return 0;
}

int TestLivingThing::getRepo() {
    return 0;
}

void TestLivingThing::update() {
    cout << "Test" << endl;
}