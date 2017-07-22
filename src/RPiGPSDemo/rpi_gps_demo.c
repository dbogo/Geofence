#include <string.h>
#include <stdio.h>

#include "rpi_gps_demo.h"
#include "parser.h"
#include <src/serial/serialInterface.h>
#include <src/logInterface.h>


int getGPSSample_RPI(FullGPSData* samp, bool passToLog, void* userData){
	char nmea[MAX_NMEA_MSG_SIZE];
    memset(nmea, '\0', MAX_NMEA_MSG_SIZE);
    fetch_sentence_from_gps(get_streamFD(), nmea);
    if(validate_checksum(nmea) != CHECKSUM_OK){
    	logEvent("checksum error. exiting getGPSSample_RPI() with CHECKSUM_ERR", LOG4C_PRIORITY_ERROR, ERROR, &logMaster);
    	return CHECKSUM_ERR;
    }

    if(passToLog){
        logEvent(nmea, LOG4C_PRIORITY_INFO, NMEA, &logMaster);    
    }

    return parse_nmea(nmea, samp);	
}
