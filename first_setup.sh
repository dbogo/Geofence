#!/bin/bash

# this script should be executed when the Geofence project is
# installed for the first time on any machine. It sets neccessary 
# variables and builds the needed libraries.

# TODO:
# 1. install log4c
# 2. install wiringPi
# 3. install pifacecad
# 4. set all environment variables
# 5. place start-up scripts if on a Raspberry-Pi
# 6. (*) build first-time and gather errors ?


echo -e "first_setup.sh invoked: performing first time installation.\n"

./misc/build_log4c.sh
./misc/build_pifacecad.sh
./misc/build_wiringPi.sh
./misc/serial_config.sh
./misc/wiringPi_env_setup.sh

echo -e "\nfirst_setup.sh done. Setup scripts (see misc/ ) done.\n"
echo -e "now at: ${PWD}"

