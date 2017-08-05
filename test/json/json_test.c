#include "../../libs/cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>

int main(){

	char *json = NULL;
	long length;
	FILE *f = fopen("../../data/sample_geofence.json", "r");
	printf("opened file\n");

	if(!f){
		perror("Error: ");
	}

	fseek(f, 0, SEEK_END);
	length = ftell(f);
	fseek(f, 0, SEEK_SET);
	json = malloc(length);

	if(!json){
		perror("Error: ");
	}

	fread(json, sizeof(char), length, f);
	printf("got file contents\n");

	fclose(f);

	printf("%s\n", json);

	//-----------------------

	cJSON *root = cJSON_Parse(json);
	cJSON *format = cJSON_GetArrayItem(root, "points");


	return 0;
}