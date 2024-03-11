#include "Gui.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/Window.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include <random>
#include <sstream>

using namespace sf;
using namespace std;

#define FONT_PATH "C:/Users/ADMIN/Documents/GitHub/Cminusminus/Font/f.ttf"
#define COLOR1 Color(194, 168, 135, 255)
#define COLOR2 Color(223, 175, 94, 255)
#define COLOR3 Color(97, 113, 77, 255)
#define COLOR4 Color(143, 174, 147, 255)
#define COLOR5 Color(109, 21, 43, 255)

//---------------------------------Grid---------------------------------

Grid::Grid(Vector2f size, Vector2f position, GameObject *object)
    : object(object) {
  shape.setSize(size);
  shape.setPosition(position);
  shape.setOutlineThickness(2.f);
  shape.setOutlineColor(COLOR5);
  shape.setFillColor(COLOR4);
}

void Grid::draw(RenderWindow &window) { window.draw(shape); }

GameObject *Grid::getObject() { return object; }

//---------------------------------Map---------------------------------

void Map::update() {
  gridMap.clear();
  showText.clear();
  for (int i = 0; i < row; i++) {
    vector<Text> tempTextVec;
    vector<Grid> temp;
    for (int j = 0; j < column; j++) {
      Grid tempGrid(Vector2f((float)width / column, (float)height / row),
                    Vector2f(border.getPosition().x + j * (float)width / column,
                             border.getPosition().y + i * (float)height / row),
                    dataMap[center.x - floor(row / 2) + i]
                           [center.y - floor(column / 2) + j]);
      tempText.setFont(font);
      tempText.setCharacterSize((float)height / row);
      tempText.setFillColor(COLOR5);
      dataMap[center.x - floor(row / 2) + i][center.y - floor(column / 2) + j]
          ? tempText.setString(dataMap[center.x - floor(row / 2) + i]
                                      [center.y - floor(column / 2) + j]
                                          ->getRepresent())
          : tempText.setString("G");
      tempText.setPosition(
          Vector2f(border.getPosition().x + j * (float)width / column,
                   border.getPosition().y + i * (float)height / row));

      tempTextVec.push_back(tempText);
      temp.push_back(tempGrid);
    }
    gridMap.push_back(temp);
    showText.push_back(tempTextVec);
  }
}

Map::Map(Vector2f size, Vector2f position, vector<vector<GameObject *>> dataMap)
    : width(size.x), height(size.y), dataMap(dataMap), row(9), column(15),
      center(Vector2i(ceil(dataMap.size() / 2), ceil(dataMap[0].size() / 2))) {
  border.setSize(Vector2f(width, height));
  border.setFillColor(Color::Transparent);
  border.setOutlineColor(COLOR5);
  border.setOutlineThickness(2.0f);
  border.setPosition(position);
  font.loadFromFile(FONT_PATH);
  update();
}

void Map::draw(RenderWindow &window) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < column; j++) {
      gridMap[i][j].draw(window);
      window.draw(showText[i][j]);
    }
  }
  window.draw(border);
}

void Map::setDataMap(vector<vector<GameObject *>> dataMap) {
  this->dataMap = dataMap;
}

void Map::setRowColumn(Vector2i size) {
  this->row = size.x;
  this->column = size.y;
}

GameObject *Map::getObject(Vector2f position) {
  float gridHeight = (float)height / row;
  float gridWidth = (float)width / column;
  for (int i = 0; i < row; i++) {
    if (position.y >= i * gridHeight && position.y < (i + 1) * gridHeight) {
      for (int j = 0; j < column; j++) {
        if (position.x >= j * gridWidth && position.x < (j + 1) * gridWidth) {
          return gridMap[i][j].getObject();
        }
      }
    }
  }
  return nullptr;
}

void Map::changeCenter(Vector2i center) { this->center = center; }

Vector2i Map::getRowColumn() { return Vector2i(row, column); }
Vector2i Map::getSize() { return Vector2i(width, height); }
Vector2i Map::getCenter() { return center; }
vector<vector<GameObject *>> Map::getDataMap() { return dataMap; }

//---------------------------------ZoomIn---------------------------------

ZoomIn::ZoomIn(Vector2f position, int radius) {
  font.loadFromFile(FONT_PATH);
  shape.setOutlineColor(COLOR5);
  shape.setOutlineThickness(2.0f);
  text.setString("+");
  text.setFillColor(COLOR5);
  shape.setRadius(radius);
  shape.setPosition(position);
  text.setFont(font);
  Vector2f textPosition = Vector2f(position.x + 3, position.y - 12);
  text.setPosition(textPosition);
  shape.setFillColor(Color::Green);
}

void ZoomIn::draw(RenderWindow &window) {
  window.draw(shape);
  window.draw(text);
}

CircleShape ZoomIn::getShape() { return shape; }

void ZoomIn::zoomIn(Map &map) {
  Vector2i mapRowColumn = map.getRowColumn();
  if (mapRowColumn.y - 30 < 15 || mapRowColumn.x - 18 < 9) {
    return;
  }
  map.setRowColumn(Vector2i(mapRowColumn.x - 18, mapRowColumn.y - 30));
}

//---------------------------------ZoomOut---------------------------------

ZoomOut::ZoomOut(Vector2f position, int radius) : ZoomIn(position, radius) {
  shape.setFillColor(Color::Red);
  text.setString("-");
}

void ZoomOut::zoomOut(Map &map) {
  Vector2i mapRowColumn = map.getRowColumn();
  Vector2i mapCenter = map.getCenter();
  if (mapCenter.x - floor(mapRowColumn.x + 30 / 2) < 0 ||
      mapCenter.y - floor(mapRowColumn.y + 18 / 2) < 0 ||
      mapCenter.x + ceil(mapRowColumn.x + 30 / 2) > map.getDataMap().size() ||
      mapCenter.y + ceil(mapRowColumn.y + 18 / 2) >
          map.getDataMap()[0].size()) {
    return;
  }
  map.setRowColumn(Vector2i(mapRowColumn.x + 18, mapRowColumn.y + 30));
}

//---------------------------------LayerButton---------------------------------

LayerButton::LayerButton(Vector2f position, Vector2f size, string text,
                         vector<vector<GameObject *>> dataMap)
    : dataMap(dataMap) {
  font.loadFromFile(FONT_PATH);
  button.setPosition(position);
  button.setSize(size);
  button.setFillColor(COLOR3);
  button.setOutlineColor(COLOR5);
  button.setOutlineThickness(2.0f);
  /*คำนวน size ของ text ตาม size ของ button*/
  this->text.setString(text);
  // this->text.setFont(this->font); // i don't know why this line make error
  this->text.setCharacterSize(24); // เดะแก้ทีหลัง
  this->text.setFillColor(COLOR5);
  this->text.setPosition(position.x + 10, position.y + 10);
}

void LayerButton::draw(RenderWindow &window) {
  window.draw(button);
  window.draw(this->text);
}

void LayerButton::setDataMap(vector<vector<GameObject *>> dataMap) {
  this->dataMap = dataMap;
}
void LayerButton::pushToMap(Map &map) { map.setDataMap(dataMap); }

//---------------------------------LayerSheet---------------------------------

void LayerSheet::dumbToButton(vector<vector<GameObject *>> DataMap,
                              LayerButton &button) {
  button.setDataMap(DataMap);
}

LayerSheet::LayerSheet(LayerSystem *data, Vector2f position, Vector2f size)
    : data(data) {
  /*คำนวนขอบ*/
  font.loadFromFile(FONT_PATH);
  sheet.setPosition(position);
  sheet.setSize(size);
  sheet.setFillColor(Color::Transparent);
  sheet.setOutlineColor(COLOR5);
  sheet.setOutlineThickness(2.0f);
  float buttonWidth = size.x / ((data->layers.size() + 1) * (1 + 0.1));
  for (int i = 0; i < data->layers.size(); i++) {
    Text tempText;
    tempText.setFont(font);
    tempText.setCharacterSize(72);
    tempText.setFillColor(COLOR5);
    tempText.setString(data->layers[i]->getName());
    tempText.setPosition(Vector2f(
        position.x + (i * (buttonWidth + (0.1 * buttonWidth))), position.y));
    text.push_back(tempText);
    LayerButton tempButton(
        Vector2f(position.x + (i * (buttonWidth + (0.1 * buttonWidth))),
                 position.y),
        Vector2f(buttonWidth, size.y), data->layers[i]->getName(),
        data->layers[i]->insideLayer);
    buttons.push_back(tempButton);
  }
  Text tempTextz;
  tempTextz.setFont(font);
  tempTextz.setCharacterSize(72);
  tempTextz.setFillColor(COLOR5);
  tempTextz.setString("OverAll");
  tempTextz.setPosition(Vector2f(
      position.x + (3 * (buttonWidth + (0.1 * buttonWidth))), position.y));
  text.push_back(tempTextz);
  LayerButton tempButtonz(
      Vector2f(position.x + (3 * (buttonWidth + (0.1 * buttonWidth))),
               position.y),
      Vector2f(buttonWidth, size.y), "OverAll", data->getOverAllLayer());
  buttons.push_back(tempButtonz);
  selectedbutton = &(buttons[0]);
}

void LayerSheet::draw(RenderWindow &window) {
  window.draw(sheet);
  for (int i = 0; i < buttons.size(); i++) {
    buttons[i].draw(window);
    window.draw(text[i]);
  }
}

void LayerSheet::setSelectedButton(int i) { selectedbutton = &buttons[i]; }

void LayerSheet::update() {
  data->update();
  for (int i = 0; i < buttons.size(); i++) {
    if (i == buttons.size() - 1) {
      dumbToButton(data->getOverAllLayer(), buttons[0]);
    } else {
      dumbToButton(data->layers[i]->insideLayer, buttons[i]);
    }
  }
}

//---------------------------------MiniMap---------------------------------

//---------------------------------StopButton---------------------------------

StopButton::StopButton(Vector2f position, Vector2f size) {
  font.loadFromFile(FONT_PATH);
  text.setFont(font);
  text.setCharacterSize(24);
  text.setString("-");
  text.setFillColor(COLOR5);
  text.setPosition(position.x + 5, position.y - 5);
  shape.setPosition(position);
  shape.setSize(size);
  shape.setFillColor(Color::Red);
  shape.setOutlineColor(COLOR5);
  shape.setOutlineThickness(2.0f);
}

void StopButton::draw(RenderWindow &window) {
  window.draw(shape);
  window.draw(text);
}

RectangleShape StopButton::getShape() { return shape; }

//---------------------------------PlayButton---------------------------------

PlayButton::PlayButton(Vector2f position, Vector2f size)
    : StopButton(position, size) {
  text.setString("+");
  shape.setPosition(position);
  shape.setSize(size);
  shape.setFillColor(Color::Green);
  text.setPosition(position.x + 5, position.y - 6);
}

//---------------------------------RunButton---------------------------------

RunButton::RunButton(Vector2f position, Vector2f size)
    : StopButton(position, size) {
  text.setString(">");
  shape.setPosition(position);
  shape.setSize(size);
  shape.setFillColor(Color::Green);
}

//---------------------------------TextBox---------------------------------

TextBox::TextBox(Vector2f position, Vector2f size, vector<string> defaultVec,
                 int textCapacity, int showTextCapacity)
    : textCapacity(textCapacity), showTextCapacity(showTextCapacity),
      showTextIndex(0), allText(defaultVec) {
  font.loadFromFile(FONT_PATH);
  shape.setPosition(position);
  shape.setSize(size);
  shape.setFillColor(COLOR3);
  shape.setOutlineColor(COLOR5);
  shape.setOutlineThickness(2.0f);
  for (int i = 0; i < showTextCapacity; i++) {
    Text tempText;
    tempText.setFont(font);
    tempText.setCharacterSize(floor(size.y / showTextCapacity));
    tempText.setFillColor(COLOR5);
    tempText.setPosition(position.x,
                         position.y + ((size.y / showTextCapacity) * i));
    showText.push_back(tempText);
  }
  update();
  up.setSize(Vector2f(20, 20));
  up.setPosition(Vector2f(position.x + size.x - 20, position.y));
  up.setFillColor(Color::Green);
  up.setOutlineColor(COLOR5);
  up.setOutlineThickness(2.0f);
  textUp.setFont(font);
  textUp.setCharacterSize(32);
  textUp.setFillColor(COLOR5);
  textUp.setString("+");
  textUp.setPosition(Vector2f(position.x + size.x - 16, position.y - 12));
  down.setSize(Vector2f(20, 20));
  down.setPosition(
      Vector2f(position.x + size.x - 20, position.y + size.y - 20));
  down.setFillColor(Color::Green);
  down.setOutlineColor(COLOR5);
  down.setOutlineThickness(2.0f);
  textDown.setFont(font);
  textDown.setCharacterSize(32);
  textDown.setFillColor(COLOR5);
  textDown.setString("-");
  textDown.setPosition(
      Vector2f(position.x + size.x - 16, position.y + size.y - 30));
}

void TextBox::update() {
  for (int i = 0; i < showTextCapacity; i++) {
    showText[i].setString(allText[showTextIndex + i]);
  }
}

void TextBox::draw(RenderWindow &window) {
  window.draw(shape);
  for (int i = 0; i < showText.size(); i++) {
    window.draw(showText[i]);
  }
  window.draw(up);
  window.draw(down);
  window.draw(textUp);
  window.draw(textDown);
}

void TextBox::dumbText(string text) {
  allText.push_back(text);
  if (allText.size() > textCapacity) {
    allText.erase(allText.begin());
  }
  if (showTextIndex < allText.size() - textCapacity) {
    showTextIndex = allText.size() - textCapacity;
  }
  update();
}

void TextBox::dumbText(vector<string> text) {
  for (int i = 0; i < text.size(); i++) {
    allText.push_back(text[i]);
    if (allText.size() > textCapacity) {
      allText.erase(allText.begin());
    }
    if (showTextIndex < allText.size() - textCapacity) {
      showTextIndex = allText.size() - textCapacity;
    }
  }
  update();
}

RectangleShape TextBox::getShape() { return shape; }

void TextBox::shiftShowTextUp() {
  if (showTextIndex > 0) {
    showTextIndex--;
    update();
  }
}

void TextBox::shiftShowTextDown() {
  if (showTextIndex + showTextCapacity < allText.size()) {
    showTextIndex++;
    update();
  }
}

//---------------------------------Detail---------------------------------

void Detail::update() {
  if (object) {
    setAllText(object->getDetail());
    TextBox::update();
  }
}

Detail::Detail(Vector2f position, Vector2f size)
    : TextBox(position, size,
              {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11 "}, 100,
              10) {
  deleteButton.setRadius(10);
  deleteButton.setFillColor(Color::Red);
  deleteButton.setPosition(
      Vector2f(shape.getPosition().x + shape.getSize().x - 50,
               shape.getPosition().y + 10));
  deleteButton.setOutlineColor(COLOR5);
  deleteButton.setOutlineThickness(2.0f);
}

void Detail::setObject(GameObject *object) {
  this->object = object;
  update();
}

void Detail::draw(RenderWindow &window) {
  TextBox::draw(window);
  if (object) {
    window.draw(deleteButton);
  }
}

CircleShape Detail::getDeleteButton() { return deleteButton; }

//---------------------------------Log---------------------------------

void Log::update() {
  // ไปดึงข้อมูลการกระทำของสัตว์มาแสดง
}

Log::Log(Vector2f position, Vector2f size)
    : TextBox(position, size,
              {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"}, 500,
              10) {
  DefaultHeight = position.y;
  shape.setFillColor(COLOR1);
}

//---------------------------------SelectedList---------------------------------

void SelectedList::update() {
  for (int i = 0; i < showTextCapacity; i++) {
    if (showTextIndex + i < allObject.size()) {
      showText[i].setString(allObject[showTextIndex + i]->getRepresent());
    } else {
      showText[i].setString(" ");
    }
  }
}

SelectedList::SelectedList(Vector2f position, Vector2f size,
                           vector<GameObject *> allObject, int showTextCapacity)
    : showTextIndex(0), showTextCapacity(showTextCapacity),
      allObject(allObject) {
  font.loadFromFile(FONT_PATH);
  shape.setPosition(position);
  shape.setSize(size);
  shape.setFillColor(COLOR1);
  shape.setOutlineColor(COLOR5);
  shape.setOutlineThickness(2.0f);
  for (int i = 0; i < showTextCapacity; i++) {
    tempText.setFont(font);
    tempShape.setSize(Vector2f(size.x, size.y / showTextCapacity));
    tempShape.setPosition(position.x,
                          position.y + (size.y * i / showTextCapacity));
    tempShape.setFillColor(COLOR1);
    tempShape.setOutlineColor(COLOR5);
    tempShape.setOutlineThickness(3.0f);
    showShape.push_back(tempShape);
    tempText.setCharacterSize(size.y / showTextCapacity);
    tempText.setFillColor(COLOR5);
    tempText.setPosition(position.x + 10,
                         position.y + (size.y * i / showTextCapacity));
    showText.push_back(tempText);
  }
  update();
  up.setSize(Vector2f(20, 20));
  up.setPosition(Vector2f(position.x + size.x - 20, position.y));
  up.setFillColor(Color::Green);
  up.setOutlineColor(COLOR5);
  up.setOutlineThickness(2.0f);
  textUp.setFont(font);
  textUp.setCharacterSize(32);
  textUp.setFillColor(COLOR5);
  textUp.setString("+");
  textUp.setPosition(Vector2f(position.x + size.x - 16, position.y - 12));
  down.setSize(Vector2f(20, 20));
  down.setPosition(
      Vector2f(position.x + size.x - 20, position.y + size.y - 20));
  down.setFillColor(Color::Green);
  down.setOutlineColor(COLOR5);
  down.setOutlineThickness(2.0f);
  textDown.setFont(font);
  textDown.setCharacterSize(32);
  textDown.setFillColor(COLOR5);
  textDown.setString("-");
  textDown.setPosition(
      Vector2f(position.x + size.x - 16, position.y + size.y - 30));
}

void SelectedList::draw(RenderWindow &window) {
  window.draw(shape);
  for (int i = 0; i < showText.size(); i++) {
    window.draw(showShape[i]);
    window.draw(showText[i]);
  }
  window.draw(up);
  window.draw(down);
  window.draw(textUp);
  window.draw(textDown);
}

void SelectedList::dumbObject(GameObject *object) {
  allObject.push_back(object);
  showTextIndex = allObject.size() - 1;
  update();
}

RectangleShape SelectedList::getShape() { return shape; }

void SelectedList::shiftShowTextUp() {
  if (showTextIndex > 0) {
    showTextIndex--;
    update();
  }
}

void SelectedList::shiftShowTextDown() {
  if (showTextIndex < allObject.size() - showTextCapacity) {
    showTextIndex++;
    update();
  }
}

GameObject *SelectedList::click(Vector2f position) {
  for (int i = 0; i < showTextCapacity; i++) {
    if (showShape[i].getGlobalBounds().contains(position) && showTextIndex + i < allObject.size()) {
      return allObject[showTextIndex + i];
    }
  }
  return nullptr;
}

void SelectedList::clear() {
  allObject.clear();
  showTextIndex = 0;
  update();
}

//---------------------------------CommandList---------------------------------

CommandList::CommandList(Vector2f position, Vector2f size,
                         vector<string> allObject, int textCapacity,
                         int showTextCapacity) {
  font.loadFromFile(FONT_PATH);
  shape.setPosition(position);
  shape.setSize(size);
  shape.setFillColor(COLOR1);
  shape.setOutlineColor(COLOR5);
  shape.setOutlineThickness(2.0f);
  for (int i = 0; i < textCapacity; i++) {
    Text tempText;
    RectangleShape tempShape;
    tempText.setFont(font);
    /*คำนวนขนาด text ตาม size ของ textBox*/
    tempShape.setSize(Vector2f(size.x, size.y / showTextCapacity));
    tempShape.setPosition(position.x,
                          position.y + (size.y * i / showTextCapacity));
    tempShape.setFillColor(COLOR3);
    tempShape.setOutlineColor(COLOR5);
    tempShape.setOutlineThickness(3.0f);
    showShape.push_back(tempShape);
    tempText.setCharacterSize(24);
    tempText.setFillColor(Color::White);
    tempText.setPosition(position.x + 10,
                         position.y + (size.y * i / showTextCapacity));
    showText.push_back(tempText);
  }
  for (int i = 0; i < textCapacity && i < allObject.size(); i++) {
    if (i < allObject.size()) {
      showText[i].setString(allObject[i]);
    } else {
      showText[i].setString(" ");
    }
  }
  up.setSize(Vector2f(20, 20));
  up.setPosition(Vector2f(position.x + size.x - 20, position.y));
  up.setFillColor(Color::Green);
  up.setOutlineColor(COLOR5);
  up.setOutlineThickness(2.0f);
  textUp.setFont(font);
  textUp.setCharacterSize(32);
  textUp.setFillColor(COLOR5);
  textUp.setString("+");
  textUp.setPosition(Vector2f(position.x + size.x - 16, position.y - 12));
  down.setSize(Vector2f(20, 20));
  down.setPosition(
      Vector2f(position.x + size.x - 20, position.y + size.y - 20));
  down.setFillColor(Color::Green);
  down.setOutlineColor(COLOR5);
  down.setOutlineThickness(2.0f);
  textDown.setFont(font);
  textDown.setCharacterSize(32);
  textDown.setFillColor(COLOR5);
  textDown.setString("-");
  textDown.setPosition(
      Vector2f(position.x + size.x - 16, position.y + size.y - 30));
}

void CommandList::draw(RenderWindow &window) {
  window.draw(shape);
  for (int i = 0; i < showText.size(); i++) {
    window.draw(showShape[i]);
    window.draw(showText[i]);
  }
  window.draw(up);
  window.draw(down);
  window.draw(textUp);
  window.draw(textDown);
}

void CommandList::shiftShowTextUp() {
  if (showTextIndex > 0) {
    showTextIndex--;
    for (int i = 0; i < textCapacity; i++) {
      if (showTextIndex + i < allObject.size()) {
        showText[i].setString(allObject[showTextIndex + i]);
      } else {
        showText[i].setString(" ");
      }
    }
  }
}

void CommandList::shiftShowTextDown() {
  if (showTextIndex < allObject.size() - textCapacity) {
    showTextIndex++;
    for (int i = 0; i < textCapacity; i++) {
      if (showTextIndex + i < allObject.size()) {
        showText[i].setString(allObject[showTextIndex + i]);
      } else {
        showText[i].setString(" ");
      }
    }
  }
}

string CommandList::click(Vector2f position) {
  for (int i = 0; i < showShape.size(); i++) {
    if (showShape[i].getGlobalBounds().contains(position)) {
      return allObject[showTextIndex + i];
    }
  }
  return "";
}

//---------------------------------Gui---------------------------------

void Gui::renderAny() {
  window->draw(turn);
  window->draw(sprite);
}

void Gui::pollEvent() {
  while (window->pollEvent(event)) {
    if (event.type == Event::Closed) {
      window->close();
    }
  }
}

void Gui::updateMousePosition() {
  mousePosition = Mouse::getPosition(*window);
  mousePositionView = window->mapPixelToCoords(mousePosition);
}

bool Gui::clickAbleCheck() {
  if (Mouse::isButtonPressed(Mouse::Left) && clickAble) {
    clickAble = false;
    return true;
  }
  return false;
}

void Gui::updateClickAble() {
  if (!Mouse::isButtonPressed(Mouse::Left)) {
    clickAble = true;
  }
}

void Gui::updateKeyboard() {
  if (Keyboard::isKeyPressed(Keyboard::Up)) {
    key = Keyboard::Up;
  } else if (Keyboard::isKeyPressed(Keyboard::Down)) {
    key = Keyboard::Down;
  } else if (Keyboard::isKeyPressed(Keyboard::Left)) {
    key = Keyboard::Left;
  } else if (Keyboard::isKeyPressed(Keyboard::Right)) {
    key = Keyboard::Right;
  } else if (Keyboard::isKeyPressed(Keyboard::LShift)) {
    key = Keyboard::LShift;
  } else {
    key = Keyboard::Unknown;
  }
}

bool Gui::pressAbleCheck() {
  if (pressAble) {
    pressAble = false;
    return true;
  }
  return false;
}

void Gui::updatePressAble() {
  if (!(Keyboard::isKeyPressed(Keyboard::Up) ||
        Keyboard::isKeyPressed(Keyboard::Down) ||
        Keyboard::isKeyPressed(Keyboard::Left) ||
        Keyboard::isKeyPressed(Keyboard::Right) ||
        Keyboard::isKeyPressed(Keyboard::LShift))) {
    pressAble = true;
  }
}

void Gui::updateUpdateButton() {
  if (stopButton->getShape().getGlobalBounds().contains(mousePositionView)) {
    if (clickAbleCheck()) {
      run = play = false;
    }
  }
  if (playButton->getShape().getGlobalBounds().contains(mousePositionView)) {
    if (clickAbleCheck()) {
      play = true;
      run = false;
    }
  }
  if (runButton->getShape().getGlobalBounds().contains(mousePositionView)) {
    if (clickAbleCheck()) {
      run = true;
    }
  }
}

void Gui::updateLayerSystem() {
  if (play || run) {
    layerSheet->update();
    turnCount++;
    turn.setString("Turn: " + to_string(turnCount));
    log->dumbText(layerSheet->getData()->logActionData);
    play ? play = false : void();
  }
}

void Gui::renderUpdateButton() {
  stopButton->draw(*window);
  playButton->draw(*window);
  runButton->draw(*window);
}

void Gui::updateLayerSheet() {
  for (int i = 0; i < layerSheet->buttons.size(); i++) {
    if (layerSheet->buttons[i].getButton().getGlobalBounds().contains(
            mousePositionView)) {
      if (clickAbleCheck()) {
        layerSheet->setSelectedButton(i);
        break;
      }
    }
  }
  layerSheet->selectedbutton->pushToMap(*map);
}

void Gui::renderLayerSheet() { layerSheet->draw(*window); }

void Gui::updateMapCenter() {
  if (key == Keyboard::Up) {
    pressAbleCheck() ? map->changeCenter(map->getCenter() + Vector2(0, 1))
                     : void();
  } else if (key == Keyboard::Down) {
    pressAbleCheck() ? map->changeCenter(map->getCenter() + Vector2(0, -1))
                     : void();
  } else if (key == Keyboard::Left) {
    pressAbleCheck() ? map->changeCenter(map->getCenter() + Vector2(1, 0))
                     : void();
  } else if (key == Keyboard::Right) {
    pressAbleCheck() ? map->changeCenter(map->getCenter() + Vector2(-1, 0))
                     : void();
  }
}

void Gui::renderMap() {
  map->update();
  map->draw(*window);
}

void Gui::updateZoomButton() {
  if (zoomIn->getShape().getGlobalBounds().contains(mousePositionView)) {
    clickAbleCheck() ? zoomIn->zoomIn(*map) : void();
  } else if (zoomOut->getShape().getGlobalBounds().contains(
                 mousePositionView)) {
    clickAbleCheck() ? zoomOut->zoomOut(*map) : void();
  }
}

void Gui::renderZoomButton() {
  zoomIn->draw(*window);
  zoomOut->draw(*window);
}

void Gui::updateDetail() {
  if (map->getBorder().getGlobalBounds().contains(mousePositionView)) {
    if (clickAbleCheck()) {
      GameObject *object = map->getObject(mousePositionView);
      if (object) {
        detail->setObject(object);
        detailShow = true;
      }
    }
  } else if (selectedList->getShape().getGlobalBounds().contains(
                 mousePositionView)) {
    if (clickAbleCheck()) {
      GameObject *object = selectedList->click(mousePositionView);
      if (object) {
        detail->setObject(object);
        detailShow = true;
      }
    }

  } else if (detail->getDeleteButton().getGlobalBounds().contains(
                 mousePositionView) &&
             detailShow) {
    if (clickAbleCheck()) {
      detail->setObject(nullptr);
      detailShow = false;
    }
  } else if (detail->getUp().getGlobalBounds().contains(mousePositionView) &&
             detailShow) {
    clickAbleCheck() ? detail->shiftShowTextUp() : void();
  } else if (detail->getDown().getGlobalBounds().contains(mousePositionView) &&
             detailShow) {
    clickAbleCheck() ? detail->shiftShowTextDown() : void();
  }
}

void Gui::renderDetail() {
  if (detailShow) {
    detail->draw(*window);
  }
}

void Gui::updateLog() {
  if (log->getUp().getGlobalBounds().contains(mousePositionView)) {
    clickAbleCheck() ? log->shiftShowTextUp() : void();
  } else if (log->getDown().getGlobalBounds().contains(mousePositionView)) {
    clickAbleCheck() ? log->shiftShowTextDown() : void();
  }
}

void Gui::renderLog() { log->draw(*window); }

void Gui::updateSelectedList() {
  if (map->getBorder().getGlobalBounds().contains(mousePositionView) &&
      key == Keyboard::LControl) {
    if (clickAbleCheck()) {
      GameObject *object = map->getObject(mousePositionView);
      object ? selectedList->dumbObject(object) : void();
    }
  } else if (map->getBorder().getGlobalBounds().contains(mousePositionView)) {
    if (clickAbleCheck()) {
      GameObject *object = map->getObject(mousePositionView);
      selectedList->clear();
      object ? selectedList->dumbObject(object) : void();
    }
  } else if (selectedList->getUp().getGlobalBounds().contains(
                 mousePositionView)) {
    clickAbleCheck() ? selectedList->shiftShowTextUp() : void();
  } else if (selectedList->getDown().getGlobalBounds().contains(
                 mousePositionView)) {
    clickAbleCheck() ? selectedList->shiftShowTextDown() : void();
  }
}

void Gui::renderSelectedList() { selectedList->draw(*window); }

void Gui::updateCommandList() {
  if (commandList->getUp().getGlobalBounds().contains(mousePositionView)) {
    clickAbleCheck() ? commandList->shiftShowTextUp() : void();
  } else if (commandList->getDown().getGlobalBounds().contains(
                 mousePositionView)) {
    clickAbleCheck() ? commandList->shiftShowTextDown() : void();
  }
}

void Gui::renderCommandList() { commandList->draw(*window); }

Gui::Gui() {
  this->window =
      new RenderWindow(VideoMode(1920, 1080), "Ecosystem Simulation");
  font.loadFromFile(FONT_PATH);
  window->setFramerateLimit(120);
  layerSheet = new LayerSheet(new LayerSystem(255, 153), Vector2f(0, 0),
                              Vector2f(1920, 100));
  map = new Map(Vector2f(1291, 775), Vector2f(629, 150),
                layerSheet->getData()->getOverAllLayer());
  zoomIn = new ZoomIn(Vector2f(1840, 115), 10);
  zoomOut = new ZoomOut(Vector2f(1875, 115), 10);
  font.loadFromFile(FONT_PATH);
  turn.setFont(font);
  turn.setCharacterSize(24);
  turn.setFillColor(COLOR5);
  turn.setString("Turn: " + to_string(turnCount));
  turn.setPosition(Vector2f(629, 115));
  stopButton = new StopButton(Vector2f(100, 115), Vector2f(20, 20));
  playButton = new PlayButton(Vector2f(230, 115), Vector2f(20, 20));
  runButton = new RunButton(Vector2f(360, 115), Vector2f(20, 20));

  log = new Log(Vector2f(629, 925), Vector2f(1291, 155));
  detail = new Detail(Vector2f(0, 150), Vector2f(625, 555.6));

  selectedList = new SelectedList(Vector2f(0, 427.8), Vector2f(625, 277.8), {});
  commandList = new CommandList(Vector2f(0, 150), Vector2f(625, 277.8));
  spriteTexture.loadFromFile(
      "C:\\Users\\ADMIN\\Documents\\GitHub\\Cminusminus\\22676.jpg");
  sprite.setTexture(spriteTexture);
  sprite.setPosition(Vector2f(0, 705.6));
  sprite.setScale(Vector2f(0.617, 0.45));
}

bool Gui::isOpen() { return window->isOpen(); }

void Gui::update() {
  pollEvent();
  updateMousePosition();
  updateClickAble();
  updateKeyboard();
  updatePressAble();
  updateMapCenter();
  updateZoomButton();
  updateLayerSheet();
  updateUpdateButton();
  updateLayerSystem();
  updateSelectedList();
  updateDetail();
  updateCommandList();
  updateLog();
}

void Gui::render() {
  window->clear(COLOR2);

  renderMap();
  renderZoomButton();
  renderLayerSheet();
  renderSelectedList();
  renderCommandList();
  renderLog();
  renderUpdateButton();
  renderAny();
  renderDetail();

  window->display();
}
