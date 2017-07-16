# Geofence

### Warning: The software is not ready for use in flight yet and some essential functionality is still developed.

--------------------------------------------------------------------------------------------------------

Geofence provides PX4/Pixhawk based autopilots with Geo-fencing capabilities by 
utilizing an external computer that's onboard the drone together with the flight controller.

Users can feed custom geofences to the program, by specifying multiple coordinates that enclose a geographical area and the external computer will make sure that the drone stays within the defined boundries, or out of restricted areas.

Geofence communicates with a drone's flight controller via the [MAVLink Protocol](http://qgroundcontrol.org/mavlink/start) over a physical serial connection (e.g an FTDI cable etc.) and continuously receives flight data from the autopilot. In case of a geofence violation, based on location information from a GPS receiver that's connected to the Autopilot or the external computer itself, the program immediately takes over the control of the drone by switching the PX4 Autopilot to ['Offboard Mode'](https://dev.px4.io/en/concept/flight_modes.html) and executing the necessary commands to return to a safe area/land/get away from the border etc. In addition, the external computer will disarm the drone to prevent it from taking off if it's detected to be in a no-fly zone.
______________________________________

## Pre-requisites
The software is mainly intended for use with the Pixhawk Flight Controller running the [PX4 Autopilot](http://px4.io/), thus:
- The [Pixhawk](https://pixhawk.org/) Flight Controller. Installation instructions for PX4 are [here](https://www.gitbook.com/book/px4/px4-user-guide/details). 
- A computer running any \*nix based OS, small enough to fit on a drone. The reference implementation assumes a Raspberry-Pi computer running the lastest [Raspbian OS](https://www.raspberrypi.org/downloads/raspbian/) and this is the recommended (although not mandatory option).
- Means of connecting the flight controller with the external computer. Preferrably an FTDI cable (by default PX4 will not fly with a USB connection as it's an unsafe for flight).
- GPS module (should be connected to the external computer), if one is not used with the Pixhawk.



## Installation and Building

After cloning the repository to the external computer:
```
cd Geofence/
./setup.sh
make
```
The setup script downloads and installs the dependencies. The script may ask for the root password at one point.


## Hardware Setup

1. If the GPS is intended to be connected to the Pixhawk, proceed to step 2. Else, Connect the GPS to the Raspberry Pi as shown [here](http://blog.whatgeek.com.pt/2015/03/connect-a-gps-to-the-raspberry-pi/). In general, the TX and RX pins of the GPS should be connected to the pairing RX,TX pins of the UART interface on the RaspberryPi's GPIO array (RX to TX and vice versa). ***Always*** connect the Ground (GND) first. Note that GPS modules differ in operating voltage. Check the manufacturer of your GPS to make sure what is the operating voltage (Commonly 3.3 or 5 Volts). A voltage regulator may be needed.
2. Connect the USB to FTDI cable to Pixhawks Telem2 according to this table:

| TELEM2  | FTDI |
|---------|------|
| +5V     | N/C  |
| TX (Out)| RX (Yellow) |
| RX (In) | TX (Orange) |
| CTS     | N/C |
| RTS     | N/C |
| GND     | GND (Black) |
