
clear
clear

#compile libraries with position independant code
gcc -c -fpic RPiGPSDemo/src/*.c -o RPiGPSDemo/build/rpi_gps_demo.o
gcc -c -fpic GPSDemo/src/*.c -o GPSDemo/build/gps_demo.o

#create shared libraries from object file
#RPiGPSDemo
gcc -shared -o RPiGPSDemo/dist/Debug/GNU-Linux/libRPiGPSDemo.so RPiGPSDemo/build/rpi_gps_demo.o

#GPSDemo
gcc -shared -o GPSDemo/dist/Debug/GNU-Linux/libGPSDemo.so GPSDemo/build/gps_demo.o


main_exec_location=dist/Debug/GNU-Linux/geofence

#=============================================
#now the source files:
#main src/
main_src=src/*.c

#additional different source files that are needed (e.g serialInterface.c, cad_utils.c etc)
more_sources1=src/serial/serialInterface.c
more_sources2=src/pifaceCAD/cad_utils.c
#=============================================

#libraries:
RPiGPSDemo_so=-LRPiGPSDemo/dist/Debug/GNU-Linux
GPSDemo_so=-LGPSDemo/dist/Debug/GNU-Linux
LOG4C_so=-Llog4c/lib
PIFACECAD_so="-Lpifacecad/libpifacecad/ -Lpifacecad/libmcp23s17/"
wiringPi_o=-LwiringPi/wiringPi/

#=============================================

#libraries' sources:
LOG4C_src=-Ilog4c/include
PIFACECAD_src=-Ipifacecad/libpifacecad/src/
wiringPi_src=-IwiringPi/wiringPi/

#=============================================

#now combine all:
ALL_SRC="${main_src} ${more_sources1} ${more_sources2}"
ALL_LIBS="${GPSDemo_so} ${LOG4C_so} ${PIFACECAD_so} ${WiringPi_o}"
ALL_LIBS_SRC="${LOG4C_src} ${PIFACECAD_src} ${WiringPi_src}"

#=============================================

#linker options:
GPSDemo_rpath=-rpath=./GPSDemo/dist/Debug/GNU-Linux
LOG4C_rpath=-rpath=./log4c/lib
 
#=============================================

#libraries' names:
libnames="-lGPSDemo -llog4c -lm -lpifacecad -lmcp23s17 -lwiringPi"

gcc -o ${main_exec_location} ${ALL_SRC} ${ALL_LIBS} ${ALL_LIBS_SRC} -Wl,${GPSDemo_rpath} -Wl,${LOG4C_rpath} -Wall ${libnames}
