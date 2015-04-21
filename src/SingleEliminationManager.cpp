/**************************************************************
EasyTourney -- A simple, easy to use tournament bracket manager
Copyright (C) 2015 Dylan Wulf

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
****************************************************************/

#include <SingleEliminationManager.h>
#include <cmath>
#include <cstdlib>
//the rand() function is in cstdlib

SingleEliminationManager::SingleEliminationManager(Player* playerList[], unsigned int numSpots, bool randomize): BracketManager() {
    if (randomize){
        randomizePlayers(playerList, numSpots);
    }
    playerTree = new BracketTree(playerList, numSpots);

}

void SingleEliminationManager::drawBracket(wxDC& dc){
    int numLevels = playerTree->getNumLevels();
    int numSpots = pow(2, numLevels - 1);
    int currSpots = numSpots;
    int currLevel = 0;
    double levelWidth = (double) canvasWidth / (log2(numSpots) + 1);

    int numPlayers = playerTree->getNumPlayers();
    int numSecondRounders = pow(2, numLevels - 2);
    int numLeftovers = numPlayers - numSecondRounders;
    int numFirstRounders = numLeftovers * 2;

    //Just draw the very left (bottom) ones first
    for (int i = 0; i < numFirstRounders; i++){
        double branchHeight = (double) canvasHeight / currSpots;
        int x1 = 0;
        int x2 = levelWidth;
        int y1 = branchHeight * i + branchHeight / 2;
        int y2 = y1;

        dc.DrawLine(x1, y1, x2, y2);
        if (playerTree->getPlayerAt(currLevel, i) != NULL){
            Player* p = playerTree->getPlayerAt(currLevel, i);
            dc.DrawText(p->getName(), x1 + 20, y1 - 16);
        }

        if (i % 2 == 0 && i < currSpots - 1)
            dc.DrawLine(x2, y1, x2, y1 + branchHeight);
    }

    currSpots /= 2;
    currLevel++;

    //Then draw all the rest
    while (currSpots >= 1){
        
        double branchHeight = (double) canvasHeight / currSpots;
        int x1 = levelWidth * currLevel;
        int x2 = levelWidth * (currLevel + 1);

        for (int i = 0; i < currSpots; i++){
            
            int y1 = branchHeight * i + branchHeight / 2;
            int y2 = y1;

            dc.DrawLine(x1, y1, x2, y2); //Draw horizontal lines
            if (playerTree->getPlayerAt(currLevel, i) != NULL){
                Player* p = playerTree->getPlayerAt(currLevel, i);
                dc.DrawText(p->getName(), x1 + 20, y1 - 16);
            }

            if (i % 2 == 0 && i < currSpots - 1)
                dc.DrawLine(x2, y1, x2, y1 + branchHeight);
                //Draw vertical lines
        }
        currSpots /= 2;
        currLevel++;
    }
}

SingleEliminationManager::~SingleEliminationManager(){
    delete playerTree;
}

Player* SingleEliminationManager::processClick(int x, int y){
    int level = x / ((float) canvasWidth / playerTree->getNumLevels());
    int numBranches = pow(2, playerTree->getNumLevels() - level - 1);
    int pos = y / ((float) canvasHeight / numBranches);
    return playerTree->getPlayerAt(level, pos);
}

void SingleEliminationManager::playerWon(Player* p){

}

void SingleEliminationManager::randomizePlayers(Player* p[], int n){
    for (int i = 0; i < n; i++){
        double random = rand()/static_cast<double>(RAND_MAX-1);
        int r = i + static_cast<int>(random * (n - i));
        Player* temp = p[i];
        p[i] = p[r];
        p[r] = temp;
    }
}

