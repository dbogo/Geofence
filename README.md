#Geofence
Geofence is a software that provides drones with Geo-fencing capabilities, (in addition to other things in the future).
It's initially designed and aimed for the Raspberry-Pi series of SBC's
(development for other boards/flight-controllers is possible in the future).
______________________________________
####The software is not ready for use on real equipment yet!
______________________________________

This project is developed on an Ubuntu 64-bit machine, and then ported and tested on the Raspberry-Pi 2.
Mikroelctronica's 'gps click' module is used for the gps. ([available here] (http://www.mikroe.com/click/gps/))
______________________________________

An emphasis is put on keeping the use of third-part libraries to a minimum. However, a few libraries are used:
- **log4c** - A logging library ([link](http://log4c.sourceforge.net/))
- **pifacecad** - A library for the 'Piface Control & Display', used for debugging/development. ([library](https://github.com/piface/libpifacecad)) and ([device](http://www.piface.org.uk/products/piface_control_and_display/))

