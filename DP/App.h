#ifndef _DP_APP_H_
#define _DP_APP_H_

#include <iostream.h>
#include <vector>

using std::vector;

enum Direction 
{
	North, South, East, West
};

class MapSite
{
public:
	virtual void Enter() = 0;
	virtual const char* ToString() const
	{
		return "MapSite";
	}
};

class Room : public MapSite
{
public:
	Room(int roomNo) : _roomNumber(roomNo)
	{
	}
	int GetRoomNo() const
	{
		return _roomNumber;
	}
	MapSite* GetSide(Direction dir) const
	{
		return _sides[dir];
	}
	void SetSide(Direction dir, MapSite* pMapSite)
	{
		_sides[dir] = pMapSite;
	}
	virtual void Enter()
	{
		cout<<"Enter room"<<_roomNumber<<endl;
	}
	const char* ToString() const
	{
		return "Room";
	}
private:
	MapSite* _sides[4];
	int _roomNumber;
};

class CastSpell
{
};

class EnchantedRoom : public Room
{
public:
	EnchantedRoom(int roomNo, CastSpell* pCastSpell) : Room(roomNo), m_pCastSpell(pCastSpell)
	{
	}
	const char* ToString() const
	{
		return "EnchantedRoom";
	}
private:
	CastSpell* m_pCastSpell;
};

class Wall : public MapSite
{
public:
	Wall()
	{
	}
	virtual void Enter()
	{
	}
	const char* ToString() const
	{
		return "Wall";
	}
};

class Door : public MapSite
{
public:
	Door(Room* r1 = 0, Room* r2 = 0, bool isOpen = false) : _room1(r1), _room2(r2), _isOpen(isOpen)
	{
	}
	virtual void Enter()
	{
		if(!_isOpen)
		{
			return;
		}
		cout<<"Enter Door "<<endl;
	}
	Room* OtherSideFrom(Room* r)
	{
		if(r == _room1)
		{
			return _room2;
		}
		else if(r == _room2)
		{
			return _room1;
		}
		cout<<"Error paramter: r = "<<r<<endl;
		return 0;
	}
	const char* ToString() const
	{
		return "Door";
	}
private:
	Room* _room1;
	Room* _room2;
	bool _isOpen;
};

class DoorNeedingSpell : public Door
{
public:
	DoorNeedingSpell(Room* r1 = 0, Room* r2 = 0, bool isOpen = false) : Door(r1, r2, isOpen)
	{
	}
	const char* ToString() const
	{
		return "DoorNeedingSpell";
	}
};

class Maze
{
public:
	Maze()
	{
	}
	void AddRoom(Room* r)
	{
		_rooms.push_back(r);
	}
	Room* RoomNo(int roomNo)
	{
		std::vector<Room*>::iterator iter = _rooms.begin();
		for(; iter != _rooms.end(); ++iter)
		{
			if((*iter)->GetRoomNo() == roomNo)
			{
				return *iter;
			}
		}
		return 0;
	}
	size_t RoomSize() const
	{
		return _rooms.size();
	}
	Room* GetRoom(size_t index)
	{
		return _rooms[index];
	}
	void show()
	{
		for(size_t i = 0, len = RoomSize(); i < len; i++)
		{
			Room* r = GetRoom(i);
			cout<<r->ToString()<<"("<<r->GetRoomNo()<<") ";
			for(int d = North; d <= West; d++)
			{
				cout<<"["<<d<<"]="<<r->GetSide((Direction)d)->ToString()<<" ";
			}
			cout<<endl;
		}
	}
private:
	vector<Room*> _rooms;
};

class MazeGame
{
public:
	MazeGame()
	{
	}
	virtual Maze* CreateMaze()
	{
		Maze* aMaze = new Maze;

		Room* r1 = new Room(1);
		Room* r2 = new Room(2);
		Door* theDoor = new Door(r1, r2);
		
		aMaze->AddRoom(r1);
		aMaze->AddRoom(r2);

		r1->SetSide(North, new Wall());
		r1->SetSide(East, theDoor);
		r1->SetSide(South, new Wall());
		r1->SetSide(West, new Wall());

		r2->SetSide(North, new Wall());
		r2->SetSide(East, new Wall());
		r2->SetSide(South, new Wall());
		r2->SetSide(West, theDoor);

		return aMaze;
	}
};

class MazeGameRawExt : public MazeGame
{
public:
	Maze* CreateMaze()
	{
		Maze* aMaze = new Maze;
		
		Room* r1 = new EnchantedRoom(1, new CastSpell());
		Room* r2 = new EnchantedRoom(2, new CastSpell());
		Door* theDoor = new DoorNeedingSpell(r1, r2);
		
		aMaze->AddRoom(r1);
		aMaze->AddRoom(r2);
		
		r1->SetSide(North, new Wall());
		r1->SetSide(East, theDoor);
		r1->SetSide(South, new Wall());
		r1->SetSide(West, new Wall());
		
		r2->SetSide(North, new Wall());
		r2->SetSide(East, new Wall());
		r2->SetSide(South, new Wall());
		r2->SetSide(West, theDoor);
		
		return aMaze;
	}
};


#endif