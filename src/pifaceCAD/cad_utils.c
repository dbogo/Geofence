#include "cad_utils.h"

void init_cad(){
	pifacecad_open();
	pifacecad_lcd_backlight_on();
}

void print_to_cad(char* str){
	pifacecad_lcd_write(str);
}

void close_cad(){
	pifacecad_close();
}
