#ifndef _ABSTRACT_FACTORY_H_
#define _ABSTRACT_FACTORY_H_

#include "App.h"

class MazeFactory
{
public:
	MazeFactory(){}

	virtual Maze* MakeMaze() const
	{ 
		return new Maze; 
	}
	virtual Room* MakeRoom(int n) const
	{
		return new Room(n);
	}
	virtual Wall* MakeWall() const
	{
		return new Wall;
	}
	virtual Door* MakeDoor(Room* r1, Room* r2) const
	{
		return new Door(r1, r2);
	}	
};

class EnchantedMazeFactory : public MazeFactory
{
public:
	virtual Room* MakeRoom(int n) const
	{
		return new EnchantedRoom(n, new CastSpell);
	}
	virtual Door* MakeDoor(Room* r1, Room* r2) const
	{
		return new DoorNeedingSpell(r1, r2);
	}
};


class MazeGame_AbstractFactory
{
public:
	Maze* CreateMaze(MazeFactory& factory)
	{
		Maze* aMaze = factory.MakeMaze();
		
		Room* r1 = factory.MakeRoom(1);
		Room* r2 = factory.MakeRoom(2);
		Door* theDoor = factory.MakeDoor(r1, r2);
		
		aMaze->AddRoom(r1);
		aMaze->AddRoom(r2);
		
		r1->SetSide(North, factory.MakeWall());
		r1->SetSide(East, theDoor);
		r1->SetSide(South, factory.MakeWall());
		r1->SetSide(West, factory.MakeWall());
		
		r2->SetSide(North, factory.MakeWall());
		r2->SetSide(East, factory.MakeWall());
		r2->SetSide(South, factory.MakeWall());
		r2->SetSide(West, theDoor);
		
		return aMaze;
	}
};
#endif