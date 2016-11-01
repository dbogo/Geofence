#include <stdio.h>

typedef struct Point{
	float x, y;
}  Point;

int wn_PnPoly(Point R, Point* zone, int n);
float det(Point p1, Point p2, Point R);

int main(int argc, char** argv){
	const int size = 5;
	Point p[size];
	Point R = { 3.5, 3 };

	/*
	//cw
	printf("cw\n");
	p[0].x = 10.098; p[0].y=0.9;
	p[1].x = 1.1; p[1].y=1.0;
	p[2].x = 1.0; p[2].y=10.7;
	p[3].x = 10.0; p[3].y=10.0;
	p[4] = p[0];
	*/

	/*
	//ccw
	printf("ccw\n");
	p[0].x = 10.098; p[0].y=0.9;
	p[1].x = 11.2; p[1].y=10.0;
	p[2].x = 1.0; p[2].y=10.7;
	p[3].x = 0.9; p[3].y=1.0;
	p[4] = p[0];
	*/

	
	printf("sandwatch\n");
	p[0].x = 10.098; p[0].y=0.9;
	p[1].x = 1.1; p[1].y=1.0;
	p[2].x = 10.7; p[2].y=10.7;
	p[3].x = 1.1; p[3].y=10.0;
	p[4] = p[0];
	

	printf("%d\n", wn_PnPoly(R, p, size));

	return 0;
}	

int wn_PnPoly(Point R, Point* zone, int n){
	int w = 0; // the winding number
	
	for(int i = 0; i < n-1; i++){
		printf("&i:%f, &i+1:%f\n", zone[i].y, zone[i+1].y);
		if((zone[i].y < R.y && zone[i+1].y >=R.y) ||
			(zone[i].y >= R.y && zone[i+1].y < R.y)){
				if(((det(zone[i], zone[i+1], R) > 0) && zone[i+1].y > zone[i].y) ||
					((det(zone[i], zone[i+1], R) < 0) && zone[i+1].y < zone[i].y)){
					if(zone[i+1].y > zone[i].y) w++;
					else w--;
				}
			}
	}

	//printf("wn: %d\n", w);
	return w;
}

float det(Point p1, Point p2, Point R){
	return (p1.x - R.x)*(p2.y - R.y) - (p2.x - R.x)*(p1.y - R.y);
}