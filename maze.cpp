//g++ maze.cpp -o main.exe -lsfml-graphics -lsfml-window -lsfml-system
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <optional>
#include <ctime>
using namespace std;
using namespace sf;

const int MAX_ROWS=12;
const int MAX_COLS=12;
const float START_X=150.f;
const float START_Y=70.f;

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
void drawmaze(RenderWindow& window){
    RectangleShape cellbg;
    RectangleShape cellwall;
    RectangleShape player;

    const int thickness=3.f;
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
    float playerSize = activecellsize * 0.42f;
    float playerOffset = (activecellsize - playerSize) / 2.f;

    player.setSize({playerSize, playerSize});
    player.setFillColor(Color::Cyan);

    player.setPosition(
        Vector2f(
            START_X + playerCol * activecellsize + playerOffset,
            START_Y + playerRow * activecellsize + playerOffset
        )
    );

    window.draw(player);
}

void runmaze(RenderWindow& window,int rows,int cols,float cellsize){
    activecellsize = cellsize;
    generate_maze(rows, cols);
    addExtraPaths(rows, cols, 8);
    bool returnToMenu = false;
    while (window.isOpen() && !returnToMenu) {
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
            if (const auto* keyEvent =
                    event->getIf<Event::KeyPressed>()) {
                if (keyEvent->code == Keyboard::Key::Escape) {
                    returnToMenu = true;
                }
                else {
                    moveplayer(keyEvent->code);
                }
            }
        }
        if (!window.isOpen()) {
            break;
        }
        window.clear(Color(30, 30, 40));
        drawmaze(window);
        window.display();
    }
}
int main(){
    srand(static_cast<unsigned int>(time(nullptr)));
    RenderWindow window(
        VideoMode({800u, 600u}),
        "MAZE RUNNER"
    );
    window.setFramerateLimit(60);
    runmaze(window, 12, 12, 30.f);
    return 0;

}