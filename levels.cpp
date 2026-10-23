#include<SFML/Graphics.hpp>
#include<iostream>
#include <string>
#include "levels.hpp"
using namespace sf;
using namespace std;
#include "maze.hpp"


LevelNode*head=NULL;
void initializeLevels(){
    if(head!=NULL){
        return;
    }
    LevelNode* current=NULL;
    for(int i=1; i<=5; i++){
        LevelNode* newNode=new LevelNode;

        newNode->levelNumber=i;
        newNode->unlocked=(i==1);
        newNode->completed=false;
        newNode->next=NULL;

        if(head==NULL){
            head=newNode;
            current=newNode;
        }
        else{
            current->next=newNode;
            current=newNode;
        }
    }
}
bool isLevelUnlocked(int levelNumber){
    LevelNode* temp=head;

    while(temp!=NULL){
        if(temp->levelNumber==levelNumber){
            return temp->unlocked;
        }

        temp=temp->next;
    }
    return false;
}
bool isLevelCompleted(int levelNumber){
    LevelNode* temp=head;
    while(temp!=NULL){
        if(temp->levelNumber==levelNumber){
            return (temp->completed);
        }
        temp=temp->next;
    }
    return false;
}
void unlockNextLevel(int completeLevel){
    LevelNode* temp=head;

    while(temp!=NULL){
        if(temp->levelNumber==completeLevel){
            temp->completed=true;
            if(temp->next!=NULL){
                temp->next->unlocked=true;
            }
            return;
        }
        temp=temp->next;

    }

}



void LevelWindow(RenderWindow& window){
    initializeLevels();
    window.setFramerateLimit(60);

    Font font;
    if(font.openFromFile("assets/font.ttf")==false){
        return;
    }
    Text title(font, "Choose Your Level");
    title.setFillColor(Color(100,72,60));
    title.setStyle(Text::Bold);
    title.setPosition({205.f, 25.f});

    string levelnames[5]={"The Hawthorn Hedge", "The Siren's Cove", "Temple Of Apollo", "The Serpent's Lair", "The UnderWorld" };
    string difficulties[5]={"Easy", "Easy", "Medium", "Hard", "Very Hard"};
    string backgrounds[5]={"assets/level1bg.png","assets/level2bg.png","assets/level3bg.png","assets/level4bg.png","assets/level5bg.png"};

    RectangleShape back;
    back.setSize({150.f,45.f});
    back.setPosition({325.f,535.f});
    back.setFillColor(Color(100,72,60));

    Text Backtext(font, "BACK", 22);
    Backtext.setFillColor(Color::White);
    Backtext.setStyle(Text::Bold);
    Backtext.setPosition({370.f, 543.f});

    Text levelText(font,"", 23);
    levelText.setFillColor(Color(100,72,60));
    levelText.setStyle(Text::Bold);

    Text difficultyText(font,"", 17);
    difficultyText.setFillColor(Color(100,72,60));

    Text statusText(font,"",21);
    statusText.setFillColor(Color(100,72,60));
    statusText.setStyle(Text::Bold);

    while(window.isOpen()){
        while(const auto event = window.pollEvent()){
            if(event->is<Event::Closed>()){
                window.close();
            }
             if(const auto* mouseEvent=event->getIf<Event::MouseButtonPressed>()){
                if(mouseEvent->button==Mouse::Button::Left){
                    Vector2f mousePosition({
                        static_cast<float>(mouseEvent->position.x),
                        static_cast<float>(mouseEvent->position.y)
                    });

                    if(back.getGlobalBounds().contains(mousePosition)){
                        return;
                    }
                    else{
                        for(int i=0;i<5;i++){
                            RectangleShape card;
                            card.setSize({620.f, 72.f});
                            card.setPosition({90.f,95.f+i*82.f});

                            if(card.getGlobalBounds().contains(mousePosition)){
                                if(isLevelUnlocked(i+1)==false){
                                    break;
                                }
                                if(i==0){
                                    runmaze(window,8,8,45.f,false,false,backgrounds[i],1);
                                }else if(i==1){
                                    runmaze(window,10,10,36.f,true,false,backgrounds[i],2);
                                }else if(i==2){
                                    runmaze(window,12,12,30.f,true,false,backgrounds[i],3);
                                }else if(i==3){
                                    runmaze(window,15,15,24.f,false,true,backgrounds[i],4);
                                }else if(i==4){
                                    runmaze(window,20,20,18.f,true,true,backgrounds[i],5);
                                }
                                break;
                            }
                        } 
                    }
                }

        }
    }
    window.clear(Color(123, 160, 91));
    window.draw(title);
    for(int i=0; i<5; i++){
        RectangleShape card;
        card.setSize({620.f, 72.f});
        card.setPosition({90.f,95.f+i*82.f});

        if(isLevelCompleted(i+1)){
            card.setFillColor(Color(185,205,165));
        }
        else if(isLevelUnlocked(i+1)){
            card.setFillColor(Color(218, 199, 166));

        }
        else{
            card.setFillColor(Color(190, 183, 170));
        }

        if (isLevelCompleted(i+1)){
            statusText.setString("COMPLETED");
        }
        else if(isLevelUnlocked(i+1)){
            statusText.setString("UNLOCKED");
        }
        else{
            statusText.setString("LOCKED");
        }
        window.draw(card);

        levelText.setString("LEVEL "+to_string(i+1)+" - "+levelnames[i]);
        levelText.setPosition({110.f,103.f+i*82.f});
        window.draw(levelText);

        difficultyText.setString(difficulties[i]);
        difficultyText.setPosition({110.f,137.f+i*82.f});
        window.draw(difficultyText);

       
        statusText.setPosition({560.f,120.f+i*82.f});
        window.draw(statusText);

    }
    window.draw(back);
    window.draw(Backtext);
    window.display();
   

}
return;
}
