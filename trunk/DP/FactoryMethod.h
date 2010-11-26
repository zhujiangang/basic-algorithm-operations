#ifndef _FACTORY_METHOD_H_
#define _FACTORY_METHOD_H_

#include "App.h"

class MazeGame_FactoryMethod
{
public:
	Maze* CreateMaze()
	{
		Maze* aMaze = MakeMaze();
		
		Room* r1 = MakeRoom(1);
		Room* r2 = MakeRoom(2);
		Door* theDoor = MakeDoor(r1, r2);
		
		aMaze->AddRoom(r1);
		aMaze->AddRoom(r2);
		
		r1->SetSide(North, MakeWall());
		r1->SetSide(East, theDoor);
		r1->SetSide(South, MakeWall());
		r1->SetSide(West, MakeWall());
		
		r2->SetSide(North, MakeWall());
		r2->SetSide(East, MakeWall());
		r2->SetSide(South, MakeWall());
		r2->SetSide(West, theDoor);
		
		return aMaze;
	}

	//factory methods;
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

class MazeGame_FactoryMethod_Enchanted : public MazeGame_FactoryMethod
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

#endif