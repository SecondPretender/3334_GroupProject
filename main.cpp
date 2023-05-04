/*
 * Authors: Peter McGuinness, Alex DeVries, Xavier Lopez,
 *          Derek Faucett, Christian Ocana
 * Assignment Title: Jigsaw Puzzle
 * Assignment Description: This program creates a jigsaw puzzle game by using
 * SDLPlotter to implement the game's behavior
 * Due Date: 5/4/2023
 * Date Created: 4/16/2023
 * Date Last Modified: 5/3/2023
 * Filename: main.cpp
 *
 *
 * Changelog:
 * 4/16/2023 created by Peter McGuinness
 * 4/23/2023 added pieceLinker and corresponding bits(did not work)
 * 4/25/2023 started making clickable functionality: Peter McGuinness
 * 4/27/2023 made PcShell functionality (snap to final place)
 * 4/30/2023 added piece shuffling, new image: Peter McGuinness, Alex DeVries
 * 4/30/2023 added piece rotating, vic state: Peter McGuinness, Alex DeVries
 * 5/01/2023 added a way to generate a random seed for each starting picture
 *           when the game launches: Christian Ocana
 * 5/03/2023 created detailed comments in the file: Christian Ocana
 *
 * Assumptions: It is assumed that the in-game pictures are the only pictures
 * the user can have access to. It is also assumed that the user can restart
 * the game by exiting out of the tab, even if they have not won yet.
*/
#include <iostream>
#include <cmath>
#include <fstream>
#include "SDL_Plotter.h"
#include "Piece.h"


using namespace std;

void printAll(Piece* e, SDL_Plotter& g);
void winThis(SDL_Plotter& g, string fileName);
static int PIECECNT = 25;

int main(int argc, char ** argv)
{
    //Data Abstraction
    ifstream file;
    queue<Piece> pQueue;
    //Generate random seed for each random picture that loads when game starts
    srand(time(nullptr));

    //TEMP: CREATE A MASSIVE 2D ARRAY OURSELVES
    int vicState = 0;
    int pick = rand() % 4;
    int height = 0;
    int width = 0;
    int xOff = 500;
    color** arr;
    Piece* pieces;

    //File input is based on the random number generated
    if (pick == 0) {
        file.open("500x500amph.png.txt");
    }
    else if (pick == 1) {
        file.open("500x500Buggy.png.txt");
    }
    else if (pick == 2) {
        file.open("500x500Walter.png.txt");
    }
    else {
        file.open("boothiful.png.txt");
    }
    file >> height;
    file >> width;
    arr = new color*[height];
    for(int i = 0; i < height; i++){
        arr[i] = new color[width];
    }
    for(int i = 0; i < height; i++){
        //arr[i] = new color[width];
        for(int j = 0; j < width; j++){
            int R, B, G = 0;
            file >> R >> G >> B;
            color* c = new color(R, G, B);
            arr[j][i] = *c;
        }
    }
    file.close();
    pieces = new Piece[PIECECNT];
    for(int i = 0; i < 5; i ++){
        for(int j = 0; j < 5; j++){
            pieces[5*i + j] = *new Piece(100, arr, (i*100), j*100);
        }
    }

    //The process, implementation, and output can be seen after this point

    //ON START: associate an array of locations with puzzle pieces
    SDL_Plotter g(500,1020);
    //pieces[0].setLoc(0,0);


    //here we need to do random location
    //xOff + rand(0-400)
    //y = rand(0-400)
    for(int i = 0; i < 25; i++){
        int x1 = rand() % 400 + 10;
        int y1 = rand()%400;
        int rot = rand()%4;
        //int x = math.Rand
        pieces[i].setLoc(x1 + xOff, y1);
        for(int r = 0; r < rot; r++){
            pieces[i].rotateClockwise();
        }
        //cout << i << ":" << pieces[i].getXc1() << ",";
        //cout << pieces[i].getYc1() << endl;
        //pieces[i].printSquare(g);

    }
    printAll(pieces, g);
    //pieces[19].printSquare(g);

    for(int i = 0+xOff; i < height+xOff; i++){
        for(int j = 0; j < width; j++){
            //g.plotPixel(i, j, arr[i-xOff][j]);
        }
        color c(0, 0, 0);
        /*g.plotPixel(xOff, i-xOff, c);
        g.plotPixel(xOff+maxSZ, i-xOff, c);
        g.plotPixel(i, 0, c);
        g.plotPixel(i, 499, c);*/
    }
    point p;
    color c;
    int size;
    Uint32 RGB;
    bool selectState = false;
    int currP;
    while (!g.getQuit())
    {
        if(g.kbhit()){

            switch(toupper(g.getKey())){
                case 'C': g.clear();
                    break;
            }
        }

        if(g.mouseClick()){
            //if g.mousClick, compare location to stack of puzzle pieces
            //when first match found, that piece is selected and we enter a
            //second loop to place it
            ///DURING SPECIAL CASE:
            //case Q: rotate piece left
            //case E: rotate piece right
            //case click: place piece down if outside puzzle border
            //if inside check for match

            p = g.getMouseClick();

            for(int i = 0; i < PIECECNT; i++){

                if(pieces[i].clickable(p.x, p.y)){
                    //updates border to select
                    pieces[i].printSquare(g);
                    g.update();
                    /*cout << i << endl;
                    cout << p.x << "," << pieces[i].getXc1() << ":" <<
                         p.y << "," << pieces[i].getYc1() << ":" << endl;*/
                    currP = i;
                    selectState = true;
                    while(!g.getQuit() && selectState){
                        if(g.kbhit()){
                            switch(toupper(g.getKey())){
                                case 'Q': //cout << "rotate";
                                    pieces[i].rotateClockwise();
                                    pieces[i].printSquare(g);
                                    g.update();
                                    break;
                                case 'E':// cout << "etator";
                                    pieces[i].rotateCounter();
                                    pieces[i].printSquare(g);
                                    g.update();
                                    break;
                            }
                        }
                        if(g.mouseClick()){
                            p = g.getMouseClick();
                            pieces[currP].setLoc(p.x - pieces[i].getSize()/2,
                                                 p.y - pieces[i].getSize()/2);

                            if(pieces[currP].clickPlace(p.x, p.y)){
                                vicState ++;
                                pieces[currP].switchClick();
                            }
                            g.clear();
                            //for(int j = 0; j < PIECECNT; j++){
                            //    pieces[j].printSquare(g);
                            //}
                            printAll(pieces, g);
                            selectState = false;
                        }
                    }
                    //cout << " ha" << endl;
                }


                //break;
            }
            //cout << i << ":" << pieces[i].getXc1() << ",";
            //cout << pieces[i].getYc1() << endl;
        }

        g.update();
        if(vicState == 25){
            //make the victory a bit more fun
            //audio? visuals?
            cout << "WIN";
            vicState ++;
            winThis(g, "Trophy.png.txt");

        }

    }

    //delete plink;
    delete[] arr;
    delete[] pieces;
    return 0;
}

/* printAll
 * description: plots all puzzle pieces
 * return: nothing
 * pre: multiple pieces and SDL_Plotter exist
 * post: prints all the puzzle pieces on screen
 */
void printAll(Piece* e, SDL_Plotter& g){
    //print all the goons
    for(int i = 0; i < PIECECNT; i++){
        e[i].printSquare(g);
    }

    //print border of the final spot
    color bCol(200, 100, 100);
    for(int i = 500; i < 510; i++){
        for(int j = 0; j < 500; j++){
            g.plotPixel(i, j, bCol);
        }

    }
}
/* winThis
 * description: display the victory screen once the puzzle has been solved
 * return: nothing
 * pre: an SDL_Plotter and file name exist
 * post: displays the victory screen with a picture from the file name
 */
void winThis(SDL_Plotter& g, string fileName){
    ifstream file;
    file.open(fileName);
    int height, width, xOffSet, yOffSet;
    xOffSet = 560;
    yOffSet = 60;
    file >> height;
    file >> width;
    color** arr = new color*[height];
    for(int i = 0; i < height; i++){
        arr[i] = new color[width];
    }
    for(int i = 0; i < height; i++){
        //arr[i] = new color[width];
        for(int j = 0; j < width; j++){
            int R, B, G = 255;
            file >> R >> G >> B;
            color* c = new color(R, G, B);
            arr[i][j] = *c;
            g.plotPixel(j+xOffSet, i+yOffSet, arr[i][j]);
        }
    }
    file.close();
    g.update();
}