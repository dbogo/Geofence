#include "../../libpifacecad/src/pifacecad.h"

int main(int argc, char** argv){
	pifacecad_open();
	
	pifacecad_lcd_backlight_on();
	pifacecad_lcd_write("RPI CAD TEST\nMULTI LINE");

	pifacecad_close();
	return 0;
}
