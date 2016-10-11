#include <time.h>
#include <stdlib.h>
#include <stdio.h>



typedef struct Point {
	int x, y;
} Point;

float det(Point p1, Point p2, Point R){
	return (p1.x - R.x)*(p2.y - R.y) - (p2.x - R.x)*(p1.y - R.y);
}

int wn_PnPoly(Point R, Point* zone, int n){
	int w = 0; // the winding number
	
	for(int i = 0; i < n; i++){
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

int main(int argc, char** argv){
	srand(time(NULL)); //seed

	Point p[200][30];
	Point R = { rand()%40000, rand()%40000 };
	printf("R: {%d,%d} \n", R.x, R.y);

	printf("random polygon:");
	for(int i = 0; i < 200; i++){
		for(int j = 0; j < 29; j++){
			p[i][j].x = (rand()%(200 * (i+1))) + (200*i); // n%0 is undefined (floating point exeption)
			p[i][j].y = (rand()%(200 * (i+1))) + (200*i);
			//printf("P[%d,%d] = (%d,%d)\n", i, j, p[i][j].x, p[i][j].y);
		}
		p[i][29] = p[i][0];
	}

	printf("done\n");

	clock_t start, end;
	double dt;

	start = clock();

	int count = 0;
	for(int i = 0; i < 200; i++){
		if(wn_PnPoly(R, p[i], 30) != 0)
			count++;
	}

	end = clock();
	dt = (double)(end - start)/CLOCKS_PER_SEC;

	printf("PiP detections: %d | in: %lf \n", count, dt);

	return 0;
}