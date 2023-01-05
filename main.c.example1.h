#include <stdlib.h>
#include <stdio.h>

#define types \
Type(Point, uint x; uint y; ) \
Type(Line, Point p1; Point p2; )

#include "types.h"

int main() {
	Point point1 = new(Point);
	Point point2 = new(Point);
	Line line = new(Line);
	
	point1->x = 100;
	point1->y = 200;
	
	point2->x = 300;
	point2->y = 400;

	line->p1 = point1;
	line->p2 = point2;
	
	printf("Print line data:\n");
	printf("Start of line (x/y): %d/%d\n", line->p1->x, line->p1->y);
	printf("End of line (x/y): %d/%d\n\n", line->p2->x, line->p2->y);

	printf("is line1 a Point? %d\n", is(line,Point));
	
	free(line);
	free(point2);
	free(point1);
	
	return 0;
}


