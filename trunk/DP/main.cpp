#include "App.h"
#include "FactoryMethod.h"
#include "AbstractFactory.h"
#include "Builder.h"

void printSep()
{
	cout<<"=================================================================="<<endl;
}

void test()
{
	MazeGame game;
	Maze* pMaze = game.CreateMaze();
	pMaze->show();
	printSep();

	//Factory method
	MazeGame_FactoryMethod_Enchanted game_fme;
	pMaze = game_fme.CreateMaze();
	pMaze->show();
	printSep();

	//Abstract Factory
	MazeGame_AbstractFactory game_af;
	EnchantedMazeFactory factory;
	pMaze = game_af.CreateMaze(factory);
	pMaze->show();
	printSep();

	//Builder
	MazeGame_Builder game_builder;
	StandardMazeBuilder builder;
	pMaze = game_builder.CreateMaze(builder);
	pMaze->show();
	printSep();
}

int main(int argc, char* argv[])
{
	test();

	return 0;
}