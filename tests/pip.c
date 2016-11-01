#include <stdio.h>

struct Point {
	float x;
	float y;
};
typedef struct Point Point;

float det(Point p1, Point p2, Point R);
void winding(Point* p, Point R);

#define SIZE 4

/*****************************************/

int main(int argc, char** argv){
	Point p[SIZE];

	Point tmp;

	

	tmp.x = 10.0; tmp.y = 10.0;
	p[0] = tmp;
	
	tmp.x = 0.0; tmp.y = 10.0;
	p[1] = tmp;

	tmp.x = 0.0; tmp.y = 0.0;
	p[2] = tmp;

	p[3] = p[0];

	

	


	Point R = {2.0, 1.0};
	winding(p, R);
	return 0;
}

void winding(Point p[], Point R){
	int w = 0;
	for(int i = 0; i < SIZE-1; i++){
		if((p[i].y < R.y && p[i+1].y >= R.y) || (p[i].y >= R.y && p[i+1].y < R.y)){
			if((det(p[i], p[i+1], R) > 0 && p[i+1].y > p[i].y) || 
				(det(p[i], p[i+1], R) < 0 && p[i+1].y < p[i].y)){
				if(p[i+1].y > p[i].y)
					w++;
				else 
					w--;
			}
		}
	}

	if(w != 0)
		printf("R(%f,%f) inside\n", R.x, R.y);
	else
		printf("R(%f,%f) outside\n", R.x, R.y);
}

float det(Point p1, Point p2, Point R){
	float d = (p1.x - R.x)*(p2.y-R.y) - (p2.x- R.x)*(p1.y-R.y);
	printf("d: %f\n", d);
	return d;
}