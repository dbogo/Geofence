#include "logInterface.h"

//NOTE: why did I have to make it a pointer to a pointer in order to make it work ??
//apender name is actually the stream - e.g a specific file, stdout, etc...
int initLogSystem(log4c_category_t** logObj, const char* instanceName, const char* appenderName){
	int errCode = log4c_init();
	if(errCode){
		printf("log4c_init() failed.\n");
	} else {
		//Instantiate a log4c_category_t with name instanceName
		*logObj = log4c_category_get(instanceName);
		log4c_category_set_appender(*logObj, log4c_appender_get(appenderName));
		
	}
	return errCode;
}

// format - may be just the string we want to log (the message itself)
void logEvent(log4c_category_t* logObj, int logPriority, const char* format){
	// char str[80];
	// sprintf(str, "%s", format);
	log4c_category_log(logObj, logPriority, format);
}

int finiLogSystem(void){
	int errCode = log4c_fini();
	if(errCode)
		printf("log4c_fini() failed.\n");
	return errCode;
}