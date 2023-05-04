//
// Authors: Peter McGuinness, Alex DeVries, Xavier Lopez, Derek Faucett
//          Christian Ocana
/*Changelog:
 * 4/17/2023 created by Peter McGuinness
 * 4/18/2023 added printing functionality:  Alex DeVries
 * 4/27/2023 added borders to pieces: Xavier Lopez
 * 4/28/2023 added new border functionality: Peter McGuinness
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
    color bCol;
    PcShell* tP;
    bool click = true;
    int orient;
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
        color tCol(0, 0, 0);
        bCol = tCol;
        orient = 0;
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
        tP = tPiece.tP;
        bCol = tPiece.bCol;
        orient = tPiece.orient;
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
        color tCol(0, 0, 0);
        bCol = tCol;
        orient = 0;
    }
    /* setLoc
     * description: sets the location variables of the piece
     * return: void
     * pre: parameters x and y are integer values
     * post: sets xC1 and yC1 to x and y
    */
    void setLoc(int x, int y){
        xC1 = x;
        yC1 = y;
    }
    /* printSquare
     * description: prints out the Square
     * return: void
     * pre: Plotter object g exists
     * post: printts the piece represented as a square
    */
    void printSquare(SDL_Plotter& g){

        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++) {
                g.plotPixel(xC1 + i, yC1 + j, pVis[i][j]);
            }
        }
        if(click == true){
            //cout << bCol.B << bCol.G << bCol.R;
            printBorder(g);
        }

    }
    /* printBorder
     * description: pritns out a border
     * return: void
     * pre: Plotter g exists
     * post: prints the border of the square
    */
    void printBorder(SDL_Plotter &g){
        for(int i = 0; i < size; i++){
            if(i == 0 || i == size-1) {
                for (int j = 0; j < size; j++) {
                    g.plotPixel(xC1 + i, yC1 + j, bCol);
                }
            }else{
                g.plotPixel(xC1 + i, yC1, bCol);
                g.plotPixel(xC1 + i, yC1 + size, bCol);
            }

        }
    }
    /* rotateClockwise
     * description: rotates the piece's data clockwise 90 degrees
     * return: void
     * pre: piece has a valid initialized square matrix of colors
     * post: color matrix is mathematically rotated
    */
    void rotateClockwise(){
        for(int i = 0; i < size; i++){
            for(int j = i; j < size; j++){
                swap(pVis[i][j], pVis[j][i]);
            }
        }
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size/2; j++){
                swap(pVis[j][i], pVis[size-j-1][i]);
            }
        }
        orient = (orient +1)%4;
    }
    /* rotateCounter
     * description: rotates the piece's data counterclockwise.
     * return: void
     * pre: piece has an initialized squarematrix of colors.
     * post: data is mathematically rotated counterclockwise.
    */
    void rotateCounter(){
        /*
        for(int j = 0; j < size; j++){
            for(int i = 0; i < j; i++){
                swap(pVis[i][j], pVis[j][i]);
            }
        }*/

        for(int j = size-1; j >= 0; j--){
            for(int i = 0; i < j; i++){
                swap(pVis[i][size-j-1], pVis[j][size-i-1]);
            }
        }

        for(int i = 0; i < size; i++){
            for(int j = 0; j < size/2; j++){
                swap(pVis[j][i], pVis[size-j-1][i]);
            }
        }
        orient = (orient-1)%4;
    }
    /* clickable
     * description: returns boolean determining if the piece is clickable
     * return: true if input matches over the piece's physical representation,
     * false else
     * pre: x and y are valid integers, piece has initialized values
     * post: if x and y are both within the pieces area, return true. else, false
    */
    bool clickable(int x, int y){
        if(!click){
            return false;
        }
        int tX = x - xC1;
        int tY = y - yC1;

        if(tX < size && tY < size && tX > 0 && tY > 0){
            color tCol(0, 252, 0);
            bCol = tCol;
            return true;
        }
        return false;
    }
    void switchClick(){
        click  = false;
    }

    /* clickPlace
     * description: returns a boolean representing if the piece has reached
     * its final spot
     * return: true if it has, else false
     * pre: x and y are valid integers, piece has initialized values and
     * associated PcShell.
     * post: returns true if the piece has made it to the final place, false else
     * sets the piece's location to the PcShell.
    */
    bool clickPlace(int x, int y){
        //first check if it collides with associated PcShell
        color tCol(0, 0, 0);
        bCol = tCol;
        if(tP->checkCollide(x, y) && orient == 0){

            xC1 = tP->getX();
            yC1 = tP->getY();
            return true;
        }
        return false;
        //after that, determine xC1 position based on center
        //prolly just input x - size/2
    }

    int getXc1() const {
        return xC1;
    }

    int getYc1() const {
        return yC1;
    }

    int getSize() const{
        return size;
    }
};

#define GROUPPROJECT_PIECE_H

#endif //GROUPPROJECT_PIECE_H
