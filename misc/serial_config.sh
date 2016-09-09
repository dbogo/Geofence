sudo stty -F /dev/ttyAMA0 raw 9600 cs8 clocal -cstopb
sudo stty -F /dev/ttyACM0 raw 9600 cs8 clocal -cstopb

echo -e"\tserial_config.sh: done."
