/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GPSInterface.h
 * Author: bogo
 *
 * Created on March 26, 2016, 4:27 PM
 */

#ifndef GPSINTERFACE_H
#define GPSINTERFACE_H

#include "types.h"

#define FULL_SAMPLE	1
#define REGISTERED_GGA 2
#define REGISTERED_RMC 3
#define UNRECOGNIZED_NMEA_FORMAT 9


// 'generic' function of a GPS sample, implemented by different libraries
extern int getGPSSample(GPSSamp* samp);

#endif /* GPSINTERFACE_H */

