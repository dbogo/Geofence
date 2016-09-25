# makes the shell not print empty lines after NMEA sentences and this also makes
# the NMEA log files be written without blank lines.
sudo stty -F /dev/ttyAMA0 raw 9600 cs8 clocal -cstopb
sudo stty -F /dev/ttyACM0 raw 9600 cs8 clocal -cstopb

echo -e "\tserial_config.sh: done."
