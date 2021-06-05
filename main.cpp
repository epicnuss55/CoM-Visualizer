#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>

int main() {
    sf::RectangleShape wall(sf::Vector2f(100,500));
    wall.setOutlineColor(sf::Color(150,150,150));
    wall.setOutlineThickness(1);
    wall.setFillColor(sf::Color(0,25,50));
    wall.setPosition(0,0);
    sf::FloatRect WallRect(-1,-1,102,502);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "error font not loaded" << std::endl;
    }

    int collisions = 0;
    sf::Text collisionCounter(std::to_string(collisions), font, 30);
    collisionCounter.setColor(sf::Color(255,255,255));
    collisionCounter.setPosition(110, 10);
    
    sf::Text NoMoreCollisions("No more Collisions!", font, 30);
    NoMoreCollisions.setColor(sf::Color(100,255,100));
    NoMoreCollisions.setPosition(110, 40);

    sf::RectangleShape S1(sf::Vector2f(50,50));
    S1.setPosition(300, 225);
    S1.setFillColor(sf::Color(0,0,0,1));
    S1.setOutlineThickness(2);
    S1.setOutlineColor(sf::Color(0,255,0));
    sf::FloatRect S1Rect(249,224,52,52);
    float S1V = -.1;
    float S1M = 1000000;

    sf::RectangleShape S2(sf::Vector2f(50,50));
    S2.setPosition(150.05, 225);
    S2.setFillColor(sf::Color(0,0,0,1));
    S2.setOutlineThickness(2);
    S2.setOutlineColor(sf::Color(0,255,0));
    sf::FloatRect S2Rect(349,224,52,52);
    float S2V = 0.0;
    float S2M = 1;

    S1.setPosition(300, 225);
    S2.setPosition(150.05, 225);
    S1Rect.left = 299;
    S1Rect.top = 224;
    S2Rect.left = 149.05;
    S2Rect.top = 224;
    int movedirection = 0;
    sf::RenderWindow window(sf::VideoMode(750, 500), "SFML works!", sf::Style::Close);
    window.setFramerateLimit(300);
    boolean opened = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    movedirection = 1;
                } else
                if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && (window.getView().getCenter().x > 380)) {
                    movedirection = -1;
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
                    movedirection = 0;
                }
                if ((event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) && (window.getView().getCenter().x > 380)) {
                    movedirection = 0;
                }
            }
        }

        switch (movedirection) {
        case 1:
            window.setView(sf::View(sf::Vector2f(window.getView().getCenter().x+1,250),sf::Vector2f(750,500)));
            break;
        case -1:
            if (window.getView().getCenter().x > 376)
                window.setView(sf::View(sf::Vector2f(window.getView().getCenter().x-1,250),sf::Vector2f(750,500)));
            break;
        case 0:
        default:
            break;
        }
        collisionCounter.setPosition(sf::Vector2f(window.getView().getCenter().x-window.getView().getSize().x/2+110,window.getView().getCenter().y-window.getSize().y/2+10));
        NoMoreCollisions.setPosition(sf::Vector2f(window.getView().getCenter().x-window.getView().getSize().x/2+110,window.getView().getCenter().y-window.getSize().y/2+40));

        if (S1.getPosition().x > 155) {
            S1.move(S1V,0);
        } else if (S1V > 0) {
            S1.move(S1V,0);
        }
        
        if (S2.getPosition().x > 100 && S2.getPosition().x+25 < S1.getPosition().x && S2V > -49 && S2V < 49) {
            S2.move(S2V,0);
        } else {
            if (S2V >= 0) {
                S2.setPosition(102,S2.getPosition().y);
                float S1VF = (((S1M - S2M)/(S1M+S2M))*S1V)+(((2*S2M)/(S1M+S2M))*S2V);
                float S2VF = (((2*S1M)/(S1M+S2M))*S1V)+(((S2M-S1M)/(S1M+S2M))*S2V);
                S1V = S1VF;
                S2V = S2VF;
                collisions = collisions+1;
            } else {
                S2V = -S2V;
                collisions = collisions+1;
            }
        }

        

        S1Rect.left = S1.getPosition().x-1;
        S1Rect.top = S1.getPosition().y-1;
        S2Rect.left = S2.getPosition().x-1;
        S2Rect.top = S2.getPosition().y-1;

        if (S1Rect.intersects(S2Rect)) {
            float S1VF = (((S1M - S2M)/(S1M+S2M))*S1V)+(((2*S2M)/(S1M+S2M))*S2V);
            float S2VF = (((2*S1M)/(S1M+S2M))*S1V)+(((S2M-S1M)/(S1M+S2M))*S2V);
            S1V = S1VF;
            S2V = S2VF;
            collisions = collisions+1;
        }

        if (WallRect.intersects(S2Rect)) {
            if (S2V < 0) {
                S2V = -S2V;
                collisions = collisions+1;
            }
        }

        collisionCounter.setString("collisions: " + std::to_string(collisions));
        
        window.clear();
        if (S1V > S2V && S1V > 0 && S2V >= 0) window.draw(NoMoreCollisions);
        window.draw(wall);
        window.draw(collisionCounter);
        window.draw(S1);
        window.draw(S2);
        window.display();
        if (!opened) {
            Sleep(500);
            opened = true;
        }
    }

    return 0;
}