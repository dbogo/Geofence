#include <stdio.h>

/**
 * @brief      converts the value that's recieved from the NMEA messages,
 *             from degrees to decimal degrees.
 *             
 *             e.g. an NMEA message could return the following string:
 *             		3218.0489, N, 6447.5086, W
 *             		to turn those values into decimal degrees values, the following is done:
 *             		
 *             		32 18.0489 N = 32 degrees + 18.0489 / 60 = 32.300815 N
 *               	64 47.5086 W = 64 degrees + 47.5086 / 60 = 64.79181 W
 *               	
 *               	The resulting values can be used in Google Maps to get the actual point.

 * @param[in]  x the value that's recieved from the NMEA message. (e.g 3218.0489 in the example above)
 *
 * @return     the decimal degrees value, that can be used in Google Maps.
 */
double dec_to_deg(double x);

int main(int argc, char** argv){
	double lat = 4209.2087;
	double lon = 01901.8800;

	double dec_deg_lat = dec_to_deg(lat);
	double dec_deg_lon = dec_to_deg(lon);

	printf("NMEA values:\n");
	printf("\tlat: %lf | lon: %lf\n", lat, lon);
	printf("Decimal Degrees values:\n");
	printf("\tlat: %lf | lon: %lf\n", dec_deg_lat, dec_deg_lon);
	return 0;
}

double dec_to_deg(double x){
	int rounded_x = x/100; // get the 2 most significant numbers of the value
	double right_of_point = x/100 - rounded_x; //get the value to the right of the decimal point.
	double deg = rounded_x + (right_of_point * 100 / 60); // the final value.
	return deg;	                                              	        
}