
clear
clear

# Get the commad-line arguments for what library to link.
# Either GPSDemo or RPiGPSDemo. val ("pi" or "demo")
link_option=$1
if [ $link_option != "pi" ] || [ $link_option != "demo" ] || [ $link_option != "" ]; then
	echo -e "build_script: uncerocnized option '${link_option}'"
	printf "build_script: default linking will be done with GPSDemo.\n\n"
	link_option=demo
fi


RPiGPSDemo_build_dir=RPiGPSDemo/build
GPSDemo_build_dir=GPSDemo/build

#TODO: differentiate between Debug and release
RPiGPSDemo_dist_dir=RPiGPSDemo/dist/Debug/GNU-Linux
GPSDemo_dist_dir=GPSDemo/dist/Debug/GNU-Linux

geofence_exec=geofence
geofence_dist_dir=dist/Debug/GNU-Linux

logs_dir=logs

#=============================================

#create dist directory for main executable
if [ ! -d ${geofence_dist_dir} ]; then
	mkdir -p ${geofence_dist_dir}
fi

#create logs directory
if [ ! -d ${logs_dir} ]; then
	mkdir -p ${logs_dir}
fi

# create build directories (for obj files) if they dont exist yet
if [ ! -d ${RPiGPSDemo_build_dir} ]; then
	mkdir -p ${RPiGPSDemo_build_dir}
fi

if [ ! -d ${GPSDemo_build_dir} ]; then
	mkdir -p ${GPSDemo_build_dir}
fi

#create dist directories (for shared libraries) if they dont exist yet
if [ ! -d ${RPiGPSDemo_dist_dir} ]; then
	mkdir -p ${RPiGPSDemo_dist_dir}
fi

if [ ! -d ${GPSDemo_dist_dir} ]; then
	mkdir -p ${GPSDemo_dist_dir}
fi

#=============================================

#TODO: maybe rpath can store the neccessary log4c locations so I dont need -I -L

#compile libraries with position independant code
pushd > /dev/null ${RPiGPSDemo_build_dir}/ #go to the place where -o will put all obj files. SUPRESS THE OUTPUT OF pushd !
gcc -c -fpic ../../RPiGPSDemo/src/*.c -I../../log4c/include 
popd > /dev/null #return to the root dir of the project. 

pushd > /dev/null ${GPSDemo_build_dir}/
gcc -c -fpic ../../GPSDemo/src/*.c -I../../log4c/include
popd > /dev/null #return to the root dir of the project. 
          
#create shared libraries from object files
#RPiGPSDemo
gcc -shared ${RPiGPSDemo_build_dir}/*.o -Llog4c/lib -llog4c -o ${RPiGPSDemo_dist_dir}/libRPiGPSDemo.so

#GPSDemo
# NOTE: GPSDemo uses <math.h>, hence -lm
gcc -shared ${GPSDemo_build_dir}/*.o -lm -o ${GPSDemo_dist_dir}/libGPSDemo.so 

main_exec_location=${geofence_dist_dir}/${geofence_exec}

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
ALL_LIBS="${RPiGPSDemo_so} ${LOG4C_so} ${PIFACECAD_so} ${WiringPi_o}"
ALL_LIBS_SRC="${LOG4C_src} ${PIFACECAD_src} ${WiringPi_src}"

#=============================================

#linker options:
RPiGPSDemo_rpath=-rpath=./RPiGPSDemo/dist/Debug/GNU-Linux
GPSDemo_rpath=-rpath=./GPSDemo/dist/Debug/GNU-Linux
LOG4C_rpath=-rpath=./log4c/lib
 
#=============================================

#libraries' names:
libnames="-llog4c -lm -lpifacecad -lmcp23s17 -lwiringPi"

if [ ${link_option} == "pi" ]; then
	libnames="${libnames} -lRPiGPSDemo"
	echo -e "libnames: $libnames\n"
	gcc -g -o ${main_exec_location} ${ALL_SRC} ${ALL_LIBS} ${ALL_LIBS_SRC} -Wl,${RPiGPSDemo_rpath} -Wl,${LOG4C_rpath} -Wall ${libnames}
else
	libnames="${libnames} -lGPSDemo"
	echo -e "libnames: $libnames\n"
	ALL_LIBS="${GPSDemo_so} ${LOG4C_so} ${PIFACECAD_so} ${WiringPi_o}"
	gcc -g -o ${main_exec_location} ${ALL_SRC} ${ALL_LIBS} ${ALL_LIBS_SRC} -Wl,${GPSDemo_rpath} -Wl,${LOG4C_rpath} -Wall ${libnames}
fi