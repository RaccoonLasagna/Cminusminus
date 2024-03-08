#include "FundamentalSystem.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/Window.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include <SFML/Graphics/Text.hpp>
#include <string>
#include <vector>

using namespace sf;
using namespace std;

#ifndef GUI_H
#define GUI_H

class Grid;
class Map;
class Gui;

class Grid {
private:
  RectangleShape shape;
  GameObject *object;
  Text text;

public:
  Grid(Vector2f size, Vector2f position, Font font,
       GameObject *object = nullptr); /*ตั้งค่าตัวแปรต่างๆและถ้า object != nullptr ก็ให้
                                         text เป็น represent ของ GameObject นั้นๆ*/
  void draw(RenderWindow &window);    /*not require just have*/
  GameObject *getObject();
};

class Map { // แสดงเฉพาะส่วน
private:
  float width, height; /*จำเป็นในการตีตาราง*/
  int row, column; /*จำเป็นในการตีตาราง*/
  Vector2i center; /*จุดกึ่งกลางของการแสดงผล*/
  RectangleShape border;        /*shape ที่กำหนด position*/
  vector<vector<Grid>> gridMap; /*เวลาซูม เคลียแล้วสร้างใหม่หมดเลยง่ายกว่า*/
  vector<vector<GameObject *>> dataMap; /*ต้องไปดึงมาทุกเทิร์น*/
  Font font;                            // กำหนด font ให้ grid
  void update(); /*สร้าง gird และลงทะเบียน grid ทั้งหมดใหม่โดยใช้ข้อมูลที่มีอยู่ในปัจจุบัน*/

  // minimap command
  /*void changeViewSize(MiniMap &miniMap);
  void changeViewCenter(MiniMap &miniMap);*/

public:
  Map(Vector2f size, Vector2f position, vector<vector<GameObject *>> dataMap);
  void draw(RenderWindow &window);
  void setDataMap(vector<vector<GameObject *>> dataMap);
  void setRowColumn(Vector2i size);
  void changeCenter(Vector2i center);
  GameObject *getObject(Vector2f position);
  Vector2i getRowColumn();
  Vector2i getSize();
  Vector2i getCenter();
  vector<vector<GameObject *>> getDataMap();
};

class ZoomIn {
protected:
  CircleShape shape;
  Font font;
  Text text;

public:
  ZoomIn(Vector2f position, int radius, Font font);
  void draw(RenderWindow &window);
  CircleShape getShape();
  void zoomIn(Map &map);
};

class ZoomOut : public ZoomIn {
public:
  ZoomOut(Vector2f position, int radius, Font font);
  void zoomOut(Map &map);
};

class LayerButton {
private:
  RectangleShape button;
  Font font;
  Text text;
  vector<vector<GameObject *>> dataMap;

public:
  LayerButton(Vector2f position, Vector2f size, string text,
              vector<vector<GameObject *>> dataMap, Font font);
  void draw(RenderWindow &window);
  void setDataMap(vector<vector<GameObject *>> layer);
  void pushToMap(Map &map);
};

class LayerSheet {
private:
  RectangleShape sheet;
  vector<LayerButton> buttons;
  LayerButton *selectedbutton;
  LayerSystem *data;
  Font font;
  void dumbToButton(vector<vector<GameObject *>> DataMap, LayerButton &button);

public:
  LayerSheet(LayerSystem *data, Vector2f position, Vector2f size, Font font);
  void draw(RenderWindow &window);
  void setSelectedButton(int i);
  void update(); /*จะทำการเล่นเทิร์นพร้อมกับ dumb ข้อมูลทั้งหมดออกมา*/
};

// class MiniMap {
// private:
//   RectangleShape miniMap;
//   RectangleShape view;
//   pair<int, int> viewCenter, viewSize;
//   Vector2f sizePerGrid;
//   void changeMapCenter(Map &map, Vector2f position);
//   void update();

// public:
//   MiniMap(Vector2f position, float scale, Map *data);
//   void draw(RenderWindow &window);
//   void setViewSize(int x, int y);
//   void setViewCenter(int x, int y);
//   int getSizePerGrid();
// };

class StopButton {
protected:
  RectangleShape shape;

public:
  StopButton(Vector2f position, Vector2f size);
  void draw(RenderWindow &window);
  RectangleShape getShape();
};

class PlayButton : public StopButton {
public:
  PlayButton(Vector2f position, Vector2f size);
};

class RunButton : public StopButton {
public:
  RunButton(Vector2f position, Vector2f size);
};

class TextBox {
protected:
  RectangleShape shape;
  vector<Text> showText; // จำนวนจะเปลี่ยนไปตามขนาดของ TextBox
  vector<string> allText;
  Font font;
  int showTextIndex, textCapacity;

public:
  TextBox(Vector2f position, Vector2f size, Font font, vector<string> allText = {},
          int textCapacity = 5);
  void draw(RenderWindow &window);
  void dumbText(string text);
  RectangleShape getShape();
  void shiftShowTextUp();
  void shiftShowTextDown();
};

class Detail : public TextBox {
protected:
  GameObject *object;
  CircleShape deleteButton;
  void update();

public:
  Detail(Vector2f position, Vector2f size, Font font);
  void setObject(GameObject *object);
  CircleShape getDeleteButton();
};

class Log : public TextBox {
protected:
  float DefaultHeight;
  void update();

public:
  Log(Vector2f position, Vector2f size, Font font);
  void changeHeight(float height);
};

class SelectedList {
  RectangleShape shape;
  vector<Text> showText; // จำนวนจะเปลี่ยนไปตามขนาดของ TextBox
  vector<GameObject *> allObject;
  Font font;
  int showTextIndex, textCapacity;

public:
  SelectedList(Vector2f position, Vector2f size, Font font,
               vector<GameObject *> allObject = {}, int textCapacity = 5);
  void draw(RenderWindow &window);
  void dumbObject(GameObject *object);
  RectangleShape getShape();
  void shiftShowTextUp();
  void shiftShowTextDown();
  GameObject *click(Vector2f position);
};

class CommandList : public TextBox {
protected:
  void update();

public:
  CommandList(Vector2f position, Vector2f size, Font font);
  string click(Vector2f position);
};

class Gui {
private:
  RenderWindow *window;
  Font *font;
  Map *map;
  Mouse *mouse;
  ZoomIn *zoomIn;
  ZoomOut *zoomOut;
  LayerSheet *layerSheet;
  StopButton *stopButton;
  PlayButton *playButton;
  RunButton *runButton;
  Detail *detail;
  Log *log;
  SelectedList *selectedList;
  CommandList *commandList;
  Event *event;
  bool isClick;
  bool canRun;

public:
  Gui();
  void draw();
  void poll();
  RenderWindow *getWindow() { return window; }
  void update();
};

#endif