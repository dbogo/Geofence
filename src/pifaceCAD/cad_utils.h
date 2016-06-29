#include "../../libpifacecad/src/pifacecad.h"

/* cad means CONTROL AND DISPLAY. this abbreaviation will be used throughout 
 * the files containing code related to the Piface Control & Display module.
 *
 * The functions in this header are a wrapper of the functionality that's 
 * provided by the libpifacecad.
 * 
 */

void init_cad();
void print_to_cad(char* str);
void close_cad();