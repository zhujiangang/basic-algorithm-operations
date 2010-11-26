#include "Builder.h"

StandardMazeBuilder::StandardMazeBuilder()
{
	_currentMaze = 0;
}

void StandardMazeBuilder::BuildMaze()
{
	_currentMaze = new Maze;
}

Maze* StandardMazeBuilder::GetMaze() 
{ 
	return _currentMaze;
}

void StandardMazeBuilder::BuildWall(Room* room, Direction dir)
{
	room->SetSide(dir, new Wall);
}

void StandardMazeBuilder::BuildDoor(int roomFrom, int roomTo) 
{
	Room* r1 = _currentMaze->RoomNo(roomFrom);
	Room* r2 = _currentMaze->RoomNo(roomTo);

	Door* d = new Door(r1, r2);

	r1->SetSide(CommonWall(r1, r2), d);
	r2->SetSide(CommonWall(r2, r1), d);
}

void StandardMazeBuilder::BuildRoom(int n)
{
	if(!_currentMaze->RoomNo(n))
	{
		Room* room = new Room(n);
		_currentMaze->AddRoom(room);

		BuildWall(room, North);
		BuildWall(room, South);
		BuildWall(room, East);
		BuildWall(room, West);
	}
}

Direction StandardMazeBuilder::CommonWall(Room* r1, Room* r2)
{
	if(r1->GetRoomNo() < r2->GetRoomNo())
	{
		return East;
	}
	else
	{
		return West;
	}
}
