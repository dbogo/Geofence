
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

gcc -o dist/Debug/GNU-Linux/geofence src/*.c src/serial/serialInterface.c src/pifaceCAD/cad_utils.c -LGPSDemo/dist/Debug/GNU-Linux -Llog4c/lib -Ilog4c/include -Lpifacecad/libpifacecad/ -Lpifacecad/libmcp23s17/ -Ipifacecad/libpifacecad/src/ -Wl,-rpath=./GPSDemo/dist/Debug/GNU-Linux -Wl,-rpath=./log4c/lib -Wall -lGPSDemo -dynamic -llog4c -dynamic -lm -lpifacecad -lmcp23s17
#gcc -o dist/Debug/GNU-Linux/geofence src/*.c -LGPSDemo/dist/Debug/GNU-Linux -Llog4c/lib -Ilog4c/include -Wl,-rpath=/home/bogo/Desktop/Geofence/GPSDemo/dist/Debug/GNU-Linux -Wl,-rpath=/home/bogo/Desktop/Geofence/log4c/lib -Wall -lGPSDemo -dynamic -llog4c -dynamic -lm
#gcc -o dist/Debug/GNU-Linux/geofence src/*.c -LRPiGPSDemo/dist/Debug/GNU-Linux -LGPSDemo/dist/Debug/GNU-Linux -Llog4c/lib -Ilog4c/include -Wl,-rpath=/home/bogo/Desktop/Geofence/RPiGPSDemo/dist/Debug/GNU-Linux -Wl,-rpath=/home/bogo/Desktop/Geofence/log4c/lib -Wall -lRPiGPSDemo -dynamic -lGPSDemo -dynamic -llog4c -dynamic -lm

rm cad_utils.*
rm serialInterface.*
rm main.*
rm GPSInterface.*
rm logInterface.*
