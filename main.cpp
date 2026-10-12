//g++ main.cpp -o output.exe -lsfml-graphics -lsfml-window -lsfml-system
#include<SFML/Graphics.hpp>
#include<iostream>
using namespace sf;
using namespace std;
int main(){
    RenderWindow window (VideoMode({800,600}),"Maze Game");
    window.setFramerateLimit(60);

    Texture backgroundtexture;
    if(backgroundtexture.loadFromFile("assets/WelcomeScreenBG.png")==false){
        return 1;
    }
 

    Sprite background(backgroundtexture);
    background.setPosition({0.f, 0.f});
    background.setScale({
        800.f/backgroundtexture.getSize().x,
        600.f/backgroundtexture.getSize().y
     });


    Font font;
    if(font.openFromFile("assets/font.ttf")==false){
        return 1;
    }
    

    Text title(font,"MAZE RUNNER");
    title.setCharacterSize(50);
    title.setFillColor(Color(100,72,60));
    title.setPosition({230.f,90.f});

    Text starttext(font,"START");
    starttext.setCharacterSize(30);
    starttext.setFillColor(Color::White);
    starttext.setPosition({350.f,230.f});
    starttext.setStyle(Text::Bold);

    Text howtoplaytext(font,"HOW TO PLAY");
    howtoplaytext.setCharacterSize(30);
    howtoplaytext.setFillColor(Color::White);
    howtoplaytext.setPosition({310.f,320.f});
    howtoplaytext.setStyle(Text::Bold);


    RectangleShape start;
    start.setSize({200,60});
    start.setPosition({300,220});
    start.setFillColor(Color(100,72,60));

    RectangleShape howtoplay;
    howtoplay.setSize({230,60});
    howtoplay.setPosition({290,310});
    howtoplay.setFillColor(Color(100,72,60));



    while(window.isOpen()){
         while(const optional event = window.pollEvent()){
            if(event->is<Event::Closed>()){
                window.close();
            }
         }
         window.clear(Color::Black);
         window.draw(background);
         window.draw(title);
         
         window.draw(start);
         window.draw(howtoplay);
         window.draw(starttext);
         window.draw(howtoplaytext);
         window.display();
    }
    return 0;

}