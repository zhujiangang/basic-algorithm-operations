// FindClosestPoints.cpp: implementation of the CFindClosestPoints class.
//
//////////////////////////////////////////////////////////////////////

#include "FindClosestPoints.h"
#include <math.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFindClosestPoints::CFindClosestPoints()
{

}

CFindClosestPoints::~CFindClosestPoints()
{

}

double CFindClosestPoints::distance(Point p1, Point p2)
{
	int x = p1.x - p2.x;
	int y = p1.y - p2.y;
	return sqrt(x*x + y*y);
}

int __cdecl CFindClosestPoints::compare_x(const void *elem1, const void *elem2 )
{
	const PointEx* pt1 = (const PointEx*)elem1;
	const PointEx* pt2 = (const PointEx*)elem2;
	return pt1->pt.x - pt2->pt.x;
}

int __cdecl CFindClosestPoints::compare_y(const void *elem1, const void *elem2 )
{
	const PointEx* pt1 = (const PointEx*)elem1;
	const PointEx* pt2 = (const PointEx*)elem2;
	return pt1->pt.x - pt2->pt.x;
}

double CFindClosestPoints::closest_points(Point points[], int n, Point* p1, Point* p2)
{
	if(n <= 1)
	{
		return -1;
	}
	int i;
	int* posBuffer = new int[n];
	//copy points into buffer
	PointEx* x = new PointEx[n];
	for(i = 0; i < n; i++)
	{
		x[i].pt = points[i];
		x[i].id = i;
		x[i].pPosX = &(posBuffer[i]);
	}
	//1. sort by x
	qsort(x, n, sizeof(PointEx), CFindClosestPoints::compare_x);
	for(i = 0; i < n; i++)
	{
		*(x[i].pPosX) = i;
	}

	PointEx* y = new PointEx[n];
	for(i = 0; i < n; i++)
	{
		y[i] = x[i];
	}
	//2. Sort by y
	qsort(x, n, sizeof(PointEx), CFindClosestPoints::compare_y);

	PointEx* z = new PointEx[n];
	//3. copy y into z
	for(i = 0; i < n; i++)
	{
		z[i] = y[i];
	}

	return closest_points(x, y, z, 0, n - 1, p1, p2);
}

double CFindClosestPoints::closest_points(PointEx x[], PointEx y[], PointEx z[], int l, int r, Point* p1, Point* p2)
{
	double d;
	//TODO: less than 3 points
	if(r - l == 1)
	{
		d = distance(x[l].pt, x[r].pt);
		*p1 = x[l].pt;
		*p2 = x[r].pt;
	}
	if(r - l == 2)
	{
		int i, j;
		d = 100000;
		double dist = 0;
		for(i = l; i < r; i++)
		{
			for(j = i + 1; j <= r; j++)
			{
				dist = distance(x[i].pt, x[j].pt);
				if(dist < d)
				{
					d = dist;
					*p1 = x[i].pt;
					*p2 = x[j].pt;
				}
			}
		}
	}

	int m = (l + r) / 2;
	//split y into yl and yr
	int i, yl = l, yr = m + 1;
	for(i = l; i <= r; i++)
	{
		if(*(y[i].pPosX) <= m)
		{
			z[yl++] = y[i];
		}
		else
		{
			z[yr++] = y[i];
		}
	}

	d = closest_points(x, z, y, l, m, p1, p2);
	Point pr1, pr2;
	double dr = closest_points(x, z, y, m + 1, r, &pr1, &pr2);
	if(dr < d)
	{
		*p1 = pr1;
		*p2 = pr2;
		d = dr;
	}

	int k;
	//TODO: merge z into y
	for(yl = l, yr = m + 1, k = l; yl <= m && yr <= r; k++)
	{
		if(z[yl].pt.y <= z[yr].pt.y)
		{
			y[k] = z[yl++];
		}
		else
		{
			y[k] = z[yr++];
		}
	}
	while(yl <= m)
	{
		y[k++] = z[yl++];
	}
	while(yr <= r)
	{
		y[k++] = z[yr++];
	}

	//exclude the points far from distance d
	k = l;
	for(i = l; i <= r; i++)
	{
		if(abs(y[i].pt.x - x[m].pt.x) < d)
		{
			z[k++] = y[i];
		}
	}
	int j;
	double dist;
	for(i = l; i < k - 1; i++)
	{
		for(j = i + 1; (j < k) && ((z[j].pt.y - z[i].pt.y) < d); j++)
		{
			dist = distance(z[i].pt, z[j].pt);
			if(dist < d)
			{
				*p1 = z[i].pt;
				*p2 = z[j].pt;
				d = dist;
			}
		}
	}

	return d;
}
