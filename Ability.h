#include "FundamentalSystem.cpp"
#include "FundamentalSystem.h"

using namespace std;

#ifndef ABILITY_H
#define ABILITY_H

class Attack : public Ability {
private:
public:
  int virtual decision(const int, const int, const int) const override;
  bool virtual action() const override;
  bool virtual canAction() override;
};

class Walk : public Ability {
private:
public:
};

class Rest : public Ability {
private:
public:
  int virtual decision(const int, const int, const int) const override;
  bool virtual action() const override;
  bool virtual canAction() override;
};

class Sleep : public Ability {
private:
public:
  int virtual decision(const int, const int, const int) const override;
  bool virtual action() const override;
  bool virtual canAction() override;
};

class Eat : public Ability {
private:
public:
  int virtual decision(const int, const int, const int) const override;
  bool virtual action() const override;
  bool virtual canAction() override;
};

class WalkN : public Walk {
private:
public:
  int virtual decision(const int, const int, const int) const override;
  bool virtual action() const override;
  bool virtual canAction() override;
};

class WalkNE : public Walk {
private:
public:
  int virtual decision(const int, const int, const int) const override;
  bool virtual action() const override;
  bool virtual canAction() override;
};

class WalkE : public Walk {
private:
public:
  int virtual decision(const int, const int, const int) const override;
  bool virtual action() const override;
  bool virtual canAction() override;
};

class WalkSE : public Walk {
private:
public:
  int virtual decision(const int, const int, const int) const override;
  bool virtual action() const override;
  bool virtual canAction() override;
};

class WalkS : public Walk {
private:
public:
  int virtual decision(const int, const int, const int) const override;
  bool virtual action() const override;
  bool virtual canAction() override;
};

class WalkSW : public Walk {
private:
public:
  int virtual decision(const int, const int, const int) const override;
  bool virtual action() const override;
  bool virtual canAction() override;
};

class WalkW : public Walk {
private:
public:
  int virtual decision(const int, const int, const int) const override;
  bool virtual action() const override;
  bool virtual canAction() override;
};

class WalkNW : public Walk {
private:
public:
  int virtual decision(const int, const int, const int) const override;
  bool virtual action() const override;
  bool virtual canAction() override;
};

#endif