#include "csrshape.h"

void ellipsePlot(int xc, int yc, int x, int y)
{
	
}

/*
	ªÊ÷∆Õ÷‘≤À„∑®
	x2/a2 + y2/b2 = r2
*/
void drawEllipse(int xc, int yc, int a, int b)
{
	double sqa = a * a;
	double sqb = b * b;

	double d = sqb + sqa * (-b + 0.25);
	int x = 0;
	int y = b;
	ellipsePlot(xc, yc, x, y);
	while (sqb * (x + 1) < sqa * (y - 0.5))
	{
		if (d < 0)
			d += sqb * (2 * x + 3);
		else
		{
			d += (sqb * (2 * x + 3) + sqa * (-2 * y + 2));
			y--;
		}
		x++;
		ellipsePlot(xc, yc, x, y);
	}

	d = (b * (x + 0.5)) * 2 + (a * (y - 1)) * 2 - (a*b) * 2;

	while (y > 0)
	{
		if (d < 0)
		{
			d += sqb * (2 * x + 2) + sqa * (-2 * y + 3);
			x++;
		}
		else
		{
			d += sqa * (-2 * y + 3);
		}
		y--;
		ellipsePlot(xc, yc, x, y);
	}
}

#define ROUND_INT(a) (int)a

//Bresenham_ellipse
void bresenhamEllipse(int xc, int yc, int a, int b)
{
	int sqa = a * a;
	int sqb = b * b;
	int x = 0;
	int y = b;
	int d = 2 * sqb - 2 * b * sqa + sqa;
	ellipsePlot(xc, yc, x, y);
	int p_x = ROUND_INT((double)sqa / sqrt((double)(sqa + sqb)));

	while (x <= p_x)
	{
		if (d < 0)
		{
			d += 2 * sqb * (2 * x + 3);
		}
		else
		{
			d += 2 * sqb * (2 * x + 3) - 4 * sqa * (y - 1);
			y--;
		}
		x++;
		ellipsePlot(xc, yc, x, y);
	}

	d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
	while (y >= 0)
	{
		ellipsePlot(xc, yc, x, y);
		y--;
		if (d < 0)
		{
			x++;
			d = d - 2 * sqa * y - sqa + 2 * sqb * x + 2 * sqb;
		}
		else
		{
			d = d - 2 * sqa * y - sqa;
		}
	}
}