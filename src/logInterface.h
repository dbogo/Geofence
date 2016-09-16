
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

/* All the different log files are placed in a special 'logs' folder that is
	at the top of the project directory (same level as src/) */

#define OPERATION_LOG_FILE "logs/operation.log"
#define NMEA_LOG_FILE "logs/nmea_signals.log"
#define ERROR_LOG_FILE "logs/errors.log"

#define INFO 0
#define ERROR 1

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

Log_Master logMaster;

/**
 * @brief      initializes the Loggers from log4c library
 * @param      logMaster  LogMaster struct that contains all loggers
 * @return     errCode
 */
int initLogSystem(Log_Master* logMaster);

/* DEPRECATED */
#if 0
int initLogSystem(log4c_category_t** logObj, const char* instanceName, const char* appenderName);
void logEvent(log4c_category_t* logObj, int logPriority, const char* format);
#endif

/**
 * @brief      close and stop the logging system
 * @return     errCode
 */
int finiLogSystem(void);


/**
 * @brief      logs the message into the specified stream, using the specifeied logger.
 * @param      str       the log message
 * @param[in]  priority  priority of the log. see LOG4C_PRIORITY enums. (log4c_priority_level_t)
 * @param[in]  logType   The log type
 * @param logMaster a pointer to the Log_Master struct that contains log objects
 */
void logEvent(char* str, int priority, int logType, Log_Master* logMaster);

#endif /* LOGINTERFACE_H */
