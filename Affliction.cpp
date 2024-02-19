#include "Affliction.h"
using namespace std;

//-------------NCDs-------------//
NCDs::NCDs(AfflictionSystem *parent, int duration, string name)
    : Affliction(parent, duration, name) {}

NCDs::~NCDs() {
    if (this->getParent() != nullptr) { // ถ้ามันยังไม่ถูกลบออกจาก parent อยู่
        this->getParent()->removeAffliction(this->getName());
    }
}

bool NCDs::tick() { return true; }

//-------------InfectiousDisease-------------//
InfectiousDisease::InfectiousDisease(AfflictionSystem *parent, int duration,
                                     string name)
    : Affliction(parent, duration, name) {}

InfectiousDisease::~InfectiousDisease() {
    if (this->getParent() != nullptr) { // ถ้ามันยังไม่ถูกลบออกจาก parent อยู่
        this->getParent()->removeAffliction(this->getName());
    }
}

// void InfectiousDisease::spread(GameObject *target) {
//   if (canSpread(target)) {
//     target->getStat()->addAffliction(
//         new InfectiousDisease(target, this->getDuration(), this->getName()));
//         // รอทำ constructor ของ InfectiousDisease
//   }
// }
// ตรงนี้ต้องให้ class ลูกไปทำเอง

//-------------Blind-------------//
Blind::Blind(AfflictionSystem *parent, int duration, string name)
    : NCDs(parent, duration, name) {}

Blind::~Blind() {
    if (this->getParent() != nullptr) { // ถ้ามันยังไม่ถูกลบออกจาก parent อยู่
        this->getParent()->removeAffliction(this->getName());
    }
}

void Blind::action() {
  GameObject *target = this->getParent()->getParent();
  if (target->getStat()->isInStat("Slight")) {
    target->getStat()->getValue("Sight")->setValue(0);
  }
}

//-------------Posion-------------//
Posion::Posion(AfflictionSystem *parent, int duration, string name)
    : InfectiousDisease(parent, duration, name) {}

Posion::~Posion() {
    if (this->getParent() != nullptr) { // ถ้ามันยังไม่ถูกลบออกจาก parent อยู่
        this->getParent()->removeAffliction(this->getName());
    }
}

bool Posion::canSpread(GameObject *target) {
  return true; // ตั้งไว้ก่อนเดี๋ยวมาทำการสุ่มที่หลัง
}

void Posion::action() {
  GameObject *target = this->getParent()->getParent();
  target->getStat()->getValue("HP")->change(-10);
}

void Posion::spread(GameObject *target) {
  if (canSpread(target)) {
    target->getStat()->addAffliction(
        new Posion(target->getStat(), this->getDuration(), this->getName()));
  }
}