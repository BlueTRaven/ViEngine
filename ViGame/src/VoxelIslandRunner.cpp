#include <iostream>
#include <stdio.h>

#include "VoxelIslandGame.h"
#undef main	//SDL defines main for some reason, so we need to undefine it

const int cSCREEN_WIDTH = 640;
const int cSCREEN_HEIGHT = 480;

int main(int argc, char* argv[])
{
	vigame::VoxelIslandGame* game = new vigame::VoxelIslandGame();
	game->Start();	//game loop here

	delete game;

    return 0;
}

