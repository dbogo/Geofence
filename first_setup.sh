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


echo -e "first_setup.sh: performing first time installation.\n"

echo -e "cloning pifacecad and mcp23s17 libraries to libs/"
cd libs/
git clone https://github.com/piface/libmcp23s17.git
git clone https://github.com/piface/libpifacecad.git
echo -e "building libmcp23s17"
cd libmcp23s17/ && make && cd -
echo -e "Done"
echo -e "building libpifacecad"
cd libpifacecad/ && make && cd -
echo -e "Done\n"
cd ..


echo -e "Installing log4c"
cd external/
echo -e "Unpacking"
tar -zxvf log4c-1.2.4.tar.gz
mkdir build && cd build
echo -e "running ./configure"
../log4c-1.2.4/configure --prefix=$PWD/../../libs/log4c/
echo -e "building"
make > /dev/null
make install > /dev/null
echo -e "Cleaning up..."
cd ../../
rm -rf external/build log4c-1.2.4
echo -e "Done\n"


echo -e "Installing WiringPi"
cd libs/
git clone git://git.drogon.net/wiringPi
cd wiringPi
git pull origin
./build
cd ../../
export WIRINGPI_CODES=1
echo -e "Done\n"


echo -e "Configuring serial port settings"
sudo stty -F /dev/ttyAMA0 raw 9600 cs8 clocal -cstopb
sudo stty -F /dev/ttyACM0 raw 9600 cs8 clocal -cstopb
echo -e "Done\n"


# cd libs/
# sudo rm -rf libmcp23s17 libpifacecad log4c wiringPi
# cd ..

# sudo ./misc/build_log4c.sh
# sudo ./misc/build_pifacecad.sh
# sudo ./misc/build_wiringPi.sh
# sudo ./misc/serial_config.sh
# sudo ./misc/wiringPi_env_setup.sh

# echo -e "\nfirst_setup.sh done. Setup scripts (see misc/ ) done.\n"
# echo -e "now at: ${PWD}"
