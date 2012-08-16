// FindClosestPoints.cpp: implementation of the CFindClosestPoints class.
//
//////////////////////////////////////////////////////////////////////
#include "config.h"
#include "FindClosestPoints.h"
#include "MyUtil.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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
	return pt1->pt.y - pt2->pt.y;
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
	qsort(y, n, sizeof(PointEx), CFindClosestPoints::compare_y);

	PointEx* z = new PointEx[n];
	//3. copy y into z
	for(i = 0; i < n; i++)
	{
		z[i] = y[i];
	}

	double d = closest_points(x, y, z, 0, n - 1, p1, p2);

	delete [] z;
	delete [] y;
	delete [] x;
	delete [] posBuffer;

	return d;
}

void CFindClosestPoints::merge(PointEx src[], PointEx dst[], int l, int m, int r)
{
	int yl, yr, k;
	for(yl = l, yr = m + 1, k = l; yl <= m && yr <= r; k++)
	{
		if(src[yl].pt.y <= src[yr].pt.y)
		{
			dst[k] = src[yl++];
		}
		else
		{
			dst[k] = src[yr++];
		}
	}
	while(yl <= m)
	{
		dst[k++] = src[yl++];
	}
	while(yr <= r)
	{
		dst[k++] = src[yr++];
	}
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
		return d;
	}
	if(r - l == 2)
	{
		int i, j;
		d = -1;
		double dist = 0;
		for(i = l; i < r; i++)
		{
			for(j = i + 1; j <= r; j++)
			{
				dist = distance(x[i].pt, x[j].pt);
				if(d < 0 || dist < d)
				{
					d = dist;
					*p1 = x[i].pt;
					*p2 = x[j].pt;
				}
			}
		}
		return d;
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

	//merge z into y
	merge(z, y, l, m, r);

	//exclude the points far from distance d
	int k = l;
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

double CFindClosestPoints::closest_points_n2(Point points[], int n, Point* p1, Point* p2)
{
	double d = -1, dist;
	int i, j;
	for(i = 0; i < n - 1; i++)
	{
		for(j = i + 1; j < n; j++)
		{
			dist = distance(points[i], points[j]);
			if(d < 0 || dist < d)
			{
				d = dist;
				*p1 = points[i];
				*p2 = points[j];
			}
		}
	}
	return d;
}

void testFindClosestPoints()
{
#if FIND_CLOSEST_POINTS == 1
	int i, j, test_count = 10;
	bool is_rand = true;
	const int n = 20;
	int buf[n * 2];

	//non-random
	int x[n] = {-5, 0, 2, 5, 8, 8, 8, /*10, 10, 10,*/ 10, 10, 10, 12, 12, 12, 15, 19, 25, 30, 55, 66, 77};
	int y[n] = {-3, 9, 6, 1, 8, 10, 12, /*8, 10, 12,*/ 8, 10, 12, 8, 10, 12, 1, 26, 45, 39, -55, -66, -77};
	
	for(j = 0; j < test_count; j++)
	{
		if(is_rand)
		{
			genrand(buf, n * 2, 100, j);
			for(i = 0; i < n; i++)
			{
				x[i] = buf[i];
			}
			
			for(i = 0; i < n; i++)
			{
				y[i] = buf[n + i];
			}
		}
		CFindClosestPoints::Point points[n];
		for(i = 0; i < n; i++)
		{
			points[i].x = x[i];
			points[i].y = y[i];
		}

		for(i = 0; i < n; i++)
		{
			if(i > 0 && i % 5 == 0)
			{
				printf("\n");
			}
			printf("(%d,%d) ", points[i].x, points[i].y);
		}
		printf("\n");

		CFindClosestPoints::Point p1, p2;
		double d1 = CFindClosestPoints::closest_points(points, n, &p1, &p2);
		printf("%f, p1=(%d,%d), p2=(%d,%d)\n", d1, p1.x, p1.y, p2.x, p2.y);

		CFindClosestPoints::Point p3, p4;
		double d2 = CFindClosestPoints::closest_points_n2(points, n, &p3, &p4);
		printf("%f, p1=(%d,%d), p2=(%d,%d)\n", d2, p3.x, p3.y, p4.x, p4.y);
		
		if(CFindClosestPoints::abs(d1 - d2) < 0.000001)
		{
			printf("%2d: [Y]\n", j);
		}
		else
		{
			printf("%2d: [N]\n", j);
		}

		if(!is_rand)
		{
			break;
		}
	}

	printSep(__FILE__);
#endif
}
