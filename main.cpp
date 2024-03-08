#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/System/Vector2.hpp>
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "SFML/Window/Window.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include <SFML/Graphics/Text.hpp>
#include <string>
#include <vector>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Text Size Example");
    sf::Font font;

    if (!font.loadFromFile("C:/Users/ADMIN/Documents/GitHub/Cminusminus/Font/f.ttf")) {
        return EXIT_FAILURE;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24); // ขนาดตัวอักษร
    text.setString("X"); // ใช้ตัวอักษรเดียว
    text.setFillColor(sf::Color::Red); // สีของข้อความ

    sf::RectangleShape rec;
    rec.setSize(sf::Vector2f(24.f, 24.f));
    rec.setPosition(sf::Vector2f(0.f, 0.f));

    text.setPosition(sf::Vector2f(0.f, 0.f));

    while (window.isOpen()) {
        window.clear();
        window.draw(rec);
        window.draw(text);
        window.display();
    }

    return 0;
}

// int main()
// {
//     // สร้างหน้าต่าง SFML
//     sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Text Size Example");

//     // เตรียม font
//     sf::Font font;
//     if (!font.loadFromFile("C:/Users/ADMIN/Documents/GitHub/Cminusminus/Font/f.ttf")) {
//         return EXIT_FAILURE;
//     }

//     // สร้าง sf::Text
//     sf::Text text;
//     text.setFont(font);
//     text.setCharacterSize(24); // ขนาดตัวอักษร
//     text.setString("X"); // ใช้ตัวอักษรเดียว
//     text.setFillColor(sf::Color::Red); // สีของข้อความ

//     // หาขนาดของ text ในหน่วย pixel
//     sf::FloatRect textBounds = text.getLocalBounds();

//     // แสดงขนาดของ text ในหน่วย pixel
//     float textWidthInPixels = textBounds.width;
//     float textHeightInPixels = textBounds.height;
//     std::cout << "Width: " << textWidthInPixels << " Height: " << textHeightInPixels << std::endl;

//     // // Loop หลักของโปรแกรม
//     while (window.isOpen())
//     {
//         // ตรวจสอบเหตุการณ์
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         // Clear หน้าต่าง
//         window.clear();

//         // วาด text บนหน้าต่าง
//         window.draw(text);

//         // Display หน้าต่าง
//         window.display();
//     }

//     return 0;
// }