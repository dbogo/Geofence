#this script build the libpifacecad library together with 
#the libmcp23s17 library that is also needed.

#use this for first time installation of the project, as the libraries should
#be recompiled on every platform !

cd external/libmcp23s17/ && make && cd -
echo -e "\tbuild_pifacecad.sh finished buildng bcmp23s17. now at ${PWD}"
cd external/libpifacecad/ && make && cd -
echo -e "\tbuild_pifacecad.sh finished buildng libpifacecad. now at ${PWD}"

cp -r external/libmcp23s17/ libs/
cp -r external/libpifacecad/ libs/
