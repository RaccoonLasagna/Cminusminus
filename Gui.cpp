// #include "Gui.h"
// #include "SFML/Graphics/CircleShape.hpp"
// #include "SFML/Graphics/Color.hpp"
// #include "SFML/Graphics/Font.hpp"
// #include "SFML/Graphics/RectangleShape.hpp"
// #include "SFML/Graphics/RenderTarget.hpp"
// #include "SFML/Graphics/RenderTexture.hpp"
// #include "SFML/Graphics/RenderWindow.hpp"
// #include "SFML/Graphics/Text.hpp"
// #include "SFML/System/Vector2.hpp"
// #include "SFML/Window/Event.hpp"
// #include "SFML/Window/Keyboard.hpp"
// #include "SFML/Window/Mouse.hpp"
// #include "SFML/Window/VideoMode.hpp"
// #include "SFML/Window/Window.hpp"
// #include "SFML/Window/WindowStyle.hpp"
// #include <SFML/Graphics/Text.hpp>
// #include <iostream>
// #include <random>
// #include <sstream>

// using namespace sf;
// using namespace std;

// //---------------------------------Grid---------------------------------

// Grid::Grid(Vector2f size, Vector2f position, Font font, GameObject *object)
//     : object(object) {
//   shape.setSize(position);
//   shape.setPosition(position);
//   text.setFont(font);
//   /*คำนวน charector size เพื่อให้มันอยู่ตรงกลางเสมอ แล้วไป set charector size*/
//   text.setCharacterSize(8);
//   if (object) {
//     text.setString(object->getRepresent());
//   } else {
//     text.setString(" ");
//   }
//   /*คำนวนความหนาของขอบ shape และเซ็ตมัน*/
// }

// void Grid::draw(RenderWindow &window) {
//   window.draw(shape);
//   window.draw(text);
// }

// GameObject *Grid::getObject() { return object; }

// //---------------------------------Map---------------------------------

// void Map::update() {
//   gridMap.clear();
//   for (int i = 0; i < row; i++) {
//     vector<Grid> temp;
//     for (int j = 0; j < column; j++) {
//       Grid tempGrid(Vector2f((float)width / column, (float)height / row),
//                     Vector2f(border.getPosition().x + j * (float)width / column,
//                              border.getPosition().y + i * (float)height / row),
//                     font,
//                     dataMap[center.x - floor(column / 2) + i]
//                            [center.y - floor(row / 2) + j]);
//       temp.push_back(tempGrid);
//     }
//     gridMap.push_back(temp);
//   }
// }

// Map::Map(Vector2f size, Vector2f position, vector<vector<GameObject *>> dataMap)
//     : row(27), column(45), width(size.x), height(size.y), dataMap(dataMap),
//       center(Vector2i(23, 14))
// /*ความกว้างยาวมาจากอัตราส่วน 15:9*/ {
//   /*คำนวน border*/
//   border.setSize(Vector2f(width, height));
//   border.setFillColor(Color::Transparent);
//   border.setOutlineColor(Color::White);
//   border.setOutlineThickness(2.0f);
//   update();
// }

// void Map::draw(RenderWindow &window) {
//   window.draw(border);
//   for (int i = 0; i < row; i++) {
//     for (int j = 0; j < column; j++) {
//       gridMap[i][j].draw(window);
//     }
//   }
// }

// void Map::setDataMap(vector<vector<GameObject *>> dataMap) {
//   this->dataMap = dataMap;
//   update();
// }

// void Map::setRowColumn(Vector2i size) {
//   this->row = size.x;
//   this->column = size.y;
//   update();
// }

// GameObject *Map::getObject(Vector2f position) {
//   float gridHeight = (float)height / row;
//   float gridWidth = (float)width / column;
//   for (int i = 0; i < row; i++) {
//     if (position.y >= i * gridHeight && position.y < (i + 1) * gridHeight) {
//       for (int j = 0; j < column; j++) {
//         if (position.x >= j * gridWidth && position.x < (j + 1) * gridWidth) {
//           return gridMap[i][j].getObject();
//         }
//       }
//     }
//   }
//   return nullptr;
// }

// void Map::changeCenter(Vector2i center) {
//   this->center = center;
//   update();
// }

// Vector2i Map::getRowColumn() { return Vector2i(row, column); }
// Vector2i Map::getSize() { return Vector2i(width, height); }
// Vector2i Map::getCenter() { return center; }
// vector<vector<GameObject *>> Map::getDataMap() { return dataMap; }

// //---------------------------------ZoomIn---------------------------------

// ZoomIn::ZoomIn(Vector2f position, int radius, Font font) {
//   shape.setFillColor(Color::Green);
//   text.setString("+");

//   shape.setRadius(radius);
//   shape.setPosition(position);
//   text.setFont(font);
//   /*คำนวน position ให้มันอยู่ตรงกลาง*/
//   text.setPosition(position);
// }

// void ZoomIn::draw(RenderWindow &window) { window.draw(shape); }

// CircleShape ZoomIn::getShape() { return shape; }

// void ZoomIn::zoomIn(Map &map) {
//   Vector2i mapRowColumn = map.getRowColumn();
//   if (mapRowColumn.x - 30 < 15 || mapRowColumn.y - 18 < 9) {
//     return;
//   }
//   map.setRowColumn(Vector2i(mapRowColumn.x - 30, mapRowColumn.y - 18));
// }

// //---------------------------------ZoomOut---------------------------------

// ZoomOut::ZoomOut(Vector2f position, int radius, Font font)
//     : ZoomIn(position, radius, font) {
//   shape.setFillColor(Color::Red);
//   text.setString("-");
// }

// void ZoomOut::zoomOut(Map &map) {
//   Vector2i mapRowColumn = map.getRowColumn();
//   vector<vector<GameObject *>> dataMap = map.getDataMap();
//   if (mapRowColumn.x + 30 < dataMap[0].size() ||
//       mapRowColumn.y + 18 < dataMap.size()) {
//     return;
//   }
//   map.setRowColumn(Vector2i(mapRowColumn.x + 30, mapRowColumn.y + 18));
// }

// //---------------------------------LayerButton---------------------------------

// LayerButton::LayerButton(Vector2f position, Vector2f size, string text,
//                          vector<vector<GameObject *>> dataMap, Font font)
//     : dataMap(dataMap), font(font) {
//   button.setPosition(position);
//   button.setSize(size);
//   button.setFillColor(Color::Blue);
//   /*คำนวน size ของ text ตาม size ของ button*/
//   this->text.setString(text);
//   this->text.setCharacterSize(24);
//   this->text.setFillColor(Color::White);
//   this->text.setPosition(position.x - 10, position.y - 10);
// }

// void LayerButton::draw(RenderWindow &window) {
//   window.draw(button);
//   window.draw(text);
// }

// void LayerButton::setDataMap(vector<vector<GameObject *>> dataMap) {
//   this->dataMap = dataMap;
// }
// void LayerButton::pushToMap(Map &map) { map.setDataMap(dataMap); }

// //---------------------------------LayerSheet---------------------------------

// void LayerSheet::dumbToButton(vector<vector<GameObject *>> DataMap,
//                               LayerButton &button) {
//   button.setDataMap(DataMap);
// }

// LayerSheet::LayerSheet(LayerSystem *data, Vector2f position, Vector2f size,
//                        Font font)
//     : data(data), font(font) {
//   /*คำนวนขอบ*/
//   sheet.setPosition(position);
//   sheet.setSize(size);
//   sheet.setFillColor(Color::Transparent);
//   sheet.setOutlineColor(Color::White);
//   sheet.setOutlineThickness(2.0f);
//   float buttonWidth = size.x / ((data->layers.size() + 1) * (1 + 0.1));
//   /*overall layer*/
//   LayerButton tempButton(
//       Vector2f(position.x + (0 * (buttonWidth + (0.1 * buttonWidth))),
//                position.y),
//       Vector2f(buttonWidth, size.y), "Over all", data->getOverAllLayer(), font);
//   buttons.push_back(tempButton);
//   /*normnal layer*/
//   for (int i = 1; i < data->layers.size() + 1; i++) {
//     LayerButton tempButton(
//         Vector2f(position.x + (i * (buttonWidth + (0.1 * buttonWidth))),
//                  position.y),
//         Vector2f(buttonWidth, size.y), data->layers[i]->getName(),
//         data->layers[i]->insideLayer, font);
//     buttons.push_back(tempButton);
//   }
//   selectedbutton = &buttons[0];
// }

// void LayerSheet::draw(RenderWindow &window) {
//   window.draw(sheet);
//   for (int i = 0; i < buttons.size(); i++) {
//     buttons[i].draw(window);
//   }
// }

// void LayerSheet::setSelectedButton(int i) { selectedbutton = &buttons[i]; }

// void LayerSheet::update() {
//   data->update();
//   for (int i = 0; i < buttons.size(); i++) {
//     if (i == 0) {
//       dumbToButton(data->getOverAllLayer(), buttons[0]);
//       continue;
//     }
//     dumbToButton(data->layers[i]->insideLayer, buttons[i]);
//   }
// }

// //---------------------------------MiniMap---------------------------------

// // void MiniMap::changeMapCenter(Map &map, Vector2f position) {
// //   map.changeCenter(
// //       static_cast<int>((position.x - miniMap.getPosition().x) / sizePerGrid),
// //       static_cast<int>((position.y - miniMap.getPosition().y) /
// //       sizePerGrid));
// // }

// // void MiniMap::update() { // viewSize, viewCenter
// //   view.setSize(Vector2f(viewSize.first, viewSize.second));
// //   view.setPosition(
// //       Vector2f((viewCenter.first - floor(data->getColumn() / 2) *
// //       sizePerGrid),
// //                (viewCenter.second - floor(data->getRow() / 2)) *
// //                sizePerGrid));
// // }

// // MiniMap::MiniMap(Vector2f position, float scale, Map *data) {
// //   miniMap.setPosition(position);
// //   miniMap.setSize(
// //       Vector2f(data->getAllColumn() * scale, data->getAllRow() * scale));
// //   miniMap.setFillColor(Color::Black);
// //   miniMap.setOutlineColor(Color::White);
// //   miniMap.setOutlineThickness(2.0f);
// //   sizePerGrid = Vector2f(data->getWidth() * scale / data->getColumn(),
// //                          data->getHeight() * scale / data->getRow());
// //   view.setPosition(
// //       Vector2f((miniMapSize.first * viewCenter.first) / data.getColumn(),
// //                (miniMapSize.second * viewCenter.second) / data.getRow()));
// //   view.setSize(
// //       Vector2f(map.getColumn() * sizePerGrid, map.getRow() * sizePerGrid));
// //   view.setFillColor(Color::Transparent);
// //   view.setOutlineColor(Color::White);
// //   view.setOutlineThickness(2.0f);
// //   update();
// // }

// // void MiniMap::draw(RenderWindow &window) {
// //   window.draw(miniMap);
// //   window.draw(view);
// // }

// // void MiniMap::setViewSize(int x, int y) {
// //   viewSize = make_pair(x, y);
// //   update();
// // }

// // void MiniMap::setViewCenter(pair<int, int> center) {
// //   viewCenter = center;
// //   update();
// // }

// // int MiniMap::getSizePerGrid() { return sizePerGrid; }

// // // void MiniMap::changeMap(Map &map) {
// // //   view.setPosition(
// // //       Vector2f((miniMapSize.first * viewCenter.first) / data.getColumn(),
// // //                (miniMapSize.second * viewCenter.second) / data.getRow()));
// // // }

// // // MiniMap::MiniMap(Vector2f position, Vector2f size, Map *data) :
// // data(*data),
// // // {
// // //   int allColumn = data->getLayer()[0].size();
// // //   int allRow = data->getLayer().size();

// // //   minimap.setPosition(position);
// // //   minimap.setSize(size);
// // //   minimap.setFillColor(Color::black);
// // //   minimap.setOutlineColor(Color::White);
// // //   minimap.setOutlineThickness(2.0f);

// // //   view.setPosition(position);
// // //   view.setSize(Vector2f((size.x * data->getColumn()) / allColumn, (size.y
// // *
// // //   data->getRow()) / allRow)); view.setFillColor(Color::Transparent);
// // //   view.setOutlineColor(Color::White);
// // //   view.setOutlineThickness(2.0f);
// // // }

// //---------------------------------StopButton---------------------------------

// StopButton::StopButton(Vector2f position, Vector2f size) {
//   shape.setPosition(position);
//   shape.setSize(size);
//   shape.setFillColor(Color::Red);
// }

// void StopButton::draw(RenderWindow &window) { window.draw(shape); }

// RectangleShape StopButton::getShape() { return shape; }

// //---------------------------------PlayButton---------------------------------

// PlayButton::PlayButton(Vector2f position, Vector2f size)
//     : StopButton(position, size) {
//   shape.setPosition(position);
//   shape.setSize(size);
//   shape.setFillColor(Color::Green);
// }

// //---------------------------------RunButton---------------------------------

// RunButton::RunButton(Vector2f position, Vector2f size)
//     : StopButton(position, size) {
//   shape.setPosition(position);
//   shape.setSize(size);
//   shape.setFillColor(Color::Blue);
// }

// //---------------------------------TextBox---------------------------------

// TextBox::TextBox(Vector2f position, Vector2f size, Font font,
//                  vector<string> allText, int textCapacity)
//     : textCapacity(textCapacity), showTextIndex(0), font(font) {
//   shape.setPosition(position);
//   shape.setSize(size);
//   shape.setFillColor(Color::Black);
//   shape.setOutlineColor(Color::White);
//   shape.setOutlineThickness(2.0f);
//   for (int i = 0; i < textCapacity; i++) {
//     Text tempText;
//     tempText.setFont(font);
//     /*คำนวนขนาด text ตาม size ของ textBox*/
//     tempText.setCharacterSize(24);
//     tempText.setFillColor(Color::White);
//     tempText.setPosition(position.x + 10, position.y + 10 + (i * 30));
//     showText.push_back(tempText);
//   }
//   for (int i = 0; i < textCapacity; i++) {
//     showText[i].setString(allText[showTextIndex + i]);
//   }
// }

// void TextBox::draw(RenderWindow &window) {
//   window.draw(shape);
//   for (int i = 0; i < showText.size(); i++) {
//     window.draw(showText[i]);
//   }
// }

// void TextBox::dumbText(string text) {
//   allText.push_back(text);
//   if (allText.size() > textCapacity) {
//     allText.erase(allText.begin());
//   }
//   if (showTextIndex < allText.size() - textCapacity) {
//     showTextIndex = allText.size() - textCapacity;
//   }
// }

// RectangleShape TextBox::getShape() { return shape; }

// void TextBox::shiftShowTextUp() {
//   if (showTextIndex > 0) {
//     showTextIndex--;
//     for (int i = 0; i < textCapacity; i++) {
//       showText[i].setString(allText[showTextIndex + i]);
//     }
//   }
// }

// void TextBox::shiftShowTextDown() {
//   if (showTextIndex < allText.size() - textCapacity) {
//     showTextIndex++;
//     for (int i = 0; i < textCapacity; i++) {
//       showText[i].setString(allText[showTextIndex + i]);
//     }
//   }
// }

// //---------------------------------Detail---------------------------------

// void Detail::update() {
//   if (object) {
//     // เอาข้อมูลของสัตว์มาแสดง
//   }
// }

// Detail::Detail(Vector2f position, Vector2f size, Font font)
//     : TextBox(position, size, font, {}, 5) {
//   deleteButton.setRadius(10);
//   deleteButton.setFillColor(Color::Red);
//   deleteButton.setPosition(
//       Vector2f(shape.getPosition().x + shape.getSize().x - 30,
//                shape.getPosition().y + 10));
// }

// void Detail::setObject(GameObject *object) {
//   this->object = object;
//   update();
// }

// CircleShape Detail::getDeleteButton() { return deleteButton; }

// //---------------------------------Log---------------------------------

// void Log::update() {
//   // ไปดึงข้อมูลการกระทำของสัตว์มาแสดง
// }

// Log::Log(Vector2f position, Vector2f size, Font font)
//     : TextBox(position, size, font, {}, 5) {
//   DefaultHeight = position.y;
// }

// void Log::changeHeight(float height) {
//   shape.setSize(Vector2f(shape.getSize().x, height - DefaultHeight));
//   for (int i = 0; i < showText.size(); i++) {
//     showText[i].setPosition(showText[i].getPosition().x,
//                             height - 20 - (i * 30));
//   }
// }

// //---------------------------------SelectedList---------------------------------

// SelectedList::SelectedList(Vector2f position, Vector2f size, Font font,
//                            vector<GameObject *> allObject, int textCapacity)
//     : textCapacity(textCapacity), showTextIndex(0), font(font) {
//   shape.setPosition(position);
//   shape.setSize(size);
//   shape.setFillColor(Color::Black);
//   shape.setOutlineColor(Color::White);
//   shape.setOutlineThickness(2.0f);
//   for (int i = 0; i < textCapacity; i++) {
//     Text tempText;
//     tempText.setFont(font);
//     /*คำนวนขนาด text ตาม size ของ textBox*/
//     tempText.setCharacterSize(24);
//     tempText.setFillColor(Color::White);
//     tempText.setPosition(position.x + 10, position.y + 10 + (i * 30));
//     showText.push_back(tempText);
//   }
//   for (int i = 0; i < textCapacity; i++) {
//     if (i < allObject.size()) {
//       showText[i].setString(allObject[i]->getRepresent());
//     } else {
//       showText[i].setString(" ");
//     }
//   }
// }

// void SelectedList::draw(RenderWindow &window) {
//   window.draw(shape);
//   for (int i = 0; i < showText.size(); i++) {
//     window.draw(showText[i]);
//   }
// }

// void SelectedList::dumbObject(GameObject *object) {
//   allObject.push_back(object);
//   if (allObject.size() > textCapacity) {
//     allObject.erase(allObject.begin());
//   }
//   if (showTextIndex < allObject.size() - textCapacity) {
//     showTextIndex = allObject.size() - textCapacity;
//   }
// }

// RectangleShape SelectedList::getShape() { return shape; }

// void SelectedList::shiftShowTextUp() {
//   if (showTextIndex > 0) {
//     showTextIndex--;
//     for (int i = 0; i < textCapacity; i++) {
//       if (showTextIndex + i < allObject.size()) {
//         showText[i].setString(allObject[showTextIndex + i]->getRepresent());
//       } else {
//         showText[i].setString(" ");
//       }
//     }
//   }
// }

// void SelectedList::shiftShowTextDown() {
//   if (showTextIndex < allObject.size() - textCapacity) {
//     showTextIndex++;
//     for (int i = 0; i < textCapacity; i++) {
//       if (showTextIndex + i < allObject.size()) {
//         showText[i].setString(allObject[showTextIndex + i]->getRepresent());
//       } else {
//         showText[i].setString(" ");
//       }
//     }
//   }
// }

// GameObject *SelectedList::click(Vector2f position) {
//   for (int i = 0; i < showText.size(); i++) {
//     if (position.x >= showText[i].getPosition().x &&
//         position.x <=
//             showText[i].getPosition().x + showText[i].getLocalBounds().width &&
//         position.y >= showText[i].getPosition().y &&
//         position.y <=
//             showText[i].getPosition().y + showText[i].getLocalBounds().height) {
//       if (showTextIndex + i < allObject.size()) {
//         return allObject[showTextIndex + i];
//       }
//     }
//   }
//   return nullptr;
// }

// //---------------------------------CommandList---------------------------------

// void CommandList::update() {
//   for (int i = 0; i < showText.size(); i++) {
//     if (i < allText.size()) {
//       showText[i].setString(allText[i]);
//     } else {
//       showText[i].setString(" ");
//     }
//   }
// }

// CommandList::CommandList(Vector2f position, Vector2f size, Font font)
//     : TextBox(position, size, font, {}, 5) {
//   textCapacity = 5;
//   showTextIndex = 0;
//   this->font = font;
//   shape.setPosition(position);
//   shape.setSize(size);
//   shape.setFillColor(Color::Black);
//   shape.setOutlineColor(Color::White);
//   shape.setOutlineThickness(2.0f);
//   for (int i = 0; i < textCapacity; i++) {
//     Text tempText;
//     tempText.setFont(font);
//     /*คำนวนขนาด text ตาม size ของ textBox*/
//     tempText.setCharacterSize(24);
//     tempText.setFillColor(Color::White);
//     tempText.setPosition(position.x + 10, position.y + 10 + (i * 30));
//     showText.push_back(tempText);
//   }
// }

// string CommandList::click(Vector2f position) {
//   for (int i = 0; i < showText.size(); i++) {
//     if (position.x >= showText[i].getPosition().x &&
//         position.x <=
//             showText[i].getPosition().x + showText[i].getLocalBounds().width &&
//         position.y >= showText[i].getPosition().y &&
//         position.y <=
//             showText[i].getPosition().y + showText[i].getLocalBounds().height) {
//       if (showTextIndex + i < allText.size()) {
//         return allText[showTextIndex + i];
//       }
//     }
//   }
//   return "";
// }

// //---------------------------------Gui---------------------------------

// Gui::Gui() {
//   font->loadFromFile("C:/Users/ADMIN/Documents/GitHub/Cminusminus/Font/f.ttf");
//   vector<vector<GameObject *>> dataMap;
//   for (int i = 0; i < 27; i++) {
//     vector<GameObject *> temp;
//     for (int j = 0; j < 45; j++) {
//       temp.push_back(nullptr);
//     }
//     dataMap.push_back(temp);
//   }
//   map = new Map(Vector2f(1350, 810), Vector2f(0, 0), dataMap);
//   zoomIn = new ZoomIn(Vector2f(1350, 0), 50, *font);
//   zoomOut = new ZoomOut(Vector2f(1350, 50), 50, *font);
//   layerSheet = new LayerSheet(new LayerSystem(255, 153), Vector2f(1350, 100),
//                               Vector2f(150, 810), *font);
//   // miniMap = new MiniMap(Vector2f(1350, 0), 0.1, map);
//   stopButton = new StopButton(Vector2f(1350, 0), Vector2f(50, 50));
//   playButton = new PlayButton(Vector2f(1350, 50), Vector2f(50, 50));
//   runButton = new RunButton(Vector2f(1350, 100), Vector2f(50, 50));
//   detail = new Detail(Vector2f(1350, 350), Vector2f(150, 200), *font);
//   log = new Log(Vector2f(1350, 550), Vector2f(150, 200), *font);
//   selectedList = new SelectedList(Vector2f(1350, 750), Vector2f(150, 60), *font,
//                                   {}, 2);
//   commandList = new CommandList(Vector2f(1350, 810), Vector2f(150, 150), *font);
// }

// void Gui::draw() {
//   map->draw(*window);
//   zoomIn->draw(*window);
//   zoomOut->draw(*window);
//   layerSheet->draw(*window);
//   // miniMap->draw(*window);
//   stopButton->draw(*window);
//   playButton->draw(*window);
//   runButton->draw(*window);
//   detail->draw(*window);
//   log->draw(*window);
//   selectedList->draw(*window);
//   commandList->draw(*window);
// }

// void Gui::poll() {
//   Event event;
//   while (window->pollEvent(event)) {
//     if (event.type == Event::Closed) {
//       window->close();
//     }
//     if (event.type == Event::MouseButtonPressed) {
//       if (event.mouseButton.button == Mouse::Left) {
//         Vector2f position = window->mapPixelToCoords(Mouse::getPosition(*window));
//         if (zoomIn->getShape().getGlobalBounds().contains(position)) {
//           zoomIn->zoomIn(*map);
//         }
//         if (zoomOut->getShape().getGlobalBounds().contains(position)) {
//           zoomOut->zoomOut(*map);
//         }
//         if (stopButton->getShape().getGlobalBounds().contains(position)) {
//           // หยุดการทำงาน
//         }
//         if (playButton->getShape().getGlobalBounds().contains(position)) {
//           // เริ่มการทำงาน
//         }
//         if (runButton->getShape().getGlobalBounds().contains(position)) {
//           // ทำงานเร็ว
//         }
//         if (detail->getDeleteButton().getGlobalBounds().contains(position)) {
//           // ลบสัตว์
//         }
//         if (selectedList->getShape().getGlobalBounds().contains(position)) {
//           GameObject *object = selectedList->click(position);
//           if (object) {
//             detail->setObject(object);
//           }
//         }
//         if (commandList->getShape().getGlobalBounds().contains(position)) {
//           string command = commandList->click(position);
//           if (command != "") {
//             // ทำคำสั่ง
//           }
//         }
//       }
//     }
//   }
// }

// void Gui::update() {
//   layerSheet->update();
//   // miniMap->changeMap(*map);
//   // log->update();
//   // detail->update();
// }