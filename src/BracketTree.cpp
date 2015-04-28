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

#include <BracketTree.h>
#include <cmath>

//Constructor which takes two arguments:
//playersList: array of Player pointers
//numPlayers: how many Players* in the list
BracketTree::BracketTree(Player* players[], int nPlayers){
    numPlayers = nPlayers;
    
    //Copy all the Players into a local array so that they can be 
    //properly deleted
    playersList = new Player*[numPlayers];
    for (int i = 0; i < numPlayers; i++)
        playersList[i] = players[i];

    //Calculate positions to place things in the array
    int log2Smashers = (int) ceil(log2(numPlayers));
    playerTreeSize = (int) pow(2, log2Smashers + 1);
    playerTree = new Player*[playerTreeSize];
    numLevels = (int) round(log2(playerTreeSize));
    int numFirstRoundSpaces = playerTreeSize / 2;
    int numSecondRounders = playerTreeSize / 4;
    int numLeftovers = numPlayers - numSecondRounders;
    int numFirstRounders = numLeftovers * 2;

    //Initialize all spots to NULL
    for (int i = 0; i < playerTreeSize; i++){
        playerTree[i] = NULL;
    }
    
    for (int i = 0; i < numFirstRounders; i++){
        playerTree[i] = playersList[i];
    }

    for (int i = 0; i < numSecondRounders - numLeftovers; i++){
        playerTree[numFirstRoundSpaces + numLeftovers + i] = playersList[numFirstRounders + i];
    }

}

//Destructor
BracketTree::~BracketTree(){
    delete [] playerTree;
    for (int i = 0; i < numPlayers; i++)
        delete playersList[i];
    delete [] playersList;
}

//Returns a pointer to the Player object at the specified position
//in the bracket
Player* BracketTree::getPlayerAt(unsigned int level, unsigned int pos){
    unsigned int treePos = playerTreeSize - playerTreeSize / pow(2, level) + pos;
    if (treePos >= playerTreeSize)
        return NULL;
    return playerTree[treePos];
}

//Checks if the player at the specified location can advance
bool BracketTree::canPlayerAdvance(unsigned int level, unsigned int pos){
    Player* p = getPlayerAt(level, pos);
    if (p == NULL)
        return false;

    int advanceTreePos = playerTreeSize - playerTreeSize / pow(2, level + 1) + (pos / 2);
    if (advanceTreePos >= playerTreeSize || playerTree[advanceTreePos] != NULL)
        return false;

    return true;
}

//Advance the player at the specified location up the bracket
//Returns true if successful, false otherwise
bool BracketTree::playerWon(unsigned int level, unsigned int pos){
    Player* p = getPlayerAt(level, pos);
    if (p == NULL)
        return false;

    int treePos = playerTreeSize - playerTreeSize / pow(2, level + 1) + (pos / 2);
    if (treePos >= playerTreeSize || playerTree[treePos] != NULL)
        return false;
    
    playerTree[treePos] = p;
    return true;
}

unsigned int BracketTree::getNumLevels(){
    return numLevels;
}

unsigned int BracketTree::getNumPlayers(){
    return numPlayers;
}

