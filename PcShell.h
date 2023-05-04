//
// Created by peter on 4/18/2023.
/*
 * Authors: Peter McGuinness, Alex DeVries, Xavier Lopez,
 *          Derek Faucett, Christian Ocana
 * 4/27/2023 changed checkCollide logic
 */
//

using namespace std;

class PcShell{
private:
    int xC1;
    int yC1;
    int size;
public:
    PcShell(int x, int y, int s){
        xC1 = x; yC1 = y; size = s;
    }
    PcShell* operator=(PcShell &tShell){
        xC1 = tShell.xC1;
        yC1 = tShell.yC1;
        size = tShell.size;
        return this;
    }
    int getX(){
        return xC1;
    }
    int getY(){
        return yC1;
    }

    /* checkCollide
     * description: returns true if the input values match to the PcShell's
     * physical representation
     * return: bool true if match, false else
     * pre: x and y are valid integers, PcShell has initialized values.
     * post: returns true if match, false else
    */
    bool checkCollide(int x, int y){
        int tX = x - xC1;
        int tY = y - yC1;

        if(tX < size && tY < size && tX > 0 && tY > 0){
            return true;
        }
        return false;
    }

};

#ifndef GROUPPROJECT_PCSHELL_H
#define GROUPPROJECT_PCSHELL_H

#endif //GROUPPROJECT_PCSHELL_H
