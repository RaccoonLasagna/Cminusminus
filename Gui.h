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
#include <SFML/Graphics.hpp>
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
  Font font;
  Text text;

public:
  Grid(Vector2f size, Vector2f position, GameObject *object);
  void draw(RenderWindow &window); /*not require just have*/
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
  void update(); /*สร้าง gird และลงทะเบียน grid ทั้งหมดใหม่โดยใช้ข้อมูลที่มีอยู่ในปัจจุบัน*/

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
  RectangleShape getBorder() { return border; }
};

class ZoomIn {
protected:
  CircleShape shape;
  Font font;
  Text text;

public:
  ZoomIn(Vector2f position, int radius);
  void draw(RenderWindow &window);
  CircleShape getShape();
  void zoomIn(Map &map);
};

class ZoomOut : public ZoomIn {
public:
  ZoomOut(Vector2f position, int radius);
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
              vector<vector<GameObject *>> dataMap);
  void draw(RenderWindow &window);
  void setDataMap(vector<vector<GameObject *>> layer);
  void pushToMap(Map &map);
  RectangleShape getButton() { return button; }
};

class LayerSheet {
private:
  RectangleShape sheet;
  LayerSystem *data;
  Font font;
  vector<Text> text;
  void dumbToButton(vector<vector<GameObject *>> DataMap, LayerButton &button);

public:
  LayerButton *selectedbutton;
  vector<LayerButton> buttons;
  LayerSheet(LayerSystem *data, Vector2f position, Vector2f size);
  void draw(RenderWindow &window);
  void setSelectedButton(int i);
  LayerSystem *getData() { return data; }
  void update(); /*จะทำการเล่นเทิร์นพร้อมกับ dumb ข้อมูลทั้งหมดออกมา*/
};

class StopButton {
protected:
  RectangleShape shape;
  Font font;
  Text text;

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
  RectangleShape shape, up, down;
  vector<Text> showText; // จำนวนจะเปลี่ยนไปตามขนาดของ TextBox
  vector<string> allText;
  Font font;
  int showTextIndex, showTextCapacity, textCapacity;
  Text textUp, textDown;

public:
  TextBox(Vector2f position, Vector2f size, vector<string> defaultVec,
          int textCapacity = 50,
          int showTextCapacity = 10); // ข้อควรระวัง defaultVec
                                      // ต้องมีขนาดเท่ากับหรือมากกว่า ShowTextCapacity
  void update();
  void draw(RenderWindow &window);
  void dumbText(string text);
  void dumbText(vector<string> text);
  RectangleShape getShape();
  RectangleShape getUp() { return up; }
  RectangleShape getDown() { return down; }
  void shiftShowTextUp();
  void shiftShowTextDown();
  void setAllText(vector<string> text) { allText = text; }
};

class Detail : public TextBox {
protected:
  GameObject *object;
  CircleShape deleteButton;
  void update();

public:
  Detail(Vector2f position, Vector2f size);
  void draw(RenderWindow &window);
  void setObject(GameObject *object);
  CircleShape getDeleteButton();
};

class Log : public TextBox {
protected:
  float DefaultHeight;
  void update();

public:
  Log(Vector2f position, Vector2f size);
};

class SelectedList {
  RectangleShape shape, up, down;
  vector<Text> showText; // จำนวนจะเปลี่ยนไปตามขนาดของ TextBox
  vector<RectangleShape> showShape;
  Text textUp, textDown;
  Font font;
  int showTextIndex, textCapacity;

public:
  vector<GameObject *> allObject;
  SelectedList(Vector2f position, Vector2f size,
               vector<GameObject *> allObject = {}, int textCapacity = 5,
               int showTextCapacity = 5);
  void draw(RenderWindow &window);
  void dumbObject(GameObject *object);
  RectangleShape getShape();
  RectangleShape getUp() { return up; }
  RectangleShape getDown() { return down; }
  void shiftShowTextUp();
  void shiftShowTextDown();
  GameObject *click(Vector2f position);
  void update();
  void setAllObject(vector<GameObject *> object) { allObject = object; }
};

class CommandList {
protected:
  RectangleShape shape, up, down;
  vector<Text> showText; // จำนวนจะเปลี่ยนไปตามขนาดของ TextBox
  vector<RectangleShape> showShape;
  Text textUp, textDown;
  Font font;
  int showTextIndex, textCapacity;

public:
  vector<string> allObject;
  CommandList(Vector2f position, Vector2f size, vector<string> allObject = {}, int textCapacity = 5,
               int showTextCapacity = 5);
  void draw(RenderWindow &window);
  RectangleShape getShape() {return shape;}
  RectangleShape getUp() { return up; }
  RectangleShape getDown() { return down; }
  void shiftShowTextUp();
  void shiftShowTextDown();
  string click(Vector2f position);
};

class Gui {
private:
  int turnCount = 0;
  RenderWindow *window;
  Text turn;
  Font font;
  Event event;
  Vector2i mousePosition;
  Vector2f mousePositionView;
  Keyboard::Key key;
  bool play, run;
  bool detailShow, SelectedListShow, CommandListShow;
  Sprite MiniMap;
  Texture MiniMapTexture;

protected:
  LayerSheet *layerSheet;
  Map *map;
  ZoomIn *zoomIn;
  ZoomOut *zoomOut;
  StopButton *stopButton;
  PlayButton *playButton;
  RunButton *runButton;
  Detail *detail;
  Log *log;
  SelectedList *selectedList;
  CommandList *commandList;

  vector<LayerButton> layerButtons;
  vector<vector<GameObject *>> dataMap;
  vector<GameObject *> selectedObject;
  vector<string> commandListText;
  vector<string> logText;

public:
  Gui();
  bool isOpen();
  void updateMousePosition();
  void updateKeyboard();
  void updateUpdateButton();
  void updateLayerSystem();
  void updateLayerSheet();
  void updateSelectedList();
  void updateZoomButton();
  void updateDetail();
  void updateMapCenter();
  void updateLog();
  void update();
  void pollEvent();

  void renderDetail();
  void render();
};

#endif