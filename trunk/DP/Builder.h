#ifndef _BUILDER_H_
#define _BUILDER_H_

#include "App.h"

class MazeBuilder
{
public:
	virtual void BuildMaze() {}
	virtual void BuildRoom(int room) {}
	virtual void BuildDoor(int roomFrom, int roomTo) {}
	virtual void BuildWall(Room* r1, Direction dir){}
	virtual Maze* GetMaze() { return 0; }
protected:
	MazeBuilder(){}
};

class StandardMazeBuilder : public MazeBuilder
{
public:
	StandardMazeBuilder();
	virtual void BuildMaze();
	virtual void BuildRoom(int room);
	virtual void BuildDoor(int roomFrom, int roomTo);
	virtual void BuildWall(Room* room, Direction dir);
	virtual Maze* GetMaze();
private:
	Direction CommonWall(Room* r1, Room* r2);
	Maze* _currentMaze;
};

class MazeGame_Builder
{
public:
	Maze* CreateMaze(MazeBuilder& builder)
	{
		builder.BuildMaze();
		builder.BuildRoom(1);
		builder.BuildRoom(2);
		builder.BuildDoor(1, 2);

		return builder.GetMaze();
	}
};

#endif