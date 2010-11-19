#include "App.h"
#include "AbstractFactory.h"

void test()
{
	MazeGame game;
	Maze* pMaze = game.CreateMaze();
	Room* pRoom = 0;
	
	pRoom = pMaze->RoomNo(2);
	cout<<pRoom<<endl;

	pRoom = pMaze->RoomNo(5);
	cout<<pRoom<<endl;	
}

int main(int argc, char* argv[])
{
	test();

	return 0;
}