//g++ maze.cpp -o main.exe -lsfml-graphics -lsfml-window -lsfml-system
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <optional>
#include <string>
#include <ctime>
#include <SFML/Audio.hpp>
#include "levels.hpp"
using namespace std;
using namespace sf;

const int MAX_ROWS=20;
const int MAX_COLS=20;
const float START_X=150.f;
const float START_Y=70.f;
const int MAX_MONSTERS=5;
 int monsterRows[MAX_MONSTERS];
 int monsterCols[MAX_MONSTERS];
 int monsterCount=0;
 bool monstersEnabled=false;


struct cell{
    int rows;
    int cols;
    bool visited;
    bool walltop;
    bool wallright;
    bool wallbottom;
    bool wallleft;
};

cell grid[MAX_ROWS][MAX_COLS];
int stackRows[MAX_ROWS*MAX_COLS];
int stackCols[MAX_ROWS*MAX_COLS];
int top=-1;

int activeRows;
int activeCols;
float activecellsize;

int playerRow=0;
int playerCol=0;
bool won=false;

void push(int r,int c){
    top++;
    stackRows[top]=r;
    stackCols[top]=c;
}

void pop(){
    top--;
}

void generate_maze(int rows, int cols){
    activeRows=rows;
    activeCols=cols;

    top=-1;
    playerRow=0;
    playerCol=0;
    won=false;

    for(int i=0;i<activeRows;i++){
        for(int j=0;j<activeCols;j++){
            grid[i][j].rows=i;
            grid[i][j].cols=j;
            grid[i][j].visited=false;
            grid[i][j].wallbottom=true;
            grid[i][j].wallleft=true;
            grid[i][j].wallright=true;
            grid[i][j].walltop=true;
        }
    }

//DFS
    int currentrow=0;
    int currentcol=0;
    grid[currentrow][currentcol].visited=true;
    push(currentrow,currentcol);

    while(top!=-1){
        currentrow=stackRows[top];
        currentcol=stackCols[top];
        int directions[4];
        int neighbourRow[4];
        int neighbourCol[4];
        int count=0;

        //up (0)
        if(currentrow>0 && grid[currentrow-1][currentcol].visited==false){
            directions[count]=0;
            neighbourRow[count]=currentrow-1;
            neighbourCol[count]=currentcol;
            count++;
        }
        //right(1)
        if(currentcol<activeCols-1 && grid[currentrow][currentcol+1].visited==false){
            
            directions[count]=1;
            neighbourRow[count]=currentrow;
            neighbourCol[count]=currentcol+1;
            count++;
        }
        //down(2)
        if(currentrow<activeRows-1 && grid[currentrow+1][currentcol].visited==false){
            
            directions[count]=2;
            neighbourRow[count]=currentrow+1;
            neighbourCol[count]=currentcol;
            count++;
        }
        //left(3)
        if(currentcol>0 && grid[currentrow][currentcol-1].visited==false){
            
            directions[count]=3;
            neighbourRow[count]=currentrow;
            neighbourCol[count]=currentcol-1;
            count++;
        }

        if(count==0){
            pop();
        }
        else{
            int choice=rand()%count;
            int nextRow=neighbourRow[choice];
            int nextCol=neighbourCol[choice];
            int direction=directions[choice];

            //Making way
            //up(0)
            if(direction==0){
                grid[currentrow][currentcol].walltop=false;
                grid[nextRow][nextCol].wallbottom=false;
            }//right(1)
            else if(direction==1){
                grid[currentrow][currentcol].wallright=false;
                grid[nextRow][nextCol].wallleft=false;
            }//down(2)
            else if(direction==2){
                grid[currentrow][currentcol].wallbottom=false;
                grid[nextRow][nextCol].walltop=false;
            }//left
            else{
                grid[currentrow][currentcol].wallleft=false;
                grid[nextRow][nextCol].wallright=false;
            }
            grid[nextRow][nextCol].visited=true;
            push(nextRow,nextCol);
        }  
    }
    //enrance and exit
    grid[0][0].walltop=false;
    grid[activeRows-1][activeCols-1].wallbottom=false;

}

void addExtraPaths(int rows, int cols, int extraPaths) {
    for (int i = 0; i < extraPaths; i++) {
        // Select a random cell
        int row = rand() % rows;
        int col = rand() % cols;
        // Select a random direction
        int direction = rand() % 4;

        // Right: remove shared wall with right neighbour
        if (direction == 1 && col<cols-1) {
            grid[row][col].wallright = false;
            grid[row][col + 1].wallleft = false;
        }
        // Down: remove shared wall with lower neighbour
        else if (direction == 2 && row < rows - 1) {
            grid[row][col].wallbottom = false;
            grid[row + 1][col].walltop = false;
        }
        // Left: remove shared wall with left neighbour
        else if (direction == 3 && col > 0) {
            grid[row][col].wallleft = false;
            grid[row][col - 1].wallright = false;
        }
        // Up: remove shared wall with upper neighbour
        else if (direction == 0 && row > 0) {
            grid[row][col].walltop = false;
            grid[row - 1][col].wallbottom = false;
        }
    }
}


//moving player using keyboard
void moveplayer(Keyboard::Key k){
    if(won==true) return;
    if(k==Keyboard::Key::Up && grid[playerRow][playerCol].walltop==false && playerRow>0){
       playerRow--;
    }
    else if(k==Keyboard::Key::Right && grid[playerRow][playerCol].wallright==false && playerCol<activeCols-1){
       playerCol++;
    }
    else if(k==Keyboard::Key::Down && grid[playerRow][playerCol].wallbottom==false && playerRow<activeRows-1){
       playerRow++;
    }else if(k==Keyboard::Key::Left && grid[playerRow][playerCol].wallleft==false && playerCol>0){
       playerCol--;
    }

    if(playerRow==activeRows-1 && playerCol==activeCols-1){
        won=true;
         cout << "Congratulations! You reached the exit!"
             << endl;
    }
}
void drawmaze(RenderWindow& window, Sprite& playerSprite){
    RectangleShape cellbg;
    RectangleShape cellwall;
    // RectangleShape player;

    float  thickness=activecellsize*0.06f;

    if(thickness<1.f){
        thickness=1.f;
    }
    cellbg.setSize({activecellsize-thickness,activecellsize-thickness});
    cellbg.setFillColor(Color(40, 100, 180));
    cellbg.setPosition({START_X+(activeCols-1)*activecellsize,START_Y+(activeRows-1)*activecellsize});
    window.draw(cellbg);

    cellwall.setFillColor(Color::White);
    for(int i=0;i<activeRows;i++){
        for(int j=0;j<activeCols;j++){
            float x=START_X + j*activecellsize;
            float y=START_Y + i*activecellsize;

            if(grid[i][j].walltop==true){
                cellwall.setSize({activecellsize,thickness});
                cellwall.setPosition({x,y});
                window.draw(cellwall);
            }

            if(grid[i][j].wallbottom==true){
                cellwall.setSize({activecellsize,thickness});
                cellwall.setPosition({x,y+activecellsize-thickness});
                window.draw(cellwall);  
            }
            if(grid[i][j].wallright==true){
                cellwall.setSize({thickness,activecellsize});
                cellwall.setPosition({x+activecellsize-thickness,y});
                window.draw(cellwall);  
            }
            if(grid[i][j].wallleft==true){
                cellwall.setSize({thickness,activecellsize});
                cellwall.setPosition({x,y});
                window.draw(cellwall);  
            }
        }
    }
      // Draw player, sized according to the current cell
    float playerSize = activecellsize * 1.0f;
    Vector2u imageSize=playerSprite.getTexture().getSize();
    playerSprite.setScale({playerSize/imageSize.x, playerSize/imageSize.y});
    
    float playerOffset=(activecellsize-playerSize)/2.f;

    playerSprite.setPosition({
        START_X + playerCol*activecellsize + playerOffset,
        START_Y + playerRow*activecellsize + playerOffset
    });
    window.draw(playerSprite);

}
bool safePath[MAX_ROWS][MAX_COLS];

void findSafePath()
{
    bool visited[MAX_ROWS][MAX_COLS] = {};

    int parentRow[MAX_ROWS][MAX_COLS];
    int parentCol[MAX_ROWS][MAX_COLS];

    int queueRows[MAX_ROWS * MAX_COLS];
    int queueCols[MAX_ROWS * MAX_COLS];

    for (int i = 0; i < activeRows; i++) {
        for (int j = 0; j < activeCols; j++) {
            safePath[i][j] = false;
            parentRow[i][j] = -1;
            parentCol[i][j] = -1;
        }
    }

    int front = 0;
    int rear = 0;

    queueRows[rear] = 0;
    queueCols[rear] = 0;
    rear++;

    visited[0][0] = true;

    while (front < rear) {
        int row = queueRows[front];
        int col = queueCols[front];
        front++;

        if (row == activeRows - 1 &&
            col == activeCols - 1) {
            break;
        }

        // Up
        if (row > 0 &&
            !grid[row][col].walltop &&
            !visited[row - 1][col]) {

            visited[row - 1][col] = true;
            parentRow[row - 1][col] = row;
            parentCol[row - 1][col] = col;

            queueRows[rear] = row - 1;
            queueCols[rear] = col;
            rear++;
        }

        // Right
        if (col < activeCols - 1 &&
            !grid[row][col].wallright &&
            !visited[row][col + 1]) {

            visited[row][col + 1] = true;
            parentRow[row][col + 1] = row;
            parentCol[row][col + 1] = col;

            queueRows[rear] = row;
            queueCols[rear] = col + 1;
            rear++;
        }

        // Down
        if (row < activeRows - 1 &&
            !grid[row][col].wallbottom &&
            !visited[row + 1][col]) {

            visited[row + 1][col] = true;
            parentRow[row + 1][col] = row;
            parentCol[row + 1][col] = col;

            queueRows[rear] = row + 1;
            queueCols[rear] = col;
            rear++;
        }

        // Left
        if (col > 0 &&
            !grid[row][col].wallleft &&
            !visited[row][col - 1]) {

            visited[row][col - 1] = true;
            parentRow[row][col - 1] = row;
            parentCol[row][col - 1] = col;

            queueRows[rear] = row;
            queueCols[rear] = col - 1;
            rear++;
        }
    }

    // Exit se wapas start tak path mark karo
    int row = activeRows - 1;
    int col = activeCols - 1;

    if (!visited[row][col]) {
        return;
    }

    while (row != 0 || col != 0) {
        safePath[row][col] = true;

        int previousRow = parentRow[row][col];
        int previousCol = parentCol[row][col];

        row = previousRow;
        col = previousCol;
    }

    safePath[0][0] = true;
}
// void setupMonsters(int count){
//     monsterCount=0;
//     if(monstersEnabled==false){
//         return;
//     }
//     while(monsterCount<count){
//         int row=rand()%activeRows;
//         int col=rand()%activeCols;

//         if(row==0 && col==0 || row==activeRows-1 && col==activeCols-1 ){
//             continue;
//         }
//         bool occupied=false;
//         for(int i=0; i<monsterCount; i++){
//             if(monsterRows[i]==row && monsterCols[i]==col){
//                 occupied=true;
//                 break;
//             }
//         }
//         if(occupied==false){
//             monsterRows[monsterCount]=row;
//             monsterCols[monsterCount]=col;
//             monsterCount++;
//         }

//     }
// }

//gpt
void setupMonsters(int count)
{
    monsterCount = 0;

    if (monstersEnabled == false) {
        return;
    }

    int attempts = 0;
    int maxAttempts = activeRows * activeCols * 20;

    while (monsterCount < count &&
           monsterCount < MAX_MONSTERS &&
           attempts < maxAttempts) {

        attempts++;

        int row = rand() % activeRows;
        int col = rand() % activeCols;

        // Safe path par monster nahi hoga
        if (safePath[row][col]) {
            continue;
        }

        bool occupied = false;

        for (int i = 0; i < monsterCount; i++) {
            int rowDifference = abs(monsterRows[i] - row);
            int colDifference = abs(monsterCols[i] - col);

            // Same cell ya adjacent cell
            if (rowDifference + colDifference <= 1) {
                occupied = true;
                break;
            }
        }

        if (occupied == false) {
            monsterRows[monsterCount] = row;
            monsterCols[monsterCount] = col;
            monsterCount++;
        }
    }
}
void drawMonsters(RenderWindow& window, Sprite& monsterSprite){
    if(monstersEnabled==false){
        return;
    }
    float monsterSize=activecellsize*1.0f;
    Vector2u imageSize = monsterSprite.getTexture().getSize();
    monsterSprite.setScale({monsterSize/imageSize.x,monsterSize/imageSize.y});
    // CircleShape monster(activecellsize*0.25f);
    // monster.setFillColor(Color::Red);
    for(int i=0; i<monsterCount;i++){
        float x=START_X + monsterCols[i]*activecellsize;
        float y=START_Y + monsterRows[i]*activecellsize;

        monsterSprite.setPosition({
            x+(activecellsize-monsterSize)/2.f,
            y+(activecellsize-monsterSize)/2.f
        });
        window.draw(monsterSprite);
    
    }
}

void checkMonsterCollision(){
    if(monstersEnabled==false){
        return;
    }
    for(int i=0; i<monsterCount; i++){
        if(playerRow==monsterRows[i] && playerCol==monsterCols[i]){
            playerRow=0;
            playerCol=0;
            cout<<"Monster Caught You! Restart"<<endl;
            
        }
    }

    return;
}
void runmaze(RenderWindow& window,int rows,int cols,float cellsize, bool timerenabled, bool enableMonsters, const string& BackgroundPath, int level){
    activecellsize = cellsize;
    Texture backgroundTexture;
    Texture playerTexture;


    if(backgroundTexture.loadFromFile(BackgroundPath)==false){
        cout<<"background img load nhi hui";
        return;
    }
    Sprite backgroundSprite(backgroundTexture);
    Vector2u windowSize=window.getSize();
    Vector2u imageSize=backgroundTexture.getSize();

    backgroundSprite.setScale({static_cast<float>(windowSize.x)/imageSize.x,
                             static_cast<float>(windowSize.y)/imageSize.y
    });



    generate_maze(rows, cols);
    addExtraPaths(rows, cols, 8);


    monstersEnabled=enableMonsters;
    findSafePath();
    setupMonsters(2);
    
    Clock timer;
    int timelimit=60;
    Font font;
    if(font.openFromFile("assets/font.ttf")==false){
        cout<<"Not load."<<endl;
        return;
    }
    Text timerText(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(Color::White);
    timerText.setPosition({20.f, 20.f});

    Music backgroundMusic;

    string musicFile;

    if (level == 1) {
        musicFile = "assets/musicl1.ogg";
    }
    else if (level == 2) {
        musicFile = "assets/musicl2.ogg";
    }
    else if (level == 3) {
        musicFile = "assets/musicl3.ogg";
    }
    else if (level == 4) {
        musicFile = "assets/musicl4.ogg";
    }
    else if (level == 5) {
        musicFile = "assets/musicl5.ogg";
    }

    if (backgroundMusic.openFromFile(musicFile)) {
        backgroundMusic.setVolume(35.f);
        backgroundMusic.setLooping(true);
        backgroundMusic.play();
    }
    else {
        cout << "Could not load: " << musicFile << endl;
    }

    Text congratsText(font);
    congratsText.setString("CONGRATULATIONS!");
    congratsText.setCharacterSize(38);
    congratsText.setFillColor(Color::Yellow);
    congratsText.setPosition({200.f, 200.f});

    Text completedText(font);
    completedText.setString(
        "You completed Level " + to_string(level) + "!"
    );
    completedText.setCharacterSize(25);
    completedText.setFillColor(Color::White);
    completedText.setPosition({220.f, 270.f});

    Text continueText(font);
    continueText.setString("Press ENTER to return to levels");
    continueText.setCharacterSize(20);
    continueText.setFillColor(Color::White);
    continueText.setPosition({210.f, 330.f});


    bool returnToMenu = false;

    if(playerTexture.loadFromFile("assets/player.png")==false){
        cout<<"player load nhi hui"<<endl;
        return;
    }
    Sprite playerSprite(playerTexture);
    
    Texture snakeTexture;
    Texture skeletonTexture;

    if(snakeTexture.loadFromFile("assets/snake.png")==false){
        cout<<"snake nhi horaha load"<<endl;
        return;
    }
     if(skeletonTexture.loadFromFile("assets/skeleton.png")==false){
        cout<<"skeleton nhi horaha load"<<endl;
        return;
    }
    Sprite snakeSprite(snakeTexture);
    Sprite skeletonSprite(skeletonTexture);


    while (window.isOpen() && !returnToMenu) {
        
        if (!window.isOpen()) {
            break;
        }
        if(timerenabled==true && won==false){
            int remainingtime=timelimit-static_cast<int>(timer.getElapsedTime().asSeconds());
        
            if(remainingtime<0){
                remainingtime=0;
            }
    
            if(won==true){      
            }
            else{
                timerText.setString(
                    "Time: "+to_string(remainingtime)
                );
                if(remainingtime==0 && won==false){
                    cout<<"GAME OVER!!"<<endl;
                    returnToMenu=true;
                    break;
                }
            }
        }
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
            if (const auto* keyEvent =
                    event->getIf<Event::KeyPressed>()) {
                if (keyEvent->code == Keyboard::Key::Escape) {
                    returnToMenu = true;
                }

                else if(won==true){
                    if(keyEvent->code==Keyboard::Key::Enter){
                        returnToMenu=true;
                    }
                }
                else {
                    moveplayer(keyEvent->code);
                    checkMonsterCollision();

                    if(won==true){
                        unlockNextLevel(level);
                    }
                }
            }
        }
        window.clear();
        window.draw(backgroundSprite);
        drawmaze(window, playerSprite);
        if(level==4){
        drawMonsters(window,snakeSprite);
        }
        else if(level==5){
        drawMonsters(window,skeletonSprite);
        }
        if(timerenabled==true){
            window.draw(timerText);
        }

        if(won==true){
        RectangleShape overlay(
            Vector2f(
                static_cast<float>(windowSize.x),
                static_cast<float>(windowSize.y))
        );
    
                overlay.setFillColor(Color(0,0,0,210));
                window.draw(overlay);
                window.draw(congratsText);
                window.draw(completedText);
                window.draw(continueText);
            }
        
        window.display();
    }
}
