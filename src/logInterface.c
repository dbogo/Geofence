#include "logInterface.h"

//NOTE: why did I have to make it a pointer to a pointer in order to make it work ??
//apender name is actually the stream - e.g a specific file, stdout, etc...
int initLogSystem(Log_Master* logMaster){
	int errCode = log4c_init();
	if(errCode){
		printf("log4c_init() failed.\n");
	} else {
		//FIXME: when doing Logger l = logMaster->operationLogger; everything breaks... why ??
		Logger l;// = logMaster->operationLogger;

		//Instantiate a log4c_category_t with name instanceName
		logMaster->operationLogger.logObj = log4c_category_get(logMaster->operationLogger.logInstanceName);
		log4c_category_set_appender(logMaster->operationLogger.logObj, log4c_appender_get(OPERATION_LOG_FILE));

		logMaster->errorLogger.logObj = log4c_category_get(logMaster->errorLogger.logInstanceName);
		log4c_category_set_appender(logMaster->errorLogger.logObj, log4c_appender_get(ERROR_LOG_FILE));

		//*logObj = log4c_category_get(instanceName);
		//log4c_category_set_appender(*logObj, log4c_appender_get(appenderName));
		
	}
	return errCode;
}

// format - may be just the string we want to log (the message itself)
void logEvent(log4c_category_t* logObj, int logPriority, const char* format){
	log4c_category_log(logObj, logPriority, format);
}

int finiLogSystem(void){
	int errCode = log4c_fini();
	if(errCode)
		printf("log4c_fini() failed.\n");
	return errCode;
}