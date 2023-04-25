/*
 * Authors: Peter McGuinness
 * Function: main
 * Changelog:
 * 4/16/2023 created by Peter McGuinness
 * 4/23/2023 added pieceLinker and corresponding bits(did not work)
 * 4/25/2023 started making clickable functionality: Peter McGuinness
*/
#include <iostream>
#include <cmath>
#include "SDL_Plotter.h"
#include "Piece.h"

using namespace std;

void drawCircle(point loc, int size, color c, SDL_Plotter& g);

//need to make a linked list of Pieces
struct pieceLinker{
    Piece curr;
    pieceLinker* prev = nullptr;
    pieceLinker* next = nullptr;
    pieceLinker(){

    }
    pieceLinker(Piece p){
        curr = p;
    }
    pieceLinker(Piece p, pieceLinker* p1, pieceLinker* p2){
        curr = p;
        prev = p1;
        next = p2;
    }
    ~pieceLinker(){
        delete next;
    }
};
void printLinked(pieceLinker* l, SDL_Plotter& g);
static int PIECECNT = 25;

int main(int argc, char ** argv)
{

    vector<Piece> pLink;
    //TEMP: CREATE A MASSIVE 2D ARRAY OURSELVES

    int maxSZ = 500;
    int xOff = 250;
    color** arr = new color*[maxSZ];
    for(int i = 0; i < maxSZ; i++){
        arr[i] = new color[maxSZ];
        for(int j = 0; j < maxSZ; j++){
            int temp = 252 - (i/2);
            color* c = new color(temp-((j+4)/2), temp, temp);
            arr[i][j] = *c;
        }
    }
    Piece* pieces = new Piece[PIECECNT];
    for(int i = 0; i < 5; i ++){
        for(int j = 0; j < 5; j++){
            pieces[5*i + j] = *new Piece(100, arr, (i*100), j*100);
        }
    }


    //ON START: associate an array of locations with puzzle pieces

    SDL_Plotter g(500,1000);
    //pieces[0].setLoc(0,0);

    //pieces[0].printSquare(g);
    //pieces[19].printSquare(g);
    for(int i = 0; i < 25; i++){
        pieces[i].setLoc(pieces[i].getXc1() + xOff, pieces[i].getYc1());
        //cout << i << ":" << pieces[i].getXc1() << "," << pieces[i].getYc1() << endl;
        pieces[i].printSquare(g);

    }
    //pieces[19].printSquare(g);

    for(int i = 0+xOff; i < maxSZ+xOff; i++){
        for(int j = 0; j < maxSZ; j++){
            //g.plotPixel(i, j, arr[i-xOff][j]);
        }
        color c(0, 0, 0);
        g.plotPixel(xOff, i-xOff, c);
        g.plotPixel(xOff+maxSZ, i-xOff, c);
        g.plotPixel(i, 0, c);
        g.plotPixel(i, 499, c);
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
            //when first match found, that piece is selected and we enter a second loop to place it
            ///DURING SPECIAL CASE:
            //case Q: rotate piece left
            //case E: rotate piece right
            //case click: place piece down if outside puzzle border, if inside check for match

            p = g.getMouseClick();

            for(int i = 0; i < PIECECNT; i++){

                if(pieces[i].clickable(p.x, p.y)){

                    /*cout << i << endl;
                    cout << p.x << "," << pieces[i].getXc1() << ":" <<
                         p.y << "," << pieces[i].getYc1() << ":" << endl;*/
                    currP = i;
                    selectState = true;
                }


                    //break;
            }
                //cout << i << ":" << pieces[i].getXc1() << "," << pieces[i].getYc1() << endl
        }
        while(!g.getQuit() && selectState){
            if(g.mouseClick()){
                p = g.getMouseClick();
                pieces[currP].setLoc(p.x, p.y);
                g.clear();
                for(int j = 0; j < PIECECNT; j++){
                    pieces[j].printSquare(g);
                }

                selectState = false;
            }
        }
        g.update();

    }
    //delete plink;
    delete[] arr;
    delete[] pieces;
    return 0;
}


void drawCircle(point loc, int size, color c, SDL_Plotter& g){
    for(double i = -size; i <= size;i+=0.1){
        for(double j = -size; j <= size; j+=0.1){
            if(i*i + j*j <= size*size){
                g.plotPixel(round(loc.x+i),round(loc.y+j),c);
            }
        }
    }

}
void printLinked(pieceLinker* l, SDL_Plotter& g){
    //cout << l->curr.getXc1();
    l->curr.printSquare(g);
    //cout << "a" << endl;
    if(l->next != nullptr){
        printLinked(l->next, g);
    }
}

