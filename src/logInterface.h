
/*
LOG4C_PRIORITY_FATAL 	fatal
LOG4C_PRIORITY_ALERT 	alert
LOG4C_PRIORITY_CRIT 	crit
LOG4C_PRIORITY_ERROR 	error
LOG4C_PRIORITY_WARN 	warn
LOG4C_PRIORITY_NOTICE 	notice
LOG4C_PRIORITY_INFO 	info
LOG4C_PRIORITY_DEBUG 	debug
LOG4C_PRIORITY_TRACE 	trace
LOG4C_PRIORITY_NOTSET 	notset
LOG4C_PRIORITY_UNKNOWN 	unknown
*/

#ifndef LOGINTERFACE_H
#define LOGINTERFACE_H

#include "../log4c/include/log4c.h"
// <stdio.h> is already included in log4c 

/* All the different log files are places in a special 'logs' folder that is
	at the top of the project directory (same level as src/) */

/* TODO: consider redesigning the logEvent() function to require only the log message,
	prioritry, and the program will figure out on its own which logger to use for that.
	error/operation etc.. 
*/

#define OPERATION_LOG_FILE "logs/operation.log"
#define NMEA_LOG_FILE "logs/nmea_signals.log"
#define ERROR_LOG_FILE "logs/errors.log"

typedef struct Logger{
	FILE* logFile;
	log4c_category_t* logObj;
	char* logInstanceName;
} Logger;

typedef struct Log_Master{
	Logger operationLogger;
	Logger nmeaLogger;
	Logger errorLogger;
} Log_Master;

int initLogSystem(Log_Master* logMaster);
//int initLogSystem(log4c_category_t** logObj, const char* instanceName, const char* appenderName);
int finiLogSystem(void);
void logEvent(log4c_category_t* logObj, int logPriority, const char* format);





#endif /* LOGINTERFACE_H */
