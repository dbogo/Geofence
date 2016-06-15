#this script build the libpifacecad library together with 
#the libmcp23s17 library that is also needed.

#use this for first time installation of the project, as the libraries should
#be recompiled on every platform !

bmcp23s17/ && make && cd -
cd libpifacecad/ && make
