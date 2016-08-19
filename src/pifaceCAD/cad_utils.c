#include "cad_utils.h"

int init_cad(){
	int rv = pifacecad_open();
	if(rv == -1)
		return -1;
	else 
		pifacecad_lcd_backlight_on();
	
	return rv;
}


int print_to_cad(char* str){
	//lcd_write() returns the current cursor address
	return pifacecad_lcd_write(str);
}

void close_cad(){
	pifacecad_close();
}

void clear_cad(){
	pifacecad_lcd_clear();
}
