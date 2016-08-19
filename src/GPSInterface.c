// NOTE: maybe this file should be named differently !

#include "GPSInterface.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

int areIntersecting(Segment side1, Segment side2){
    float d1, d2;
    float a1, a2, b1, b2, c1, c2;

    // Convert vector 1 to a line (line 1) of infinite length.
    // We want the line in linear equation standard form: A*x + B*y + C = 0
    // See: http://en.wikipedia.org/wiki/Linear_equation
    a1 = side1.p2.latitude - side1.p1.latitude;
    b1 = side1.p1.longitude - side1.p2.longitude;
    c1 = (side1.p2.longitude * side1.p1.latitude) - (side1.p1.longitude * side1.p2.latitude);

    // Every point (x,y), that solves the equation above, is on the line,
    // every point that does not solve it, is not. The equation will have a
    // positive result if it is on one side of the line and a negative one 
    // if is on the other side of it. We insert (x1,y1) and (x2,y2) of vector
    // 2 into the equation above.
    d1 = (a1 * side2.p1.longitude) + (b1 * side2.p1.latitude) + c1;
    d2 = (a1 * side2.p2.longitude) + (b1 * side2.p2.latitude) + c1;

    // If d1 and d2 both have the same sign, they are both on the same side
    // of our line 1 and in that case no intersection is possible. Careful, 
    // 0 is a special case, that's why we don't test ">=" and "<=", 
    // but "<" and ">".
    if (d1 > 0 && d2 > 0) return 0;
    if (d1 < 0 && d2 < 0) return 0;

    // The fact that vector 2 intersected the infinite line 1 above doesn't 
    // mean it also intersects the vector 1. Vector 1 is only a subset of that
    // infinite line 1, so it may have intersected that line before the vector
    // started or after it ended. To k0w for sure, we have to repeat the
    // the same test the other way round. We start by calculating the 
    // infinite line 2 in linear equation standard form.
    a2 = side2.p2.latitude - side2.p1.latitude;
    b2 = side2.p1.longitude - side2.p2.longitude;
    c2 = (side2.p2.longitude * side2.p1.latitude) - (side2.p1.longitude * side2.p2.latitude);

    // Calculate d1 and d2 again, this time using points of vector 1.
    d1 = (a2 * side1.p1.longitude) + (b2 * side1.p1.latitude) + c2;
    d2 = (a2 * side1.p2.longitude) + (b2 * side1.p2.latitude) + c2;

    // Again, if both have the same sign (and neither one is 0),
    // 0 intersection is possible.
    if (d1 > 0 && d2 > 0) return 0;
    if (d1 < 0 && d2 < 0) return 0;

    // If we get here, only two possibilities are left. Either the two
    // vectors intersect in exactly one point or they are collinear, which
    // means they intersect in any number of points from zero to infinite.
    if ((a1 * b2) - (a2 * b1) == 0.0f) return 2;

    // If they are not collinear, they must intersect in exactly one point.
    return 1;
}


//TODO: ray, and sides like (x1/y1)-(x2/y2)
//TODO: all the sides should be precomputed and stores in memory rather than here.

void create_segments_of_zone(void){
	double Xmin = 190, Xmax = -1;
	double Ymin = 100, Ymax = -1;

	int i;
	for(i = 0; i < zone.numVertices; i++){
		//printf("x%d: %f, y%d: %f || ", i, zone.vertices[i].longitude, i, zone.vertices[i].latitude);
		if(zone.vertices[i].longitude < Xmin)
			Xmin = zone.vertices[i].longitude;
		if(zone.vertices[i].latitude < Ymin)
			Ymin = zone.vertices[i].latitude; 
	}
	for(i = 0; i < zone.numVertices; i++){
		if(zone.vertices[i].longitude > Xmax)
			Xmax = zone.vertices[i].longitude;
		if(zone.vertices[i].latitude > Ymax)
			Ymax = zone.vertices[i].latitude; 
	}
	//printf("xmin: %f, xmax: %f, ymin: %f, ymax: %f\n", Xmin, Xmax, Ymin, Ymax);
	double e = ((Xmax - Xmin) / 100);

	//Segment sides[zone.numVertices];
	//double sidesCalc[zone.numVertices];
	for(i = 0; i < zone.numVertices-1; i++){
		sides[i].p1.longitude = zone.vertices[i].longitude;
		sides[i].p1.latitude = zone.vertices[i].latitude;

		sides[i].p2.longitude = zone.vertices[i+1].longitude;
		sides[i].p2.latitude = zone.vertices[i+1].latitude;
		//sidesCalc[i] = (sides[i].p1.longitude/sides[i].p1.latitude) - 
		//(sides[i].p2.longitude/sides[i].p2.latitude);
	}

	sides[zone.numVertices-1].p1.longitude = zone.vertices[zone.numVertices-1].longitude;
	sides[zone.numVertices-1].p1.latitude = zone.vertices[zone.numVertices-1].latitude;

	sides[zone.numVertices-1].p2.longitude = zone.vertices[0].longitude;
	sides[zone.numVertices-1].p2.latitude = zone.vertices[0].latitude;
	//sidesCalc[zone->numVertices-1] = (sides[zone->numVertices-1].p1.longitude/sides[zone->numVertices-1].p1.latitude) - 
	//	(sides[0].p2.longitude/sides[0].p2.latitude);

	Segment ray1 = { .p1.longitude = sample.longitude, .p1.latitude = sample.latitude,
					.p2.longitude = Xmax + e, .p2.latitude = sample.latitude };
	printf("%d\n", &ray1);

	//ray = tmpRay;


}

bool isSampleInRangeGeneral(GPSSamp* samp, Zone_general* zone_gen){
	clock_t start, end;
	double cpu_time_used;
	start = clock();

	// find X,Y min and max each.

	/*printf("1: (%f, %f), 2: (%f, %f), 3: (%f, %f), 4: (%f, %f)\n", zone_gen->vertices[0].longitude, 
										zone_gen->vertices[0].latitude,
										zone_gen->vertices[1].longitude, zone_gen->vertices[1].latitude,
										zone_gen->vertices[2].longitude, zone_gen->vertices[2].latitude,
										zone_gen->vertices[3].longitude, zone_gen->vertices[3].latitude
			);*/

	

	//double rayCalc = ( ray.p1.longitude/ray.p1.latitude) - (ray.p2.longitude + ray.p2.latitude);
	
	/* test intersections */
	int intersections = 0;
	int i = 0;
	for(i = 0; i < zone_gen->numVertices; i++){
		//if(areIntersecting(ray, sides[i]))
			intersections++;
	}

	end = clock();
	cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC;

	printf("clock: %f\n", cpu_time_used);
	if((intersections % 2) == 1)
		return true; // inside polygon
	return false; //outside
}	





#if 0

int isSampleInRangeGeneral1(Zone_general* zone, int numVertices, GPSSamp* sample){
	//TODO: understand this stuff, delete the comment and commit
	int counter = 0;
	int i;
	double xinters;
	GEO_Point p1,p2;
	// the test point
	GEO_Point p = {  .longitude = sample->longitude, .latitude = sample->latitude};
	
	p1 = zone->vertices[0];
	for (i = 1; i <= numVertices; i++) {
		p2 = zone->vertices[i % numVertices];
		if (p.latitude > min(p1.latitude, p2.latitude)){
			if(p.latitude <= max(p1.latitude, p2.latitude)) {
    			if (p.longitude <= min(p1.longitude,p2.longitude)) {
      				if (p1.latitude != p2.latitude) {
        				xinters = (p.latitude-p1.latitude) * (p2.longitude-p1.longitude) / (p2.latitude-p1.latitude) + p1.longitude;
        				if (p1.longitude == p2.longitude || p.longitude <= xinters)
          					counter++;
      			}
    		}
	  	}
		}
		p1 = p2;
	}

	if (counter % 2 == 0)
		return(OUTSIDE);
	return(INSIDE);
}


#if 0
/* This function checks whether a point is inside an N-sides, general
	case polygon, using a version of the ray-casting algorithm.
	see: https://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html */
bool isSampleInRangeGeneral(GPSSamp* samp, Zone_general* zone_gen){
	double vertx[zone_gen->numVertices];
	double verty[zone_gen->numVertices];
	
	int i;
	for(i = 0; i < zone_gen->numVertices; i++){
		vertx[i] = zone_gen->vertices[i].longitude;
		verty[i] = zone_gen->vertices[i].latitude;
	}

	if(pnpoly(zone_gen->numVertices, vertx, verty, samp->latitude, samp->longitude))
		return true; // if returned value is 1 (odd, then inside)
	return false;
}

int pnpoly(int nvert, double *vertx, double *verty, double testx, double testy){
	int i, j, c = 0;
  	for (i = 0, j = nvert-1; i < nvert; j = i++) {
    	if ( ((verty[i]>testy) != (verty[j]>testy)) &&
	 	(testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       		c = !c; // c alternates between 1 and 0 - even and odd.
  	}
  	return c;
}
#endif

bool isDroneGoingOffBorder(GPSSamp* samp, Zone_general* zone_gen){

	//TODO: maybe estimate for more than 1 step ahead.
	//		introduce some margin for error
	//		* maybe alert levels, etc..

	GPSSamp estimatedNextSamp = {
		.altitude = samp->altitude,
		.latitude = samp->latitude + (samp->speed * sin(samp->course * PI/180)),
		.longitude = samp->longitude + (samp->speed * cos(samp->course * PI/180)),
		.course = samp->course,
		.speed = samp->speed };


	// check if the drone's next estimated position is within the borders 
	GEO_Point p = { .latitude = estimatedNextSamp.latitude,
					.longitude = estimatedNextSamp.longitude };
	if(isSampleInRangeGeneral1(zone_gen, zone_gen->numVertices, &estimatedNextSamp)){	
		return false;
	}

	return true;
}

 #endif
