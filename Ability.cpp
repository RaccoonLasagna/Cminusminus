#include "Ability.h"
#include "Affliction.h"
#include "FundamentalSystem.cpp"
#include "FundamentalSystem.h"

#include "StatParam.h"

using namespace std;

void Eat::produceAffliction() { return; }

void Eat::createStatParam() {
  StatParam *aff = new Hunger(getParent()->getParent());
}

vector<GameObject *> Eat::findTargetForPassive() {
  vector<GameObject *> target = {getParent()->getParent()};
  return target;
}

vector<GameObject *> Eat::findTargetForActive() {
  vector<GameObject *> target = {};
  Layer *foodLayer =
      getParent()->getParent()->getParent()->getParent()->getLayer("Food");
  // ฟังก์ชั่นหา object ที่อยู่ห่างจากตัวเองไม่เกิน 1 ช่อง
  return target;
}

bool Eat::canActive(vector<GameObject *> target) {
  // ตรวจสอบว่ารอบตัวใน Layer Food มีอาหารอยู่รอบตัวไหม
  return true;
}

void Eat::passive(vector<GameObject *> target) {
  Hungry *hungry = new Hungry(getParent()->getParent());
  return;
}
void Eat::active(vector<GameObject *> target) {
  // ลบอาหารที่กินไป
  // สร้าง Affliction food ที่จะบวก hunger ให้กับตัวเอง
  Full *full = new Full(getParent()->getParent());
  return;
}