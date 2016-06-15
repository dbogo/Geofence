#include "cad_utils.h"

int main(int argc, char** argv){
	init_cad();
	print_to_cad("CAD TEST\ncad_test");
	close_cad();
	return 0;
}
