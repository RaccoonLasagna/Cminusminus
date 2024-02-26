// class StatParameter { // เป็น paramiter ที่ไม่สามารถติดลบได้
// private:
//   string name; // มี Name เพราะว่าเวลาที่เราต้องการเรียกใช้ใน stat
//                // เราต้องการเรียกแต่ละพารามิเตอร์ด้วยชื่อ
//   int value;
//   StatusSystem *parent;

// public: // ยังไม่ได้ทำ destructor เลย
//   explicit StatParameter(StatusSystem *parent, string name, int value);
//   //-------------------------//
//   void virtual change(const int i);
//   bool isZero();
//   void virtual actionZero();
//   void virtual passiveAction();
//   bool virtual update();
//   //-------------------------//
//   inline void setValue(const int i);
//   inline void setName(const string n);
//   //-------------------------//
//   inline int &getValue(); // ทำไมต้องเป็น ref
//   inline string getName();
//   inline StatusSystem *getParent();
// };

// class StatParameterMax : public StatParameter { //
// ติดลบไม่ได้และมีค่าสไม่เกินค่าที่กำหนด private:
//   int maxValue;

// public:
//   explicit StatParameterMax(StatusSystem *parent, string name, int value,
//                             int maxValue);
//   //-------------------------//
//   void change(const int) override; // value อยู่ในช่วง 0 ถึง max
//   bool isMax();
//   void virtual actionMax();
//   bool virtual update() override;
//   //-------------------------//
//   inline int getMax();
//   inline void setMax(const int i);
// };

// class Ability { // แล้วไอตัวนี้มันจะถูกทำลายตอน stat ถูกทำลาย
// private:
//   string name;
//   AbilitySystem *parent;

// public:
//   explicit Ability(AbilitySystem *parent, string name);
//   explicit Ability(
//       AbilitySystem *parent, string name,
//       vector<string> statGiven); // หน้าเพิ่ม constructor อีกซักตัวให้รับอินพุตเป็น
//                                  // vector<statparamiteer*>
//   ~Ability(); // ยังไม่รู้ว่าต้องทำไง
//   //-------------------------//
//   int virtual decision(const int, const int, const int)
//       const = 0; // ออกมาเป็นค่า desition point ให้สัตว์เลือก ability
//                  // ที่แต้มมากที่สุดไปใช้งานในเทิร์นนั้นๆ
//   bool virtual action()
//       const = 0; // ตีโดนไหม ถ้าถามว่าทำไม action มาอยู่ในนี้แล้วไม่มีฟังก์ชั่น update
//                  // เพราะว่าเราจะสั่งให้คำสั่ง action ทำงานจากตัวของสัตว์ ไม่ใช่จากตัวของ
//                  // ability เอง
//   bool virtual canAction() = 0; // เช็คเงื่อนไขว่าสามารถทำ action นั้นๆได้ไหม
//   //-------------------------//
//   inline void setName(const string n);
//   //-------------------------//
//   inline string getName();
//   inline AbilitySystem *getParent();
// };

// class Affliction { // แล้วไอตัวนี้มันจะถูกทำลายตอน stat ถูกทำลาย
// private:
//   string name;
//   int duration, passedTime = 0;
//   AfflictionSystem *parent;

//   bool virtual tick(); // ถ้า duration ลดลงแล้วเป็น 0 ให้ return false
//                        // และฟังก์ชั่นนนี้สามรถใช้เพื่อลดเวลาการติดได้ด้วยโดยการให้มัน tick
//                        // หลายๆรอบ
//   void virtual action() = 0; // ทำอะไรเมื่อมีโรคโดยที่มันจะไป acess
//                              // ค่าต่างๆของสัตว์และส่งผลก่อนที่สัตว์จะได้ตัดสินใจ

// public:
//   explicit Affliction(AfflictionSystem *parent, int duration, string name);
//   ~Affliction(); // ยังไม่รู้ว่าจะทำไง
//   //-------------------------//
//   bool refresh();
//   void
//   update(); // ที่มีฟังก์ชั่น update
//   เพราะว่าต้องการให้มันตรวจก่อนว่าเวลาที่โรคจะหมดยังไม่เท่ากับ 0
//             // แล้วค่อยใช้ action
//   //-------------------------//
//   inline AfflictionSystem *getParent();
//   inline void setName(const string n);
//   inline void setDuration(const int d);
//   //-------------------------//
//   inline string getName();
//   inline int getDuration();
// };

// class AbilitySystem {
// private:
//   vector<Ability *> abilities; // สร้างเป็นเวกเตอร์ของ pointer เพื่อเวลาที่ Ability
//                                // มันพัฒนา จะได้สามารถเปลี่ยนค่า pointer แทนที่ได้เลย
//   GameObject *parent;
// public:
//   explicit AbilitySystem(GameObject *parent, vector<Ability *> abilities);
//   //-------------------------//
//   bool addAbility(Ability *); // มีชื่อเดียวกันอยู่แล้วไหม
//   bool removeAbility(const string); // มีให้ลบไหม
//   bool isInAbility(const string);   // มีอยู่ไหม
//   void decisionMakeing(const int, const int, const int);
//   //-------------------------// ไม่ทำ set
//   inline GameObject *getParent();
//   inline int getAbilitySize(); // มีทำไม
// };

// class AfflictionSystem {
// private:
//   vector<Affliction *> afflictions;
//   GameObject *parent;
//   bool checkAlive(); // ตายยัง?

// public:
//   explicit AfflictionSystem(GameObject *parent,
//                             vector<Affliction *> afflictions);
//   ~AfflictionSystem();
//   //-------------------------//
//   bool addAffliction(
//       Affliction *); // มีอยู่แล้วไหม ถ้ามมีก็ใส่เพิ่มรีเทิร์นจริง
//       ไม่มีก็ใส่เพิ่มเหมือนกันแต่รีเทิร์นเท็จ
//   bool removeAffliction(string name);     // มีให้ลบไหม
//   bool isInAffliction(const string name); // มีอยู่ไหม
//   bool updateAffliction(); // ยังไม่ตายใช่ไหม
//   //-------------------------// ไม่ทำ set เหตุผลเหมือนกับ AbilitySystem
//   inline vector<Affliction *> *getAfflictions();
//   inline GameObject *getParent();
//   inline int getAfflictionSize();
// };

// class StatusSystem : public AbilitySystem, public AfflictionSystem {
// private:
//   vector<StatParameter *>
//       value; // GameObject บางแต่ละประเภทก็มี param ต่างกันไป จึงทำเช่นนี้
//   GameObject *parentParamiter; // ต้องเป็น pointer เพราะว่ามันสามารถถูกโอนได้

// public:
//   explicit StatusSystem(GameObject *parent, vector<StatParameter *> *value,
//                         vector<Ability *> *abilities,
//                         vector<Affliction *> *afflictions);
//   // ~StatusSystem();
//   //-------------------------//
//   bool isInStat(string name); // เอาไว้เช็คว่าอยู่ใน value ไหม
//   // void changeStat(string name, int value); // ใช้เพื่อเปลี่ยนค่าของ parameter
//   bool addStat(string name); // ต้องชื่อไม่ซ้ำ เอาไว้เป็น StatParameterMax
//   bool removeStat(string name); // มีอยู่ไหม
//   void
//   setParent(GameObject *p); // เปลี่ยน parent ของ stat ให้เป็น p โดยไม่สนอะไรทั้งนั้น
//   //-------------------------//
//   StatParameter *
//   getValue(string name); // ใช้เพื่อเอา parameter object ที่มีชื่อตรงกันออกมา
//   vector<StatParameter *> &getValue();
//   inline StatParameterCoord *getPosition(); // ใช้เพื่อเอาค่า x ออกมา
//   inline GameObject *getParent();
// };

// // GameObject's parent is Layer btw, not LayerSystem.
// // This means Gameobjects can't just move layers since parent can't be
// changed
// // rn
// class GameObject {
// private:
//   string represent;
//   string name;
//   StatusSystem *stat; // stat ต้องเป็น pointer เพราะว่ามันสามาถเปลี่ยนเป็นตัวอื่นได้
//   Layer *parent; // stat ต้องเป็น pointer เพราะว่ามันสามาถเปลี่ยนย้าย layer ได้

// public:
//   explicit GameObject(Layer *, string, string, vector<StatParameter *> *,
//                       vector<Ability *> *,
//                       vector<Affliction *> *); // ต้องมี parent ใน stat ด้วย
//   //-------------------------//
//   void virtual calculateNeed();
//   void virtual update(); // ให้ class ลูกไปเขียนเองเพราะบางอย่าง เช่น พื้นดิน
//                          // ก็ไม่ต้องตัดสินใจเหมือนสัตว์
//   //-------------------------//
//   inline void setName(const string n);
//   inline void setRepresent(const string r);
//   void setStat(StatusSystem *statInput); // ไปแย่งเอา stat ของตัวอื่นมาใช้
//                                          // จะเอามาใช้สำหรับกรณีสัตว์วิวัฒนาการ
//   void setParent(Layer *p); // เปลี่ยนแบบไม่สนว่าที่ตรงนั้นจะไปทับใครใน layer
//   เดียวกันไหม
//   //-------------------------//
//   string getRepresent();
//   string getName();
//   StatusSystem *getStat();
//   Layer *getParent();
//   //-------------------------//
// };
