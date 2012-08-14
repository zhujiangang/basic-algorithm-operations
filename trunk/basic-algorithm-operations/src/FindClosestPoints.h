// FindClosestPoints.h: interface for the CFindClosestPoints class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FINDCLOSESTPOINTS_H__6D492F5D_20EB_4E25_A782_8D32A556BDA1__INCLUDED_)
#define AFX_FINDCLOSESTPOINTS_H__6D492F5D_20EB_4E25_A782_8D32A556BDA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFindClosestPoints  
{
public:
	CFindClosestPoints();
	virtual ~CFindClosestPoints();

public:
	typedef struct
	{
		int x, y;
	}Point;

	typedef struct  
	{
		Point pt;
		int id;
		int* pPosX;
	}PointEx;

	static double closest_points(Point points[], int n, Point* p1, Point* p2);
	static int abs(int a)
	{
		return (a >= 0 ? a : -a);
	}
	static double abs(double a)
	{
		return (a >= 0 ? a : -a);
	}
	static double max(double a, double b)
	{
		return (a >= b ? a : b);
	}
	static double distance(Point p1, Point p2);

	static int __cdecl compare_x(const void *elem1, const void *elem2 );
	static int __cdecl compare_y(const void *elem1, const void *elem2 );
private:

	static double closest_points(PointEx x[], PointEx y[], PointEx z[], int l, int r, Point* p1, Point* p2);
};

#endif // !defined(AFX_FINDCLOSESTPOINTS_H__6D492F5D_20EB_4E25_A782_8D32A556BDA1__INCLUDED_)
