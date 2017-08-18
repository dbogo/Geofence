##
## This makefile is inspired by fiz's answer from https://stackoverflow.com/questions/14844268/makefile-template-for-large-c-project
##


PROJECT = geofence
CC 		= gcc
      
COMMANDLINE_OPTIONS = #/dev/ttyUSB0
MACROS = -DRPI_GPS -DDEBUG

COMPILE_OPTIONS = -Wall -Wextra -std=gnu11 -g

HEADERS = -Ilibs/log4c/include \
			-Ilibs/libpifacecad/src \
			-Ilibs/wiringPi/wiringPi/ \
			-Ilibs/mavlink \
			-Isrc/ \
			-Isrc/mavlink_interface/ \
			-Ilibs/ \
			-I.
			
LIBS 	= -Llibs/libpifacecad \
			-Llibs/libmcp23s17 \
			-Llibs/wiringPi/wiringPi \
			-Llibs/log4c/lib \
			-Wl,-rpath=./libs/log4c/lib/ \
			-llog4c -lm -lpifacecad -lmcp23s17 -lwiringPi

DEPENDENCY_OPTIONS = -MM

# Subdirs to search for additional source files
SUBDIRS 		= src/ src/pifaceCAD/ src/serial/ src/mavlink_interface/ src/RPiGPSDemo/ src/GPSDemo/ #$(shell ls -F src/ | grep "\/" )
DIRS 			= $(SUBDIRS)
SOURCE_FILES 	= $(foreach d, $(DIRS), $(wildcard $(d)*.c) )
OBJECTS			= $(patsubst %.c, %.o, $(SOURCE_FILES))
DEPENDENCIES 	= $(patsubst %.c, %.d, $(SOURCE_FILES))


%.d: %.c
	$(CC) $(DEPENDENCY_OPTIONS) $< -MT "$*.o $*.d" -MF $*.d

all: $(DEPENDENCIES) $(PROJECT) 

$(PROJECT): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS) $(COMPILE_OPTIONS)

# Include dependencies (if there are any)
ifneq "$(strip $(DEPENDENCIES))" ""
  include $(DEPENDENCIES)
endif


%.o: %.c
	$(CC) $(MACROS) -c $(COMPILE_OPTIONS) -o $@ $< $(HEADERS)

run: $(PROJECT)
	./$(PROJECT) $(COMMANDLINE_OPTIONS)

.PHONY: clean
clean:
	rm -f $(PROJECT) $(OBJECTS)

.PHONY: depclean
depclean:
	rm -f $(DEPENDENCIES)

clean-all: clean depclean

.PHONY: remake
remake: clean-all all
