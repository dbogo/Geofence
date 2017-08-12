#include <string.h>
#include <stdio.h>

#include "rpi_gps_demo.h"
#include "parser.h"
#include <src/serial/serialInterface.h>
#include <src/logInterface.h>


int getGPSSample_RPI(FullGPSData* samp, bool passToLog){
	char nmea[MAX_NMEA_MSG_SIZE];
    memset(nmea, '\0', MAX_NMEA_MSG_SIZE);
    fetch_sentence_from_gps(get_streamFD(), nmea);
    if(validate_checksum(nmea) != CHECKSUM_OK){
    	log_err(&logMaster, "checksum error. exiting getGPSSample_RPI() with CHECKSUM_ERR");
    	return CHECKSUM_ERR;
    }

    if(passToLog){
        log_info(&logMaster, nmea);    
    }

    return parse_nmea(nmea, samp);	
}
