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

cd libs/
sudo rm -rf libmcp23s17 libpifacecad log4c wiringPi
cd ..

sudo ./misc/build_log4c.sh
sudo ./misc/build_pifacecad.sh
sudo ./misc/build_wiringPi.sh
sudo ./misc/serial_config.sh
sudo ./misc/wiringPi_env_setup.sh

echo -e "\nfirst_setup.sh done. Setup scripts (see misc/ ) done.\n"
echo -e "now at: ${PWD}"

