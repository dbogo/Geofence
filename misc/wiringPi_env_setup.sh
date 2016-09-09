#!/bin/bash

#set this environment variable to restore wiringPi's v1 behavior
#of returning DIFFERENT values upon call to wiringPiSetup().
#we need this in order to check for successful wiringPi init 
#(so that this call won't terminate the program when NOT run on the 
#Raspberry-Pi).
export WIRINGPI_CODES=1

echo -e "\twiringPi_env_setup.sh: done"
