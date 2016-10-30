# Builds the wiringPi library

cd libs/
git clone git://git.drogon.net/wiringPi
cd wiringPi/
sudo ./build
cd ../../
echo -e "\tbuild_wiringPi.sh finished at: ${PWD}"
