//
// Authors: Peter McGuinness
/*Changelog:
 * 4/17/2023 created by Peter McGuinness
 * 4/18/2023 added printing functionality
 */
//

#ifndef GROUPPROJECT_PIECE_H
#include "SDL_Plotter.h"
#include "PcShell.h"
#include "math.h"
using namespace std;


class Piece{
private:
    int size = 0; //piece is roughly square
    int xC1;//denotes upper left corner
    int yC1;
    color** pVis;
    PcShell* tP;
public:
    Piece(){

    }
    Piece(int s) {
        tP = new PcShell(0, 0, s);
        size = s;
        pVis = new color *[size];
        for (int i = 0; i < size; i++) {
            pVis[i] = new color[size];
        }
        xC1 = -1;
        yC1 = -1;
    }
    ~Piece(){
        delete tP;
        delete[] pVis;
    }
    const Piece& operator=(Piece &tPiece){
        size = tPiece.size;
        pVis = new color*[size];
        for(int i = 0; i < size; i++){
            pVis[i] = new color[size];
            for(int j = 0; j < size; j++){
                pVis[i][j] = tPiece.pVis[i][j];
            }
        }
        xC1 = tPiece.xC1;
        yC1 = tPiece.yC1;
        return *this;
    }
    Piece(int s, color** bigC, int x, int y){
        size = s;
        pVis = new color*[size];
        xC1 = x;
        yC1 = y;
        for(int i = 0; i < size; i++){
            pVis[i] = new color[size];
        }
        tP = new PcShell(x, y, s);
        //bigC tells us where to pull from
        //loc tells us where to start
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                pVis[i][j] = bigC[x+i][y+j];
            }
        }
    }
    void setLoc(int x, int y){
        xC1 = x;
        yC1 = y;
    }
    void printSquare(SDL_Plotter& g){

        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                g.plotPixel(xC1 + i, yC1 + j, pVis[i][j]);
            }
        }
    }
    bool clickable(int x, int y){
        int tX = x - xC1;
        int tY = y - yC1;

        if(tX < size && tY < size && tX > 0 && tY > 0){
            return true;
        }
        return false;
    }

    void clickPlace(int x, int y, SDL_Plotter& g){
        //first check if it collides with associated PcShell
        if(tP->checkCollide(x, y)){
           cout << "a" << endl;
        }

        //after that, determine xC1 position based on center
        //prolly just input x - size/2
    }

    int getXc1() const {
        return xC1;
    }

    int getYc1() const {
        return yC1;
    }
};

#define GROUPPROJECT_PIECE_H

#endif //GROUPPROJECT_PIECE_H
