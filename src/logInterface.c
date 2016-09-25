#include "logInterface.h"

//NOTE: why did I have to make it a pointer to a pointer in order to make it work ??
//apender name is actually the stream - e.g a specific file, stdout, etc...
int initLogSystem(Log_Master* logMaster){

	/**
	 * FIXME: if the logInstanceName attribute of the Logger struct, is set to "" in 
	 * all of the loggers, they stop working and only one logger works with only one log stream 
	 * (be it ERROR, NMEA or INFO). 
	 * find a way to get rid of this string AND keep the logs on.
	 */
	
	logMaster->operationLogger.logFile = fopen(OPERATION_LOG_FILE, "w");
	logMaster->operationLogger.logObj = NULL;
	logMaster->operationLogger.logInstanceName = "operation"; 

	logMaster->errorLogger.logFile = fopen(ERROR_LOG_FILE, "w");
	logMaster->errorLogger.logObj = NULL;
	logMaster->errorLogger.logInstanceName = "errorlog"; // e.g. 'errorlog' etc.
	                                                     // 
	logMaster->nmeaLogger.logFile = fopen(NMEA_LOG_FILE, "w");
	logMaster->nmeaLogger.logObj = NULL;
	logMaster->nmeaLogger.logInstanceName = "nmealog"; // e.g. 'nmealog' etc.

	int errCode = log4c_init();
	if(errCode){
		printf("log4c_init() failed.\n");
	} else {
		//FIXME: when doing Logger l = logMaster->operationLogger; everything breaks... why ??

		//Instantiate a log4c_category_t with name instanceName
		logMaster->operationLogger.logObj = log4c_category_get(logMaster->operationLogger.logInstanceName);
		log4c_category_set_appender(logMaster->operationLogger.logObj, log4c_appender_get(OPERATION_LOG_FILE));

		logMaster->errorLogger.logObj = log4c_category_get(logMaster->errorLogger.logInstanceName);
		log4c_category_set_appender(logMaster->errorLogger.logObj, log4c_appender_get(ERROR_LOG_FILE));

		logMaster->nmeaLogger.logObj = log4c_category_get(logMaster->nmeaLogger.logInstanceName);
		log4c_category_set_appender(logMaster->nmeaLogger.logObj, log4c_appender_get(NMEA_LOG_FILE));

		//*logObj = log4c_category_get(instanceName);
		//log4c_category_set_appender(*logObj, log4c_appender_get(appenderName));
		
	}
	/* first log message to each of the files */
	logEvent("operation log ready to use.", LOG4C_PRIORITY_INFO, INFO, logMaster);
	logEvent("error log ready to use.", LOG4C_PRIORITY_ERROR, ERROR, logMaster);
	logEvent("nmea log ready to use.", LOG4C_PRIORITY_INFO , NMEA, logMaster);

	return errCode;
}

#if 0
// format - may be just the string we want to log (the message itself)
void logEvent(log4c_category_t* logObj, int logPriority, const char* format){
	log4c_category_log(logObj, logPriority, format);
}
#endif

void logEvent(char* str, int priority, int logType, Log_Master* logMaster){
	switch(logType){
		case ERROR:
			log4c_category_log(logMaster->errorLogger.logObj, priority, str);
			break;
		case INFO:
			log4c_category_log(logMaster->operationLogger.logObj, priority, str);
			break;
		case NMEA:
			log4c_category_log(logMaster->nmeaLogger.logObj, priority, str);
			break;
		default:
			printf("unrecognized logType. \n");
			break;
	}
}

int finiLogSystem(void){
	fclose(logMaster.operationLogger.logFile);
	fclose(logMaster.errorLogger.logFile);
	int errCode = log4c_fini();
	if(errCode)
		printf("log4c_fini() failed.\n");
	return errCode;
}
